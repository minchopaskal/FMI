// C std
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// C++ std
#include <algorithm>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include <unordered_map>

template <class T>
using Vec = std::vector<T>;

template <class K, class V>
using Map = std::unordered_map<K, V>;

using DataType = float;

using Str = std::string;

template <int K>
struct KNN {
  using Data = Vec<Vec<DataType>>;

  Data data;
  //Data transposed; // needed to find easily min/max in collumns
  Vec<Str> dataClass;
  Vec<DataType> Mins, Maxs; // for normalization
  Map<Str, int> classInd;

  KNN(Data &&data, Vec<Str> &&dataClass) : data(std::move(data)), dataClass(std::move(dataClass)) {}

  void train() {
    // data is assumed to be in row-major order
    // Mins.resize(data[0].size());
    // Maxs.resize(data[0].size());

    // transposed.resize(data[0].size());
    // for (int i = 0 ; i < transposed.size(); ++i) {
    //   transposed[i].resize(data.size());
    // }

    // for (int i = 0; i < data.size(); ++i) {
    //   for (int j = 0; j < data[i].size(); ++j) {
    //     transposed[j][i] = data[i][j];
    //   }
    // }

    // TODO: Mins[i] = *std::min_element(transposed[i].cbegin(), transposed[i].cend());
    // TODO: Maxs[i] = --||--;

    // TODO: normalize data

    initIndices();
  }

  Str predict(const Vec<DataType> &test) {
    Vec<DataType> testValue = test;

    //normalize(testValue);
    Vec<DataType> dists;
    Vec<DataType> ind;
    for (int i = 0; i < data.size(); ++i) {
      dists.emplace_back(getDist(testValue, data[i]));
      ind.push_back(i);
    }
    
    std::sort(ind.begin(), ind.end(), [&dists](int i1, int i2) {return dists[i1] < dists[i2];});

    // The mean value of the K-nearest classes
    Vec<int> results;
    for (int i = 0; i < K; ++i) {
      results.push_back(classInd[dataClass[ind[i]]]);
    }

    int sum = std::accumulate(results.begin(), results.end(), 0, std::plus<int>());
    sum /= results.size();

    for (auto p : classInd) {
      if (p.second == sum) {
        return p.first;
      }
    }

    // assert(false && "Class not foound!\n");

    // Most frequent element version:
    // Vec<Str> results;
    // Vec<int> cnts(classInd.size(), 0);

    // for (int i = 0; i < K; ++i) {
    //   cnts[classInd[dataClass[ind[i]]]]++;
    // }

    // int resInd = -1;
    // int max = -1;
    // for (int i = 0; i < cnts.size(); ++i) {
    //   if (cnts[i] > max) {
    //     max = cnts[i];
    //     resInd = i;
    //   }
    // }

    // for (auto p : classInd) {
    //   if (p.second == resInd) {
    //     return p.first;
    //   }
    // }
    
    return dataClass[0];
  }

private:
  void normalize(Vec<DataType> &value) {
    assert(value.size() == Mins.size() && "Test value have diff dims\n");
    
    for (int i = 0; i < value.size(); ++i) {
      value[i] -= Mins[i];
      value[i] /= (Maxs[i] - Mins[i]);
    }
  }

  float getDist(const Vec<DataType> &v1, const Vec<DataType> &v2) {
    assert(v1.size() == v2.size());
    float res = 0.f;
    for (int i = 0; i < v1.size(); ++i) {
      res += (v1[i] - v2[i]);//* (v1[i] - v2[i]);
    }
    return res;
  }

  void initIndices() {
    int idx = 0;
    for (int i = 0; i < dataClass.size(); ++i) {
      if (classInd.find(dataClass[i]) != classInd.end()) {
        continue;
      }

      classInd[dataClass[i]] = idx++;
    }
  }
  
};

bool isnumeric(const char *str) {
  bool foundDot = false;

  while (*str != '\0') {
    if (!isdigit(*str)) {
      if (foundDot == true) {
        return false;
      }
      
      if (*str != '.') {
        return false;
      }

      foundDot = true;
    }

    ++str;
  }

  return true;
}

template <typename T>
Vec<T> reorder(const Vec<T> v, const Vec<int> &ind) {
  Vec<T> result(v.size(), T{});
  for (int i = 0; i < v.size(); ++i) {
    result[i] = v[ind[i]];
  }

  return result;
}

int main(int argc, char **argv) {
  // read data...
  FILE *dataFile = fopen("iris.arff", "r");
  if (dataFile == nullptr) {
    printf("invalid file!\n");
    exit(1);
  }
  
  Vec<Vec<DataType>> data;
  Vec<Str> classes;

  ssize_t bytes;
  size_t len = 0;
  char *line = nullptr;

  printf("opened file!\n");
  
  int lineno = 0;
  while ((bytes = getline(&line, &len, dataFile)) != -1) {
    data.push_back({});
    classes.push_back({});
    char *next = nullptr;
    while ((next = strchr(line, ',')) != nullptr) {
      *next = '\0';
      data[lineno].push_back(strtof(line, nullptr));
      line = next + 1;
    }
    next = strchr(line, '\n');
    classes[lineno] = Str{line, next};
    ++lineno;
  }

  assert(data.size() == classes.size() && "Data and classes should have the same size!\n");
  
  printf("file read\n");

  // One tenth of the data for testing
  Vec<int> indices(data.size(), 0);
  std::iota(indices.begin(), indices.end(), 0);
  std::shuffle(indices.begin(), indices.end(), std::mt19937{std::random_device{}()});

  data = reorder(data, indices);
  classes = reorder(classes, indices);
  
  int oneTenth = data.size() / 10;
  Vec<Vec<DataType>> testData{data.begin() + data.size() - oneTenth, data.end()};
  Vec<Str> testClasses{classes.begin() + classes.size() - oneTenth, classes.end()};
    
  data.resize(data.size() - oneTenth);
  classes.resize(classes.size() - oneTenth);
  
  KNN<2> algo{std::move(data), std::move(classes)};
  algo.train();

  int correct = 0;
  for (int i = 0; i < testData.size(); ++i) {
    Str prediction = algo.predict(testData[i]);
    printf("Test %s, Prediction: %s", testClasses[i].c_str(), prediction.c_str());
    if (testClasses[i] == prediction) {
      ++correct;
      printf("\n");
    } else {
      printf(" WRONG\n");
    }
  }

  printf("Correct predictions: %.2f%% (%d/%d)\n", (correct / float(testData.size())) * 100, correct, testData.size());
   
  return 0;
}
