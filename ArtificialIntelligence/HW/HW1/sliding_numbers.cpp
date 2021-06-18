#include <cassert>
#include <cstdio>
#include <ctime>

#include <algorithm>
#include <chrono>
#include <stack>
#include <vector>
#include <queue>

#include <unordered_set>

using hrc = std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

enum Step : char
{
	RIGHT = 0,
	DOWN,
	LEFT,
	UP,
	STEP_COUNT
};
using Path = std::stack<Step>;

struct Point {
	int x = 0;
	int y = 0;

	int get1D(int width) const {
		return y * width + x;
	}

	void print() const {
		printf("(%d, %d)", x, y);
	}

	bool valid(int width) const {
		return x >= 0 && x < width && y >= 0 && y < width;
	}
};

bool operator==(const Point &p1, const Point &p2) {
	return p1.x == p2.x && p1.y == p2.y;
}

Point operator+(const Point &p1, const Point &p2) {
	return { p1.x + p2.x, p1.y + p2.y };
}

bool operator<(const Point &p1, const Point &p2) {
	if (p1.x < p2.x) return true;
	if (p1.x > p2.x) return false;
	if (p1.y < p2.y) return true;
	return false;
}

static constexpr Point dir[4] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };


struct Table {
private:
	Path p; ///< Step we used to reach current state
	int *table; ///< Game table
	int count; ///< Squares in the table count
	int w; ///< Width of the board
	int zeroPos; ///< Current position of the zero
	int targetPos; ///< Target positon of the zero

public:
	Table(int count, int targetPos) :
		table(nullptr),
		count(count),
		w(sqrt(count)),
		zeroPos(-1),
		targetPos(targetPos)
	{
		table = (int*)malloc(count * sizeof(int));

		if (targetPos == -1) {
			this->targetPos = count - 1;
		}
	}

	Table(const Table &other) :
		table(nullptr)
	{
		copy(other);
	}

	Table& operator=(const Table &rhs) {
		if (this == &rhs) {
			return *this;
		}

		copy(rhs);

		return *this;
	}

	~Table() {
		if (table != nullptr) {
			free(table);
		}
	}

	Table move(Step step) const {
		Table res = *this;

		Point next = zero() + dir[step];
		int n = res.table[next.get1D(width())];
		res.table[zero().get1D(width())] = n;
		res.zeroPos = next.get1D(width());
		res.table[res.zeroPos] = 0;

		res.p.push(step);

		return res;
	}

	

	int operator[](int i) {
		assert(i >= 0 && i < count);
		return table[i];
	}

	int* at(int i) {
		assert(i >= 0 && i < count);
		return table + i;
	}

	const int* at(int i) const {
		assert(i >= 0 && i < count);
		return table + i;
	}

	// If the inversions count of the board are and odd number
	// the puzzle is not solvable
	bool solvable() const {
		int inversions = 0;
		for (int i = 0; i < count; ++i) {
			if (table[i] == 0) {
				continue;
			}

			for (int j = i + 1; j < count; ++j) {
				if (table[j] == 0) {
					continue;
				}

				if (table[j] < table[i]) {
					++inversions;
				}
			}
		}

		return inversions % 2 == 0;
	}

	int width() const {
		return w;
	}

	void setZero(int i) {
		zeroPos = i;
	}

	void setTarget(int i) {
		targetPos = i;
	}

	Point zero() const {
		Point res;
		res.y = zeroPos / width();
		res.x = zeroPos - res.y * width();

		return res;
	}

	Point target() const {
		Point res;
		res.y = targetPos / width();
		res.x = targetPos - res.y * width();

		return res;

	}

	const Path& path() const {
		return p;
	}

	int staraZagoraDistance() const {
		int res = 0;
		for (int i = 0; i < count; ++i) {
			if (table[i] != 0) {
				res += staraZagoraDistance(table[i], i);
			}
		}

		return res;
	}

	int cost() const {
		return staraZagoraDistance();

		// A better heuristics, although apearantly not well implemented since it works way slower :/
		//return staraZagoraDistance() + 2 * linearConflicts();
	}

	int linearConflicts() const {
		int res = 0;
		for (int i = 0; i < w; ++i) {
			res += lc(i, &Table::lcRow);
			res += lc(i, &Table::lcCol);
		}

		return res;
	}

