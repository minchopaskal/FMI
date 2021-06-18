#include <cassert>
#include <cstdio>
#include <ctime>

#include <algorithm>
#include <chrono>
#include <stack>
#include <vector>
#include <queue>

#include <unordered_map>
#include <unordered_set>

using hrc = std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

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
	mutable int h = -1;

public:
	bool useManhatan = true;

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
		assert(next.valid(w));

		int n = res.table[next.get1D(width())];
		res.table[zero().get1D(width())] = n;
		res.zeroPos = next.get1D(width());
		res.table[res.zeroPos] = 0;

		res.p.push(step);

		// calculate heuristics function
		res.h = -1;
		res.staraZagoraDistance();

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
		if (h > 0) {
			return h;
		}

		// Lazy eval h
		h = 0;
		for (int i = 0; i < count; ++i) {
			if (table[i] != 0) {
				h += useManhatan ? staraZagoraDistance(table[i], i) : euclidDistance(table[i], i);
			}
		}

		return h;
	}

	int cost() const {
		// cost == fScore
		// staraZagoraDistance == heuristics
		// p.size(number of steps) == gScore 

		return staraZagoraDistance() + p.size();

		// A better heuristics, although apearantly not well implemented since it works way slower :/
		//return staraZagoraDistance() + 2 * linearConflicts();
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

		h = other.h;
	}

	// Another name for Manhatan distance is staraZagora distance for obvious reasons
	int staraZagoraDistance(int number, int numPos) const {
		const int y = numPos / width();
		const int x = numPos - y * width();

		const int offset = targetPos >= number ? -1 : 0;
		const int realPos = number + offset;

		const int realY = realPos / width();
		const int realX = realPos - realY * width();

		int result = abs(realX - x) + abs(realY - y);
		return result;
	}

	int euclidDistance(int number, int numPos) const {
		const int y = numPos / width();
		const int x = numPos - y * width();

		const int offset = targetPos >= number ? -1 : 0;
		const int realPos = number + offset;

		const int realY = realPos / width();
		const int realX = realPos - realY * width();

		int result = sqrt((realX - x) * (realX - x) + (realY - y) * (realY - y));
		return result;
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

	assert(t1.w == t2.w);

	if (t1.targetPos != t2.targetPos) {
		return false;
	}

	for (int i = 0; i < t1.count; ++i) {
		if (t1.table[i] != t2.table[i]) {
			return false;
		}
	}

	assert(t1.zeroPos == t2.zeroPos);

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

void printPath(Path);

template <typename T>
using Map = std::unordered_map<Table, T>;

void runAStar(Table table, Path &solution) {
	using Queue = std::priority_queue<Table>;
	using Visited = std::unordered_set<Table>;

	static constexpr int skipTable[4] = { LEFT, UP, RIGHT, DOWN };

	Queue q; // priority queue
	q.push(table);

	Visited openSet;
	openSet.insert(table);

	Visited closedSet;


	while (!q.empty()) {
		Table t = q.top();

		q.pop();
		openSet.erase(t);

		const Path &steps = t.path();

		int skip = -1;
		if (!steps.empty()) {
			skip = skipTable[steps.top()];
		}
		for (int i = 0; i < Step::STEP_COUNT; ++i) {
			if (skip >= 0 && i == skip) {
				continue;
			}

			if (!t.path().empty())
				assert((Step)i != skipTable[t.path().top()]);

			Point p = t.zero() + dir[i];
			if (!p.valid(t.width())) {
				continue;
			}

			Table newTable = t.move(static_cast<Step>(i));

			if (newTable.checkOrdered()) {
				solution = newTable.path();
				return;
			}

			if ((openSet.find(newTable) != openSet.end()) && openSet.find(newTable)->cost() < newTable.cost()) {
				continue;
			}

			if ((closedSet.find(newTable) != closedSet.end()) && closedSet.find(newTable)->cost() < newTable.cost()) {
				continue;
			}

			q.push(newTable);
			openSet.insert(newTable);
		}

		closedSet.insert(t);
	}
}

void runBeamSearch(Table table, int b, Path &solution) {
	using Queue = std::queue<Table>;
	using PriorityQueue = std::priority_queue<Table>;
	using Visited = std::unordered_set<Table>;
	static constexpr int skipTable[4] = { LEFT, UP, RIGHT, DOWN };

	Visited closedSet;

	Queue q; // A normal queue
	q.push(table);

	PriorityQueue pq; // Used for the beam

	while (!q.empty()) {
		Table t = q.front();

		if (t.checkOrdered()) {
			solution = t.path();
			return;
		}

		q.pop();
		closedSet.insert(t);

		const Path &steps = table.path();

		int skip = -1;
		if (!steps.empty()) {
			skip = skipTable[steps.top()];
		}
		for (int i = 0; i < Step::STEP_COUNT; ++i) {
			if (skip > 0 && i == skip) {
				continue;
			}

			Point p = t.zero() + dir[i];
			if (!p.valid(t.width())) {
				continue;
			}

			Table newTable = t.move(static_cast<Step>(i));

			if (closedSet.find(newTable) != closedSet.end()) {
				continue;
			}

			pq.push(newTable);
		}

		// Add for search only nodes that are inside the beam
		while (!pq.empty() && b > 0) {
			q.push(pq.top());
			pq.pop();
		}

		// Prune nodes that weren't inside the beam
		while (!pq.empty()) {
			closedSet.insert(pq.top());
			pq.pop();
		}
	}
}

void printPath(Path path);

int main(int argc, char **argv) {
	int N;
	int targetPos;

	int method;
	printf("Available search methods:\n"
		"\tA* algorithm(1);\n"
		"\tBeam Search(2).\n"
		"\tDefault is A*.\n");

	printf("Please enter search method: ");
	scanf_s("%d", &method);
	if (method != 1 && method != 2) {
		method = 1;
	}

	int beamWidth = -1;
	if (method == 2) {
		printf("Please enter beam width(1-4): ");
		scanf_s("%d", &beamWidth);

		if (beamWidth <= 0) {
			printf("Beam width should be positive number!\n");
			return 10;
		}

		beamWidth = std::min(4, beamWidth);
	}

	printf("Please enter count of blocks and target position of the empty space: ");

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

	printf("Please enter the board itself(it should have %d numbers incl. the 0):\n", count);
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

	auto s = hrc::now();

	Path path;
	if (method == 1) {
		runAStar(table, path);
		hrc::time_point e = hrc::now();

		auto manhatanTime = duration_cast<std::chrono::milliseconds>(e - s);

		printf("Manhatan time: %d ms\n", manhatanTime.count());

		s = hrc::now();
		table.useManhatan = false;
		runAStar(table, path);
		e = hrc::now();
		manhatanTime = duration_cast<std::chrono::milliseconds>(e - s);
		printf("Euclid time: %d ms\n", manhatanTime.count());

	} else {
		runBeamSearch(table, beamWidth, path);
	}

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

	Step lastStep = Step::STEP_COUNT;
	while (!reversed.empty()) {
		Step s = reversed.top();
		reversed.pop();

		if (lastStep != STEP_COUNT) {
			switch (lastStep) {
			case UP:
				assert(s != DOWN);
				break;
			case DOWN:
				assert(s != UP);
				break;
			case LEFT:
				assert(s != RIGHT);
				break;
			case RIGHT:
				assert(s != LEFT);
				break;
			default:
				break;
			}
		}

		printf("%s\n", getStepString(s));
		lastStep = s;
	}
}
