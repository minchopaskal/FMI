#pragma once

#include <vector>

namespace baron {

/// Range minimum query algorithm
struct RMQ {
	using Array = std::vector<int>;

private:
	using Table = std::vector<std::vector<int>>;

	enum RMQFlag : int {
		use_naive = (1 << 0),
		array_initialized = (1 << 1),
		array_not_empty = (1 << 2),
		index_made = (1 << 3),
	};

	Array array; ///< Array on which we are working
	Table index; ///< Index used by queries
	Array log; ///< log[k] == l iff 2^l <= k < 2^(l+1)
	Array power; ///< pow[d] == 2 ^ d - 1

	int flags; ///< Flags used to indicate state of the algorithm

public:
	RMQ(const Array &array = {}, bool useNaive = false);

	/// Change the array we are working on.
	/// Invalidates the index if one is created.
	void setArray(const Array &array);

	/// Create index on the array, which will then be used
	/// for making queries
	void makeIndex();

	int query(int start, int end) const;

private:
	void rmqIndexNaive();
	void rmqLog();

	void clearFlags();
	void setFlag(RMQFlag flag);
	void removeFlag(RMQFlag flag);
	bool checkFlag(int flag) const;
};

} // namespace baron
