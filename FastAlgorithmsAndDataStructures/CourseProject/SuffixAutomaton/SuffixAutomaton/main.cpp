#include <errno.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define KB_64 64000 
#define SZ 200'000'000

#ifndef RELEASE
#define PRINTF(format, ...) printf(format, __VA_ARGS__)
#include <string>
#else
#define PRINTF(...) (void)0
#endif

struct suffix_automaton {
private:
	/// Nodes
	int *transitionIdx;
#ifndef RELEASE
	int *link;
	int *len;
#endif

	char *labels; ///< labels[i] is the label of transition[i]
	int *transitions; ///< index in [0, 2 * count - 1]

	int *nextTransition; ///< linked list of transitions

	int count; ///< count of nodes
	mutable int trCount; ///< count of transitions

	int sink; ///< last added node

	int finalStCount; ///< count of final states

public:
	suffix_automaton(FILE *filePtr) : 
		transitionIdx(nullptr),
		labels(nullptr),
		nextTransition(nullptr),
		count(1),
		sink(0),
		trCount(1),
		finalStCount(0)
	{
		fseek(filePtr, 0, SEEK_SET);
		unsigned int begin = ftell(filePtr);
		fseek(filePtr, 0, SEEK_END);
		unsigned int end = ftell(filePtr);
		rewind(filePtr);
		unsigned int fileSize = end - begin;
		
		// initialize arrays
#ifndef RELEASE
		link = new int[2 * fileSize]; // helper array tracks suffix links
		len = new int[2 * fileSize]; // helper to track node's representative's length
#else
		int *link = new int[2 * fileSize]; // helper array tracks suffix links
		int *len = new int[2 * fileSize]; // helper to track node's representative's length
#endif

		transitionIdx = new int[2 * fileSize];
		labels = new char[3 * fileSize];
		transitions = new int[3 * fileSize];
		nextTransition = new int[3 * fileSize];

		// initial values
		link[0] = -1;
		len[0] = 0;

		unsigned int i = 0;
		for (; i < 2 * fileSize; ++i) {
			// zeroes are invlid transitions since all transitions
			// go forward and there are no loops
			transitions[i] = 0;
			nextTransition[i] = 0;

			// The first transition is invalid in order
			// to store wether the state is final in the sign of the index
			transitionIdx[i] = 0;
		}

		for (; i < 3 * fileSize; ++i) {
			transitions[i] = 0;
			nextTransition[i] = 0;
		}

		memset(labels, 0x00, 3 * fileSize);

		build(filePtr, link, len);
#ifdef RELEASE
		delete[] link;
		delete[] len;
#endif
	}

	~suffix_automaton() {
		delete[] transitionIdx;

		delete[] labels;
		delete[] transitions;
		delete[] nextTransition;
	
#ifndef RELEASE
		delete[] link;
		delete[] len;
#endif
	}

	int getStatesCount() const {
		return count;
	}

	int getTransitionsCount() const {
		if (trCount != -1) {
			return trCount;
		}

		trCount = 0;
		for (int i = 0; i < count; ++i) {
			int trIdx = getTransitionIdx(i);
			while (trIdx != 0) {
				++trCount;
				trIdx = nextTransition[trIdx];
			}
		}

		return trCount;
	}

	int getFinalStatesCount() const {
		return finalStCount;
	}

	bool isSuffix(const char *str) const {
		if (str == nullptr || strlen(str) == 0) {
			PRINTF("IsSuffix EPS\n");
			return true;
		}

		int trIdx = 0;
		const char *p = str;
		while (*p) {
			trIdx = checkTransition(trIdx, *p);
			if (trIdx == 0) {
				PRINTF("Not suffix: %s\n", str);
				return false;
			}

			++p;
		}

		if (isStateFinal(trIdx)) {
			PRINTF("Is suffix %s\n", str);
			return true;
		} else {
			PRINTF("Not suffix: %s\n", str);
			return false;
		}
	}

#ifndef RELEASE
	void printAutomaton() const {
		for (int i = 0; i < count; ++i) {
			printf("===================================================\n");
			printf("State %d, len: %d\n", i, len[i]);
			printf("final: %s\n", (isStateFinal(i) ? "true" : "false"));
			printf("Transitions:\n");
			printf("%d ~suffix~> %d\n", i, link[i]);
			
			int trIdx = getTransitionIdx(i);
			while (trIdx != 0) {
				printf("%d -(%c)-> %d\n", i, labels[trIdx], transitions[trIdx]);
				trIdx = nextTransition[trIdx];
			}
		}
		printf("===================================================\n");
	}

