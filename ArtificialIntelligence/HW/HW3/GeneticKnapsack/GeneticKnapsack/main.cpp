#include <cassert>
#include <cstdio>

#include <algorithm>
#include <chrono>
#include <vector>
#include <random>

using Number = int;
#define NUMBER_FMT "%d"
#define FIVE_MIN_IN_MILLIS 300000

using Array = std::vector<Number>;
using BitArray = std::vector<bool>;
using std::chrono::duration_cast;
using hrc = std::chrono::high_resolution_clock;

// Uniform real distribution random generator
struct RNG {
	std::random_device device;
	std::mt19937_64 generator;
	std::uniform_real_distribution<double> dist;

	RNG(double min, double max) : generator(device()), dist(min, max) { }

	double generate() { return dist(generator); }
};

struct Genom {
	BitArray array; // 0-1 array of genom
	Number f; // fitness of genom

	BitArray::reference operator[](unsigned i) {
		assert(i < array.size());

		return array[i];
	}
};

struct GeneticKnapsack {
	Array w, v;
	BitArray answer; ///< We use the first element to indicate if the algorithm has ran
	Number constraint;
	mutable Number weight;
	mutable Number value;

	int populationSize = 250;
	int maxIter = 500;
	int stop = 50;
	float elitism = 0.01f;

	GeneticKnapsack(Array &w, Array &v, Number r) : 
		w(std::move(w)), 
		v(std::move(v)), 
		answer({ false }), 
		constraint(r),
		weight(-1), 
		value(-1) 
	{
	
	}

	void setObjects(Array &w, Array &v, Number r) {
		w.clear();
		v.clear();
		answer.clear();

		w = std::move(w);
		v = std::move(v);
		answer.push_back(false);

		constraint = r;

		weight = -1;
		value = -1;
	}

	void run(int &iter) {
		assert(!bool(answer[0]) && !w.empty() && !v.empty() && w.size() == v.size());
		int N = w.size();

		answer.resize(N + 1);

		std::vector<Genom> pool;
		pool.resize(populationSize);
		for (unsigned i = 0; i < pool.size(); ++i) {
			generateNewArray(pool[i]);
		}

		// Current maximum fitness
		Number maxFitness = std::numeric_limits<Number>::min();
		BitArray maxFitnessElement;
		int convergenceIter = 0;

		auto compFunc = [](const Genom &a1, const Genom &a2) { return a1.f > a2.f; };

		// Clock for stop
		auto start = hrc::now();

		// Generations
		int &i = iter;
		i = 0;
		for (; i < maxIter; ++i) {
			auto end = hrc::now();
			if (duration_cast<std::chrono::minutes>(end - start).count() > 1) {
				break;
			}

			++convergenceIter;
			if (convergenceIter >= stop) {
				break;
			}

			std::sort(pool.begin(), pool.end(), compFunc);
			Number f = pool[0].f;
			if (i % 10 == 0) printf("f: %d\n", f);
			if (f > maxFitness) {
				printf("new f: %d, w: %d, i: %d\n", f, computeWeight(pool[0].array), i);
				maxFitness = f;
				maxFitnessElement = pool[0].array;
				convergenceIter = 0;
			}

			RNG crossoverProb(0, 1);
			for (unsigned j = pool.size() - 1; j >= elitism * pool.size(); --j) { 
				if (crossoverProb.generate() > 0.15) {
					crossover(pool, j);
				} 

				mutate(pool[j]);
			}
		}

		answer[0] = true; //< algorithm ran successfully
		for (unsigned i = 1; i < answer.size(); ++i) {
			answer[i] = bool(maxFitnessElement[i - 1]);
		}
	}

	void printAns() const {
		assert(bool(answer[0]));

		for (unsigned i = 1; i < answer.size(); ++i) {
			printf("%d ", bool(answer[i]));
		}
	}

	Number getAnsWeight() const {
		assert(bool(answer[0]));

		if (weight > 0) {
			return weight;
		}

		return (weight = computeWeight(answer, 1));
	}

	Number getAnsValue() const {
		assert(bool(answer[0]));

		if (value > 0) {
			return value;
		}

		return (value = fitness(answer, 1));
	}

private:
	void generateNewArray(Genom &result) {
		int N = w.size();
		result.array.resize(N);

		RNG g(0, 1);

		Number ws = 0;
		result.f = 0;
		for (int i = 0; i < N; ++i) {
			result[i] = (g.generate() >= 0.4);
			result.f += int(result[i]) * v[i];
			ws += int(result[i]) * w[i];
		}
		checkFitness(result.f, ws);
	}

