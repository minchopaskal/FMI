#include <cassert>
#include <cstdio>
#include <limits>
#include <vector>
#include <queue>
#include <stack>
#include <set>

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

struct Path {
private:
	using Stack = std::stack<Point>;
	using Visited = std::set<Point>;

	Stack path;
	Visited visitedPoints;
public:
	int portsUsed = false;

	bool visited(const Point &p) {
		return visitedPoints.find(p) != visitedPoints.end();
	}

	bool empty() const {
		return path.empty();
	}

	void push(const Point &p) {
		path.push(p);
		visitedPoints.insert(p);
	}

	void pop() {
		path.pop();
	}

	const Point& top() const {
		return path.top();
	}

	auto size() -> decltype(path.size()) const {
		return path.size();
	}

	// Don't count the first tile as step OR
	// |E| = |V| - 1 in a path
	auto pathLen() -> decltype(size()) const {
		assert(path.size() > 0);
		return size() - 1;
	}
};

enum LabyrinthFields {
	IMPASSABLE = 0,
	PASSABLE = 1,
	TELEPOR = 2
};

bool isInBounds(int val, int n) {
	return val >= 0 && val < n;
}

/// Print a given path
/// First reverse the path as it is a stack.
void printPath(Path p) {
	Path rev;
	while (!p.empty()) {
		rev.push(std::move(p.top()));
		p.pop();
	}

	while (!rev.empty()) {
		rev.top().print();
		rev.pop();
		if (!rev.empty()) {
			printf(" -> ");
		}
	}

	printf("\n");
}

/// Solution to 1.homework.
/// Traverse the map via DFS. Whenever a teleport is encountered
/// store the current state of the teleports before entering the recursion
/// @param map - a table representing the labyrinth.
/// @param W - the width of the labyrinth
/// @param s - starting position
/// @param t - target position
/// @param printCurrentPath - true if we want to log the BFS
/// @param length - for storing the solution's length
/// @param path - for storing the solution itself
void solveHW1(const int *const map, int W, const Point &s, const Point &t, const Point teleports[2], bool printCurrentPath, int &length, Path &path) {
	using Queue = std::queue<Path>;
	using Vector = std::vector<Point>;

	// right, down, left, up
	const Vector dir = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

	size_t minLen = std::numeric_limits<size_t>::max();
	Path minPath;

	Path startPath;
	startPath.push(s);

	Queue q; // push
	q.push(startPath);

	while (!q.empty()) {
		Path &path = q.front();

		assert(!path.empty());

		if (printCurrentPath) {
			printf("Current path:\n");
			printPath(path);
		}
		
		// We reached the target point
		if (path.top() == t) {
			if (path.pathLen() < minLen) {
				minLen = path.pathLen();
				minPath = std::move(path);
			}

			q.pop();
			continue;
		}

		// Small optimization
		// If the path is already longer than the minPath
		// no need continue with it
		if (path.pathLen() >= minLen) {
			q.pop();
			continue;
		}

		// If we landed on a teleport add the current position to the path
		// change it to the teleport value and mark the teleports as used.
		if (map[path.top().get1D(W)] == 2) {
			path.portsUsed = true;

			path.push(path.top() == teleports[0] ? teleports[1] : teleports[0]);
		}

		// Go around in all directions in search of a path
		for (unsigned int i = 0; i < dir.size(); ++i) {
			Point next = path.top() + dir[i];
			int idx = next.get1D(W);
			
			if (!next.valid(W)) {
				continue;
			}

			if (map[idx] == 0) {
				continue;
			} 

			if (map[idx] == 2 && path.portsUsed) {
				continue;
			}
			
			if (path.visited(next)) {
				continue;
			}

			// Generate a new path and add it to queue
			Path newPath = path;
			newPath.push(next);
			q.push(newPath);
		}

		q.pop();
	}

	length = minLen;
	path = std::move(minPath);
}

int main(int argc, char **argv) {
	bool printCurrentPath = false;
	
	if (argc > 1) {
		for (int i = 1; i < argc; ++i) {
			if (!strncmp(argv[1], "printCurrentPath", strlen(argv[1]))) {
				printCurrentPath = true;
				break;
			}
		}
	}

	int N = -1;
	int k = -1;
	Point start;
	Point target;
	Point teleports[2];

	/// Read problem parameters
	printf("Please enter: width of map, No. impassable, start point, target point\n");
	scanf_s("%d %d %d %d %d %d", &N, &k, &start.x, &start.y, &target.x, &target.y);

	if (N < 0 || !isInBounds(k, N * N) || !start.valid(N) || !target.valid(N) || start == target) {
		printf("Incorrect parameters!\n");
		return 1;
	}

	printf("Please enter map:\n");
	int *map = (int*)malloc(N * N * sizeof(int));
	if (map == nullptr) {
	  printf("Allocation of %lld bytes failed!\n", N * N * sizeof(int));
	  return 2;
	}
	
	int cntImpassable = 0;
	int cntTeleports = 0;
	for (int i = 0; i < N * N; ++i) {
	  scanf_s("%d", map + i); // read value at i


	  int y = i / N;
	  int x = i - y * N;
	  if (map[i] == 2) {
	    if (((start.x == x && start.y == y) || (target.x == x && target.y == y))) {
	      printf("Start or target position cannot be on a teleport!\n");
	      return 3;
	    }
	       
	    if (cntTeleports + 1 > 2) {
	      printf("Invalid number of teleports! Teleports must be exactly 2!\n");
	      return 4;
	    }

	    teleports[cntTeleports++] = {x, y};
	  }

	  if (map[i] == 0) {
		++cntImpassable;
		if ((start.x == x && start.y == y) || (target.x == x && target.y == y)) {
			printf("Start or target position must be passable!\n");
			return 5;
		}
	  }
	}

	if (cntTeleports != 2) {
		printf("Invalid number of teleports! Teleports must be exactly 2!\n");
		return 4;
	}
	
	if (cntImpassable != k) {
		printf("Invalid number of impassable tiles! They must be exactly %d!\n", k);
		return 6;
	}

	int length;
	Path path;
	solveHW1(map, N, start, target, teleports, printCurrentPath, length, path);

	printf("Shortest path is %d steps long. It is:\n", length);
	printPath(path);

	return 0;
}
