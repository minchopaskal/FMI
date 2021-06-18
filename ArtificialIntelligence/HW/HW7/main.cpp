// god why didn't i wrote this in python

#include <cstdio>
#include <cmath>

#include <algorithm>
#include <limits>
#include <numeric>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

template <class T>
using Vec = std::vector<T>;

template <class K, class V>
using Pair = std::pair<K, V>;

using String = std::string;

using DataType = float;
using DataVec = Vec<DataType>;

using Data = Vec<DataVec>;

constexpr int SEX_CNT = 2; // political incorrectness :(
constexpr int MALE = 0;
constexpr int FEMALE = 1;

inline float sqr(float x) {
  return x * x;
}

// Naive Bayes Classifier
struct NBC {
private:
  Vec<Data> data;
  Vec<String> catergories;

  Vec<float> P_y;
  Vec<Vec<Pair<float, float>>> muSigmas;
  
public:
  NBC(const Vec<Data> &data, const Vec<String> &categories) : data(data), catergories(categories) {
    if (data.size() > catergories.size()) {
      int n = catergories.size();
      catergories.resize(data.size());
      std::for_each(catergories.begin() + n, catergories.end(), [](String &str) {str = "NA";});
    } else if (data.size() < catergories.size()) {
      catergories.resize(data.size());
    }
  }

  void fit() {
    int samplesCount = std::accumulate(data.begin(), data.end(), 0,
                                       [](int acc, const Data &v) { return acc + v.size(); });

    P_y.resize(catergories.size()); // P(y_i) = ?
    for (int i = 0; i < data.size(); ++i) {
      P_y[i] = data[i].size() / float(samplesCount);
    }

    // for each class
    // for each feature
    // calculate it's median and variance
    muSigmas.resize(catergories.size());
    int featuresCnt = data[0][0].size();
    for (int i = 0; i < catergories.size(); ++i) {   
      muSigmas[i].resize(featuresCnt);
      int classSize = data[i].size();
      
      for (int j = 0; j < classSize; ++j) {
        for (int k = 0; k < featuresCnt; ++k) {
          muSigmas[i][k].first += data[i][j][k]; // mu
        }
      }

      for (int j = 0; j < featuresCnt; ++j) {
        muSigmas[i][j].first /= classSize;
      }

      // Variance
      for (int j = 0; j < classSize; ++j) {
        for (int k = 0; k < featuresCnt; ++k) {
          muSigmas[i][k].second += sqr(data[i][j][k] - muSigmas[i][k].first); // mu
        }
      }

      for (int j = 0; j < featuresCnt; ++j) {
        muSigmas[i][j].second /= (classSize - 1);
      }
    }
  }

  int predict(const DataVec &X) {
    int idx = -1;
    float prob = std::numeric_limits<float>::min();

    int featuresCnt = data[0][0].size();
    
    // argmax(y) { P(y) * P(X | y) }
    for (int i = 0; i < catergories.size(); ++i) {
      float product = P_y[i];
      for (int j = 0; j < featuresCnt; ++j) {
        product *= gauss(X[j], muSigmas[i][j].first, muSigmas[i][j].second);
      }

      if (product > prob) {
        prob = product;
        idx = i;
      }
    }

    return idx;
  }
  
  String getCategoryById(int id) {
    if (id < 0 || id >= catergories.size()) {
      return "invalid";
    }

    return catergories[id];
  }
  
private:
  static float gauss(float x, float mu, float sigmaSq) {
    static constexpr float c = 1.f / sqrt(2 * M_PI);

    return c * (1.f / sqrt(sigmaSq)) * exp(-((x - mu) * (x - mu)) / (2 * sigmaSq));
  }
};

int main(int argc, char **argv) {
  // Prepare data
  Vec<Data> data;
  data.push_back({{6, 180, 12}, {5.92f, 190, 11}, {5.58f, 170, 12}, {5.92f, 165, 10}});
  data.push_back({{5, 100, 6}, {5.5f, 150, 8}, {5.42f, 130, 7}, {5.75, 150, 9}});
  Vec<String> categories;
  categories.resize(8, "male");
  std::for_each(categories.begin() + 4, categories.end(), [](String &str) {str = "female";});

  // train
  NBC algo{data, categories};
  algo.fit();

  DataVec X = {5.4f, 140, 10};
  int catId = algo.predict(X);

  printf("(5.4, 140, 10) was predicted to be in category %s\n", algo.getCategoryById(catId).c_str());
  
  return 0;
}
