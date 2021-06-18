#include <cassert>
#include <cstdio>
#include <ctime>

#include <algorithm>
#include <vector>

#define X true
#define O false

using IntVec = std::vector<int>;

static std::vector<IntVec> combinations = {
	{ 0, 1, 2 },
	{ 3, 4, 5 },
	{ 6, 7, 8 },
	{ 0, 3, 6 },
	{ 1, 4, 7 },
	{ 2, 5, 8 },
	{ 0, 4, 8 },
	{ 2, 4, 6 }
};

struct Board {
	std::vector<bool> board;

	Board() {
		board.resize(18);
		for (int i = 9; i < board.size(); ++i) {
			board[i] = false;
		}
	}

	bool finished() const {
		if (won(true) || lost(true)) return true; // either player won

		for (int i = 9; i < board.size(); ++i) {
			if (!board[i]) return false;
		}

		return true; // tie
	}

	bool lost(bool side) const {
		return won(!side);
	}

	bool won(bool side) const {
		bool result = false;
		for (unsigned i = 0; i < combinations.size(); ++i) {
			if (checkComb(combinations[i], side)) return true;
		}
		return false;
	}

	bool myTurn(bool side) const {
		int xs = 0;
		int os = 0;
		for (int i = 0; i < 9; ++i) {
			if (!board[i + 9]) continue; // field not marked yet

			if (board[i] == X) ++xs;
			else ++os;
		}

		if (xs == os) return side == X;

		assert(xs == os + 1);
		return side == O;
	}

	bool checkEmpty(unsigned i) const {
		return !board[i + 9];
	}

	Board mark(int i, bool side) const {
		assert(!board[i + 9]);

		auto res = *this;
		res.board[i] = side;
		res.board[i + 9] = true;
		return res;
	}

	void print() const {
		char digit[2];
		digit[0] = '-';
		digit[1] = '\0';

		for (int i = 0; i < 9; ++i) {
			if (i != 0 && i % 3 == 0) {
				printf("|\n");
			}
			if (i % 3 == 0) {
				printf("-------------\n");
			}

			if (board[i + 9]) {
				digit[0] = (board[i] == X ? 'X' : 'O');
			} else {
				digit[0] = (char)(i + '0');
			}
			printf("| %s ", digit);
		}
		printf("|\n-------------\n");
	}

private:
	bool checkComb(const IntVec &idx, bool side) const {
		for (unsigned i = 0; i < idx.size(); ++i) {
			if (!board[idx[i] + 9]) return false; // field is not marked
			if ((bool)board[idx[i]] != side) return false;
		}
		return true;
	}
};

int alphabeta(Board board, bool side, bool maxSide, int alpha, int beta, Board *nextMove);

Board minimax(Board board, bool side) {
	assert(board.myTurn(side) && !board.finished());

	Board nextMove;
	alphabeta(board, side, side, -1, 1, &nextMove);
	return nextMove;
}

int alphabeta(Board board, bool side, bool maxSide, int alpha, int beta, Board *nextMove) {
	if (board.won(side)) {
		return (side == maxSide ? +1 : -1);
	} else if (board.won(!side)) {
		return (side == maxSide ? -1 : +1);
	} else if (board.finished()) {
		return 0;
	}

	bool isMaximizing = (side == maxSide);

	int val = isMaximizing ? -1 : +1;

	int bestVal = val;
	Board best;

	for (int i = 0; i < 9; ++i) {
		if (!board.checkEmpty(i)) continue;

		Board next = board.mark(i, side);

		if (isMaximizing) {
			val = std::max(val, alphabeta(next, !side, maxSide, alpha, beta, nullptr));
			alpha = std::max(alpha, val);

			if (val > bestVal) {
				bestVal = val;
				best = next;
			}

			if (alpha >= beta) {
				break;
			}
		} else {
			val = std::min(val, alphabeta(next, !side, maxSide, alpha, beta, nullptr));
			beta = std::min(beta, val);

			if (val < bestVal) {
				bestVal = val;
				best = next;
			}

			if (alpha >= beta) {
				break;
			}
		}
	}

	if (nextMove) *nextMove = best;
	return bestVal;
}

void gameloop() {
	system("cls");
	int fr;
	do {
		printf("X begins first!\n");
		printf("Choose fraction! X(1) or O(2): ");
		scanf_s("%d", &fr);
		if (fr == 1 || fr == 2) {
			break;
		}

		printf("Invalid fraction!\n");
	} while (true);

	bool side = (fr == 1);
	Board game;
	if (side == O) {
		printf("Computer:\n");
		game = minimax(game, X);
		game.print();
	} else {
		game.print();
	}

	while (!game.finished()) {
		printf("Choose a field number: ");
		int field;
		scanf_s("%d", &field);
		if (field < 0 || field >= 9 || !game.checkEmpty(field)) {
			printf("Invalid filed number!\n");
			continue;
		}

		printf("You:\n");
		game = game.mark(field, side);
		game.print();
		if (game.finished()) {
			break;
		}

		printf("Computer:\n");
		game = minimax(game, !side);
		game.print();
	}

	if (game.won(side)) {
		printf("Computer: Lucky guy, hooman!\n");
	} else if (game.lost(side)) {
		printf("Computer: ggwp, ez!\n");
	} else {
		printf("Computer: b-bye.");
	}
}

void test() {
	Board game;
	srand((unsigned)time(0));
	int i = rand() % 9;
	game = game.mark(i, X);
	printf("X move:\n");
	game.print();


	while (!game.finished()) {
		game = minimax(game, O);
		if (game.finished()) {
			break;
		}
		printf("O move:\n");
		game.print(); 
		
		game = minimax(game, X);
		printf("X move:\n");
		game.print();
	}

	assert(game.finished() && !game.won(X) && !game.won(O));
}

int main(int argc, char **argv) {
	printf("Welcome to Tic-Tac-Toe!\n");
	
	//test();

	while (true) {
		gameloop();

		printf("\n\nWanna play again? y/n: ");
		char ans;
		scanf_s(" %c", &ans);
		if (strncmp(&ans, "y", 1)) {
			break;
		}
	}

	int i;
	scanf_s("%d", &i);

	return 0;
}