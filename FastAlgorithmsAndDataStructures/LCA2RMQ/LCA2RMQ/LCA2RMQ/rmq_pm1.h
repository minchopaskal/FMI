#ifndef RMQ_PM1_H
#define RMQ_PM1_H

#include <cassert>
#include <map>

#include "../../../RMQ/RMQ/rmq.h"

// Example node-based tree structure
struct Node {
	int data; ///< Let data be index in some array of elements
	std::vector<Node*> children = {};
};

namespace baron {

/// RMQ plus-minus 1 problem
/// Index build and querying in time (O(n), O(1))
/// with the assumption that the difference between each
/// two neighbouring array elements is 1.
/// Answers Lowest common ancesstor queries, after the tree
/// is transformed into an array via euler touring.
struct RMQPM1 {
private:
	std::vector<int> depth;
	std::vector<Node*> time;
	std::map<Node*, int> start; ///< Only needed for LCA query, mapping can be outside of the class
	std::vector<baron::RMQ> RMQTable;
	std::vector<int> Ind;
	baron::RMQ rmqLogM;
	std::vector<int> numD;
	int t;
	int B;

public:
	RMQPM1(Node *tree, int nodesCnt) {
		init(tree, nodesCnt);
	}

	void setTree(Node *tree, int nodesCnt) {
		init(tree, nodesCnt);
	}

	Node* LCA(Node *a, Node *b);

	int RMQ(int i, int j);

private:
	void clear();

	void init(Node *tree, int nodesCnt);

	void tree2RMQ(Node *tree, int currDepth);

	void buildRMQTableIndex();

	void buildLogIndex(int m);

	std::vector<int> generateNextVector(int n, int &numD);
};

} // namespace baron

#endif // RMQ_PM1_H