	void writeAsDOT(const char *filename) const {
		FILE *f = fopen(filename, "w");
		if (!f) {
			printf("Error while writing as DOT file. Opening file %s failed with error: %s!\n", filename, strerror(errno));
			return;
		}

		fprintf(f, "digraph SuffixAutomaton {\n");
		{
			for (int i = 0; i < count; ++i) {
				if (isStateFinal(i)) {
					fprintf(f, "\t%s [color=\"green\" style=\"bold\"];\n", std::to_string(i).c_str());
				}

				if (link[i] != -1) {
					fprintf(f, "\t%s -> %s [style=\"dashed\"];\n", std::to_string(i).c_str(), std::to_string(link[i]).c_str());
				}

				int trIdx = getTransitionIdx(i);
				while (trIdx != 0) {
					fprintf(f, "\t%s -> %s [label=\"%c\"];\n", std::to_string(i).c_str(), std::to_string(transitions[trIdx]).c_str(), labels[trIdx]);
					trIdx = nextTransition[trIdx];
				}
			}
		}
		fprintf(f, "}");
	}
#endif

private:
	int getTransitionIdx(int state) const {
		return abs(transitionIdx[state] & 0x7F'FF'FF'FF);
	}

	bool isStateFinal(int state) const {
		return transitionIdx[state] < 0;
	}

	void build(FILE *filePtr, int *link, int *len) {
		// TODO: assert filePtr is opened
		static char buffer[KB_64];
	
		int read = 0;
		int all = 0;
		do {
			read = fread(buffer, 1, KB_64, filePtr);
			if (read != KB_64 && ferror(filePtr)) {
				PRINTF("Error while reading file!\n");
				exit(1);
			}

			all += read;
			if (all % (16 * KB_64) == 0) {
				PRINTF("Read %d bytes!\n", all);
			}

			for (int i = 0; i < read; ++i) {
				append(buffer[i], link, len);
			}
		} while (read > 0);

		markFinalStates(link);
		trCount = -1; // compute it later
	}

	// Using the marvelous CP-Algorithms Suffix Automaton article
	// and https://cs.nyu.edu/~mohri/pub/nfac.pdf paper which
	// is based on Blumer&Blumer et. al. (1985)
	void append(char c, int *link, int *len) {
		int newstate = count++;
		len[newstate] = len[sink] + 1;
		int parent = sink;
		while (parent != -1 && (checkTransition(parent, c) == 0)) {
			addTransition(parent, c, newstate);
			parent = link[parent];
		}

		if (parent == -1) {
			link[newstate] = 0;
		} else {
			int child = checkTransition(parent, c);
			if (len[parent] + 1 == len[child]) {
				link[newstate] = child;
			} else {
				int clone = split(parent, child, c, link, len);
				link[child] = clone;
				link[newstate] = clone;
			}
		}
		sink = newstate;
	}

	int checkTransition(int state, char c) const {
		int trIdx = getTransitionIdx(state);
		do {
			if (labels[trIdx] == c) {
				return transitions[trIdx];
			}
			trIdx = nextTransition[trIdx];
		} while (trIdx != 0);

		return 0;
	}

	int split(int parent, int child, char c, int *link, int *len) {
		int clone = count++;

		len[clone] = len[parent] + 1;
		link[clone] = link[child];
		copyTransitions(child, clone);

		while (parent != -1 && checkTransition(parent, c) == child) {
			setTransition(parent, c, clone);
			parent = link[parent];
		}

		return clone;
	}

	void copyTransitions(int from, int to) {
		int trIdx = getTransitionIdx(from);
		while (trIdx != 0) {
			char label = labels[trIdx];
			int dest = transitions[trIdx];

			addTransition(to, label, dest);

			trIdx = nextTransition[trIdx];
		}
	}

	void setTransition(int state, char label, int dest) {
		int trIdx = transitionIdx[state];
		while (labels[trIdx] != label) {
			trIdx = nextTransition[trIdx];
		}
		transitions[trIdx] = dest;
	}

	void addTransition(int from, char label, int to) {
		int trIdx = transitionIdx[from];
		int prev = 0;
		while (trIdx != 0) {
			prev = trIdx;
			trIdx = nextTransition[trIdx];
		}

		int newTrIdx = newTransition(label, to);

		if (prev == 0) { // adding the first transition
			transitionIdx[from] = newTrIdx;
			nextTransition[newTrIdx] = 0;
		} else {
			assert(nextTransition[prev] == 0);
			nextTransition[prev] = newTrIdx;
			nextTransition[newTrIdx] = 0;
		}
	}

	int newTransition(char label, int to) {
		int newTr = trCount++;
		labels[newTr] = label;
		transitions[newTr] = to;
		return newTr;
	}

	void markFinalStates(int *link) {
		int s = sink;
		while (s != -1) {
			// We are using the sign bit as a flag for final state
			transitionIdx[s] |= 0x80'00'00'00;
			s = link[s];
			++finalStCount;
		}
	}
};

int main(int argc, char **argv) {
	//if (argc != 2) {
	//	printf("Invalid number of arguments!\nUsage ./suffix_automaton [txt file]\n");
	//	exit(2);
	//}

	FILE *f = nullptr;
	auto err = fopen_s(&f, "test_file_100M.txt", "r");

	//auto err = fopen_s(&f, argv[1], "r");
	if (err != 0) {
		printf("Opening file %s failed with error: %s!\n", argv[1], strerror(err));

		int a;
		scanf_s("%d", &a); 
		exit(1);
	}

	suffix_automaton sa(f);

	//sa.printAutomaton();
	//sa.writeAsDOT("graph.dot");

	//sa.isSuffix("a");
	//sa.isSuffix("bcb");
	//sa.isSuffix("");
	//sa.isSuffix("c");
	//sa.isSuffix("bc");
	//sa.isSuffix("cbc");
	//sa.isSuffix("bcbc");
	//sa.isSuffix("abcbcd");
	//sa.isSuffix("cbcd");

	printf("States: %d\n", sa.getStatesCount());
	printf("Transitions: %d\n", sa.getTransitionsCount());
	printf("Final states: %d\n", sa.getFinalStatesCount());

	int a;
	scanf_s("%d", &a);
}