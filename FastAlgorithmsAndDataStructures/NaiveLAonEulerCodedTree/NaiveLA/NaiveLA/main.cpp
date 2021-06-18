#include <cassert>
#include <cstdio>

#include <string>
#include <stack>

struct EulerCodedTree {
private:
	// TODO: make bits a bitset
	std::string bits;

public:
	EulerCodedTree(const std::string &encoding) { init(encoding); }

	void init(const std::string &encoding) {
		if (!checkEncoding(encoding)) {
			bits.clear();
			return;
		}
		bits = encoding;
	}

	bool initialized() const { return !bits.empty(); }

	int getCount() const { return bits.size(); }

	int getTreeSize() const {
		assert(initialized());

		return bits.size() / 2 + 1;
	}

	int getBit(int i) const {
		int res = strncmp(&bits[i], "0", 1);
		assert(res == 0 || res == 1);

		return res;
	}

	int* getTable() const;

private:
	bool checkEncoding(const std::string &encoding) {
		// encoding should have equal num of zeroes and ones so 
		// its size should be even number
		if (encoding.size() % 2 == 1) {
			return false;
		}

		int onesCnt = 0;
		int zeroesCnt = 0;
		for (size_t i = 0; i < encoding.size(); ++i) {
			if (!strncmp(&encoding[i], "1", 1)) {
				++onesCnt;
			} else if (!strncmp(&encoding[i], "0", 1)) {
				++zeroesCnt;
			} else {
				return false;
			}

			if (zeroesCnt > onesCnt) {
				return false;
			}
		}

		return true;
	}
};

int* EulerCodedTree::getTable() const {
	if (!initialized()) {
		return nullptr;
	}

	int sz = getTreeSize();
	if (sz == 1) {
		return new int(0);
	}
	
	std::stack<int> tree;
	tree.push(0);

	int *res = (int*)malloc(sz * sz * sizeof(int));
	for (int i = 0; i < sz; ++i) {
		int idx = i * sz;
		res[idx] = i;
		
		// Make first row nulls except the 0th ancesstor of the root which is the root.
		if (i == 0) continue;
		idx = 0 * sz + i;
		res[idx] = -1;
	}

	int maxN = 0;
	for (int i = 0; i < getCount(); ++i) {
		if (getBit(i) == 1) {
			++maxN;
			tree.push(maxN);
			continue;
		}

		// bit is zero
		int n = tree.top();
		tree.pop();
		int p = tree.top();
		
		// res[n][1] = p;
		int idx = n * sz + 1;
		res[idx] = p;
	}

	for (int i = 1; i < sz; ++i) {
		bool reachedOne = false;
		int parent = res[i * sz + 1];

		for (int j = 2; j < sz; ++j) {
			int idx = i * sz + j;
			
			if (reachedOne) {
				res[idx] = -1;
			} else {
				int parentIdx = parent * sz + (j - 1);
				res[idx] = res[parentIdx];
				if (res[idx] == 0) {
					reachedOne = true;
				}
			}
		}
	}

	return res;
}

int main(int argc, char **argv) {
	std::string tree{ "1110100110100010" };
	
	EulerCodedTree t{ tree };
	int *table = t.getTable();

	int sz = t.getTreeSize();
	for (int i = 0; i < sz; ++i) {
		for (int j = 0; j < sz; ++j) {
			printf("%d ", table[i * sz + j] + 1);
		}

		printf("\n");
	}

	return 0;
}