	int lcCol(int col, int i, int j) const {
		// We assume i, j are on col

		int iReal = realPosition(i);
		int jReal = realPosition(j);

		int iy = iReal / w;
		int ix = iReal - iy * w;

		int jy = jReal / w;
		int jx = jReal - jy * w;

		// Their real positions are not on that col
		if (ix != jx || ix != col) {
			return 0;
		}

		return int((i < j && iReal > jReal) || (i > j && iReal < jReal));
	}

	int lcRow(int row, int i, int j) const {
		// We assume i, j are on row

		int iReal = realPosition(i);
		int jReal = realPosition(j);

		int irow = iReal / w;
		int jrow = jReal / w;

		// Their real positions are not on that row
		if (irow != jrow || irow != row) {
			return 0;
		}

		return int((i < j && iReal > jReal) || (i > j && iReal < jReal));
	}

	using ConflictFunc = int (Table::*)(int, int, int) const;
	int lc(int row, ConflictFunc conflictFunc) const {
		//std::vector<int> conflicts{ w, 0 };
		std::vector<std::vector<int>> conflicts;
		conflicts.resize(w);
		std::for_each(conflicts.begin(), conflicts.end(), [this](std::vector<int>& v) { v.resize(this->w, 0); });
		
		std::vector<int> c;
		c.resize(w, 0);

		for (int i = 0; i < w; ++i) {
			for (int j = 0; j < w; ++j) {
				if (j == i) {
					continue;
				}

				conflicts[i][j] = (this->*conflictFunc)(row, i, j);
				c[i] += conflicts[i][j];
			}
		}

		int res = 0;
		while (c.end() != 
			std::find_if(c.cbegin(), c.cend(),
				[](int i) {
					if (i != 0) return true;
					return false;
				}
			)) { // While there are elements != 0 (elements with conflicts)
			auto it = std::max_element(c.begin(), c.end());
			*it = 0; // move the conflicting element
			int index = it - c.begin();
			for (int i = 0; i < conflicts.size(); ++i) {
				if (i == index) {
					continue;
				}
				if (conflicts[i][index]) {
					--c[i];
				}
			}
			++res;
		}

		return res;
	}

	int realPosition(int idx) const {
		const int num = table[idx];
		const int y = num / width();
		const int x = num - y * width();

		const int offset = targetPos >= num ? -1 : 0;
		const int realPos = num + offset;

		return realPos;
	}

	/// Returns true if we reached the solution
	bool checkOrdered() const {
		int offset = 1;
		for (int i = 0; i < count; ++i) {
			if (i == targetPos) {
				if (table[i] != 0) {
					return false;
				}

				--offset;
				continue;
			}

			if (table[i] != i + offset) {
				return false;
			}
		}

		return true;
	}

	friend bool operator==(const Table&, const Table&);

private:
	void copy(const Table &other) {
		p = other.p;
		count = other.count;
		w = other.w;
		zeroPos = other.zeroPos;
		targetPos = other.targetPos;

		if (table) {
			free(table);
		}

		table = (int*)malloc(count * sizeof(int));
		for (int i = 0; i < count; ++i) {
			table[i] = other.table[i];
		}
	}

	// Another name for Manhatan distance is staraZagora distance for obvious reasons
	int staraZagoraDistance(int number, int numPos) const {
		const int y = numPos / width();
		const int x = numPos - y * width();

		const int offset = targetPos >= number ? -1 : 0;
		const int realPos = number + offset;

		const int realY = realPos / width();
		const int realX = realPos - realY * width();

		return abs(realX - x) + abs(realY - y);
	}
};

// Hash for Table in order to be used in the visited unordered_set
namespace std {
template <>
struct hash<Table> {
	size_t operator()(const Table& t) const
	{
		const int *table = t.at(0);
		size_t res = 0;
		for (int i = 0; i < t.width() * t.width(); ++i) {
			res += table[i] ^ t.realPosition(table[i]);
		}
		res ^= t.zero().x;
		res ^= t.zero().y;
		return res;
	}
};
}

bool operator==(const Table &t1, const Table &t2) {
	if (t1.count != t2.count) {
		return false;
	}

	if (t1.targetPos != t2.targetPos) {
		return false;
	}

	for (int i = 0; i < t1.count; ++i) {
		if (t1.table[i] != t2.table[i]) {
			return false;
		}
	}

	Path p = t1.path();
	Path p1 = t2.path();

	if (t1.p.size() != p1.size()) {
		return false;
	}

	while (!p.empty()) {
		if (p.top() != p1.top()) {
			return false;
		}

		p.pop();
		p1.pop();
	}

	return true;
}

