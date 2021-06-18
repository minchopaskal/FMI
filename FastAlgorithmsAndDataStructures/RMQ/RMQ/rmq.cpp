#include "../../RMQ/RMQ/rmq.h"

// C includes
#include <cassert>
#include <cmath>

// C++ includes
#include <algorithm>

using baron::RMQ;

RMQ::RMQ(const Array &array, bool useNaive) : array(array), flags(0) {
	setFlag(array_initialized);
	if (!array.empty()) setFlag(array_not_empty);
	if (useNaive) setFlag(use_naive);
}

void baron::RMQ::setArray(const Array &array) {
	clearFlags();

	this->array = array;
	index.clear();
	log.clear();
	power.clear();

	setFlag(array_initialized);
	if (!array.empty()) setFlag(array_not_empty);
}

void RMQ::makeIndex() {
	assert(checkFlag(array_initialized | array_not_empty));

	if (checkFlag(use_naive)) {
		rmqIndexNaive();
	} else {
		rmqLog();
	}

	setFlag(index_made);
}

int RMQ::query(int start, int end) const {
	assert(checkFlag(index_made) && start >= 0 && end >= start && (Array::size_type)end < array.size());

	if (checkFlag(use_naive)) {
		return index[start][end];
	} else {
		int l = log[end - start + 1];
		
		int endLeft = end - power[l];
		
		int a = index[l][start];
		int b = index[l][endLeft];

		if (array[a] <= array[b]) return a;
		return b;
	}
}

void baron::RMQ::rmqIndexNaive() {
	if (checkFlag(index_made)) {
		return;
	}

	index.resize(array.size());
	std::for_each(index.begin(), index.end(), 
		[this](Array &a) -> void { a.resize(array.size()); });

	// For each inclusive tuple (i, j) i <= j record 
	// argmin(array[i...j])
	for (Array::size_type i = 0; i < array.size(); ++i) {
		int m = array[i];
		int idx = i;
		
		for (Array::size_type j = i; j < array.size(); ++j) {
			if (array[j] < m) {
				m = array[j];
				idx = j;
			}
			index[i][j] = idx;
		}
	}
}

void baron::RMQ::rmqLog() {
	assert(!checkFlag(index_made));
	
	// logN = floor(log N)
	int logN = (int) log2(array.size());

	index.resize(logN + 1);
	std::for_each(index.begin(), index.end(),
		[this](Array &a) -> void { a.resize(array.size()); }
	);
	
	log.resize(array.size() + 1);
	power.resize(logN + 1);

	// log[k] == l iff 2^l <= k < 2^(l+1)
	for (Array::size_type i = 1; i < log.size(); ++i) {
		// TODO: test which is faster

		//log[i] = (int)log2(i);
		
		int l = 0;
		while ((1 << l) <= (int)i) {
			++l;
		}
		log[i] = l - 1;
	}

	// pow[d] == 2 ^ d - 1
	for (Array::size_type i = 0; i < power.size(); ++i) {
		power[i] = (1 << i) - 1;
	}

	for (Array::size_type i = 0; i < index[0].size(); ++i) {
		index[0][i] = i;
	}

	// for each power of 2 p smaller than array.size()
	// index[p][i] == RMQ(i, i + 2^p - 1)
	for (Array::size_type i = 1; i < index.size(); ++i) {
		for (Array::size_type j = 0; j < index[0].size(); ++j) {
			if (j + power[i - 1] + 1 >= array.size()) {
				break;
			}
			int a = index[i - 1][j];
			int b = index[i - 1][j + power[i - 1] + 1];

			if (array[a] <= array[b]) {
				index[i][j] = a;
			} else {
				index[i][j] = b;
			}
		}
	}
}

void baron::RMQ::clearFlags() {
	flags = use_naive;
}

void baron::RMQ::setFlag(RMQFlag flag) {
	flags |= flag;
}

void baron::RMQ::removeFlag(RMQFlag flag) {
	flags &= (~flag);
}

bool baron::RMQ::checkFlag(int flag) const {
	return flags & flag;
}
