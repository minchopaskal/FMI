#include "rmq_pm1.h"

using baron::RMQPM1;

Node* RMQPM1::LCA(Node *a, Node *b) {
	assert(start.find(a) != start.end() && start.find(b) != start.end());

	int iA = start[a];
	int iB = start[b];

	if (iA > iB) {
		std::swap(iA, iB);
	}

	int k = RMQ(iA, iB);
	return time[k];
}

int RMQPM1::RMQ(int i, int j) {
	if (i > j) {
		return -1;
	}

	// find block indices
	int I = i / B;
	int J = j / B;

	if (I == J) {
		int k = RMQTable[numD[I]].query(i - I * B, j - I * B);
		return k + I * B;
	}

	if (I + 1 == J) {
		int k1 = RMQTable[numD[I]].query(i - I * B, B - 1);
		int k2 = RMQTable[numD[J]].query(0, j - J * B);
		if (depth[k1 + I * B] <= depth[k2 + J * B]) {
			return k1;
		}
		return k2;
	}

	// I + 1 <= J - 1
	int k1 = RMQTable[numD[I]].query(i - I * B, B - 1) + I * B;
	int k2 = RMQTable[numD[J]].query(0, j - J * B) + J * B;
	int K = rmqLogM.query(I + 1, J - 1);
	int k3 = Ind[K] + K * B;

	if (depth[k1] <= depth[k2]) {
		if (depth[k1] <= depth[k3]) {
			return k1;
		} else if (depth[k3] <= depth[k2]) {
			return k3;
		}
		return k2;
	} else if (depth[k3] <= depth[k2]) {
		return k3;
	}
	return k2;
}

void RMQPM1::clear() {
	t = 0;
	B = 0;
	depth.clear();
	time.clear();
	start.clear();
	RMQTable.clear();
	Ind.clear();
	rmqLogM.setArray({});
}

void RMQPM1::init(Node *tree, int nodesCnt) {
	clear();

	B = (int)(log2(nodesCnt) / 2) + 1;

	int edgeTraverse = 2 * (nodesCnt - 1) + 1;

	int mod = edgeTraverse % B;
	edgeTraverse = edgeTraverse + (mod == 0 ? 0 : B - mod);

	depth.resize(edgeTraverse);
	time.resize(edgeTraverse);

	tree2RMQ(tree, 0);

	buildRMQTableIndex();

	// The function bellow computes the minimums of each block
	// of the depth array and makes an array for which a RMQLog table
	// is computed using the (O(nlogn), O(1)) rmq method.
	// Also it computes the index of each such block in the RMQTable table
	// which stores the naively computed indicies of each different block
	// Since each block is of size B and can be though of as always begining 
	// with 0, an the difference b/n each 2 different elements in it is 1,
	// then there are 2^(B-1) such blocks. For each of them a naive table
	// is computed in buildRMQTableIndex()
	buildLogIndex(edgeTraverse / B);
}

void RMQPM1::tree2RMQ(Node *tree, int currDepth) {
	if (tree == nullptr) {
		return;
	}
	start[tree] = t;
	depth[t] = currDepth;
	time[t] = tree;
	for (auto c : tree->children) {
		++t;
		tree2RMQ(c, currDepth + 1);
		time[t] = tree;
		depth[t] = currDepth;
	}
	++t;
}

void RMQPM1::buildRMQTableIndex() {
	int p = 1 << (B - 1);
	RMQTable.resize(p, { {}, true });

	int sumD = 0;
	for (int i = 0; i < p; ++i) {
		std::vector<int> D = generateNextVector(i, sumD);
		RMQTable[sumD].setArray(D);
		RMQTable[sumD].makeIndex();
	}
}

void RMQPM1::buildLogIndex(int m) {
	int el = 0;
	int currIdx = 0;

	std::vector<int> mins;
	mins.resize(m);
	Ind.resize(m);

	numD.resize(m);
	int numDi = 0;

	for (unsigned i = 0; i < depth.size() + 1; ++i) {
		if (i % B == 0) {
			// We reached the end of the array, save the last block and go resting
			if (i == depth.size()) {
				mins[m - 1] = el;
				Ind[m - 1] = currIdx;
				break;
			}

			if (i > 0) {
				// save the index(in RMQtable) of the previous block
				numD[i / B - 1] = numDi;

				// save the min element and its index of the previous block
				mins[i / B - 1] = el;
				Ind[i / B - 1] = currIdx;
			}

			numDi = 0; // reset the numDi sum

			// reset the minimum element and its index
			el = depth[i];
			currIdx = 0;
		} else {
			// update the index of the array
			numDi += ((depth[i] - depth[i - 1] + 1) / 2) * (1 << (i % B - 1));

			if (depth[i] < el) {
				el = depth[i];
				currIdx = i % B;
			}
		}
	}

	rmqLogM.setArray(mins);
	rmqLogM.makeIndex();
}

// N here is used to obtain the next binary vector which
// represents a sequence of the form {-1, 1}*
// A vector D(of size B) of which the first element is 0 and all the 
// rest fullfil the condition ||D[i] - D[i-1]|| == 1
// will be computed using naive RMQ method
std::vector<int> RMQPM1::generateNextVector(int n, int &numD) {
	std::vector<int> result;
	result.resize(B);
	result[0] = 0;
	result[1] = (n % 2 ? 1 : -1);

	numD = (result[1] - result[0] + 1) / 2;

	int i = B - 1;
	while (i >= 2) {
		// result[i] here represents D[i] - D[i-1]
		result[i] = n % 2 ? 1 : -1;
		numD += (result[i] + 1) * (1 << (i - 1));

		// Obtain the original Di array
		result[i] += result[i - 1];

		n /= 2;
		++i;
	}

	return std::move(result);
}