// This Manhatan heuristics is used in the priority queue
bool operator<(const Table &t1, const Table &t2) {
	return t1.cost() > t2.cost();
}

using Visited = std::unordered_set<Table>;

void printPath(Path p);

// Only updates path if solution is not found, so as theprevious iteration
// which reached the solution updates it
int runAStar(const Table &table, Visited &visited, int g, int threshold, Table &solution) {
	static constexpr int skipTable[4] = { LEFT, UP, RIGHT, DOWN };

	// f(x) == h(x) + g(x)
	// h = Table::cost()
	int f = table.cost() + g;

	if (f > threshold) {
		return f;
	}
	if (table.checkOrdered()) {
		solution = table;
		return -1; // indicates we found a solution
	}

	const Path& steps = table.path();

	/*printf("curr path:\n");
	printPath(steps);
	printf("\n");*/

	int skip = -1;
	if (!steps.empty()) {
		skip = skipTable[steps.top()];
	}
	int min = std::numeric_limits<int>::max();
	for (int i = 0; i < Step::STEP_COUNT; ++i) {
		// don't go back from where we came from
		if (skip > 0 && i == skip) {
			continue;
		}

		Point p = table.zero() + dir[i];
		if (!p.valid(table.width())) {
			continue;
		}

		Table newTable = table.move(static_cast<Step>(i));
		if (visited.find(newTable) != visited.end()) {
			continue;
		}

		visited.insert(newTable);
		// Assume step cost function is always one (we move one square in some direction)
		int t = runAStar(newTable, visited, g + 1, threshold, solution);
		if (t == -1) {
			return -1;
		}
		if (t < min) {
			min = t;
		}
	}

	return min;
}

void runIDAStar(const Table &table, Path &solutionPath) {
	int threshold = table.cost();
	Table solution(table);

	Visited v;
	v.insert(table);
	while (true) {
		int t = runAStar(table, v, 0, threshold, solution);
		if (t == -1) { // if we found a solution
			solutionPath = solution.path();
			return;
		}

		// clear visited
		while (!v.empty()) {
			v.erase(v.begin());
		}
		threshold = t;
	}
}

void printPath(Path path);

int main(int argc, char **argv) {
	int N;
	int targetPos;

	scanf_s("%d %d", &N, &targetPos);

	int count = N + 1;
	int width = sqrt(count);
	if (width * width != count) {
		printf("N should be a square of a number minus 1! Ex. N = 24 => sqrt(N + 1) == 5.\n");
		return 1;
	}

	if (targetPos < 0 || targetPos > N) {
		if (targetPos != -1) {
			printf("Invalid position!\n");
			return 2;
		}
	}

	Table table{ count, targetPos };

	int cntZeroes = 0;
	Point posZero;
	bool* markers = (bool*)malloc(N);
	for (int i = 0; i < count; ++i) {
		scanf_s("%d", table.at(i));

		if (table[i] == 0) {
			++cntZeroes;
			table.setZero(i);
		}

		if (table[i] < 0 || table[i] > N) {
			printf("Invalid number in the table: %d!\n", table[i]);
			return 3;
		}

		markers[table[i] - 1] = true;
	}

	if (cntZeroes != 1) {
		printf("There should be exactly one zero in the table!\n");
		return 4;
	}

	for (int i = 0; i < N; ++i) {
		if (!markers[i]) {
			printf("Not all numbers present!\n");
			return 5;
		}
	}

	if (!table.solvable()) {
		printf("Puzzle not solvable!\n");
		return 6;
	}


	Path path;
	
	auto start = hrc::now();
	runIDAStar(table, path);
	auto end = hrc::now();

	printf("Solution found in %d ms\n", duration_cast<milliseconds>(end - start).count());

	printf("%d\n", path.size());
	printPath(path);

	return 0;
}

// Had my logic backwards with that of the exercise
const char* getStepString(Step s) {
	switch (s) {
	case RIGHT:
		return "left";
	case DOWN:
		return "up";
	case LEFT:
		return "right";
	case UP:
		return "down";
	default:
		return "default";
	}
}

void printPath(Path path) {
	Path reversed;

	while (!path.empty()) {
		Step s = path.top();
		path.pop();

		reversed.push(s);
	}

	while (!reversed.empty()) {
		Step s = reversed.top();
		reversed.pop();

		printf("%s\n", getStepString(s));
	}
}