	Number computeWeight(const BitArray &a, int offset = 0) const {
		assert(a.size() - offset == w.size());
		
		Number sum = 0;
		for (unsigned i = offset; i < a.size(); ++i) {
			sum += int(a[i]) * w[i - offset];
		}

		return sum;
	}

	Number fitness(const BitArray &a, int offset = 0) const {
		assert(a.size() - offset == v.size());

		Number sum = 0;
		for (unsigned i = offset; i < a.size(); ++i) {
			sum += int(a[i]) * v[i - offset];
		}

		return sum;
	}

	void fitnessWeight(const BitArray &a, Number &fitnessSum, Number &weightSum, int offset = 0) const {
		assert(a.size() - offset == v.size());
		
		Number ws = 0;
		for (unsigned i = offset; i < a.size(); ++i) {
			fitnessSum += int(a[i]) * v[i - offset];
			ws += int(a[i]) * w[i - offset];
		}

		weightSum = ws;
		checkFitness(fitnessSum, ws);
	}

	// using this paper: for crossover and mutation methods
	// http://www.sc.ehu.es/ccwbayes/docencia/kzmm/files/AG-knapsack.pdf
	void crossover(std::vector<Genom> &pool, int i) {
		RNG crossoverProb(0, 1);

		static int px = 0;
		
		float p1Prob = crossoverProb.generate();
		float p2Prob = crossoverProb.generate();
		
		int p1 = getParentIdx(p1Prob, pool.size());
		int p2 = getParentIdx(p2Prob, pool.size());

		int idx = (int) (crossoverProb.generate() * w.size());

		Number ws = 0;

		for (int j = 0; j < idx; ++j) {
			pool[i][j] = bool(pool[p1][j]);
			ws += int(pool[i][j]) * w[j];
			pool[i].f += int(pool[i][j]) * v[j];
		}

		for (unsigned j = idx; j < w.size(); ++j) {
			pool[i][j] = bool(pool[p2][j]);
			ws += int(pool[i][j]) * w[j];
			pool[i].f += int(pool[i][j]) * v[j];
		}

		checkFitness(pool[i].f, ws);
	}

	void mutate(Genom &g) {
		RNG d(0, 1);

		for (int i = 0; i < w.size(); ++i) {
			if (d.generate() < 0.1) {
				g.array[i] = !bool(g.array[i]);
			}
		}

		g.f = 0;
		Number ws;
		fitnessWeight(g.array, g.f, ws);
	}

	void checkFitness(Number &f, Number weightSum) const {
		if (weightSum > constraint) {
			f -= 7 * (weightSum - constraint);
		}
	}

	int getParentIdx(double prob, int n) {
		RNG rng(0, 1);
		
		float part = 0.25 * n;

		if (prob < 0.5) {
			return rng.generate() * part;
		} else if (prob < 0.8) {
			return rng.generate() * part + part;
		} else if (prob < 0.95) {
			return rng.generate() * part + 2 * part;
		} else {
			return rng.generate() * part + 3 * part;
		}
	}
};

int main(int argc, char **argv) {
	int N;
	Number r;
	printf("Please enter the number of objects: \n");
	scanf_s(NUMBER_FMT, &N);

	printf("Please enter the weight restriction: \n");
	scanf_s(NUMBER_FMT, &r);

	Array w;
	Array v;
	w.resize(N);
	v.resize(N);

	printf("Please enter the weights of the objects: ");
	for (int i = 0; i < N; ++i) {
		scanf_s(NUMBER_FMT, &w[i]);
	}

	printf("Please enter the value of the objects: ");
	for (int i = 0; i < N; ++i) {
		scanf_s(NUMBER_FMT, &v[i]);
	}

	int iterations;
	GeneticKnapsack gk{ w, v, r };
	gk.run(iterations);

	printf("\nAnswer : ");
	gk.printAns();
	printf("\n\tweight: %d\n\tvalue: %d\nFound in: %d iterations", gk.getAnsWeight(), gk.getAnsValue(), iterations);

	int i;
	scanf_s("%d", &i);
}
