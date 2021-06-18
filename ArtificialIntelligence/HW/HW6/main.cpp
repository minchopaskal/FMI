#include <cstdio>
#include <cstring>

#include <algorithm>
#include <limits>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <class T>
using Vec = std::vector<T>;

template <class K, class V>
using Map = std::unordered_map<K, V>;

template <class K>
using Set = std::unordered_set<K>;

using Str = std::string;

using DataType = float;
using Point = Vec<DataType>;

// Uniform real distribution random generator
struct RNG {
  std::random_device device;
  std::mt19937_64 generator;
  std::uniform_int_distribution<int> dist;

  RNG(int min, int max) : generator(device()), dist(min, max) { }

  int generate() { return dist(generator); }
};

float sqr(float x) {
  return x * x;
}

struct Cluster {
  Point centroid;
  Vec<Point> points;
};

template <int Iters>
struct KMeans {
private:
  Vec<Point> data;
  Vec<Cluster> clusters;
  Vec<int> clusterIds; ///< cluster id for each point

  int dataDim;
  
  int k;
  
public:
  explicit KMeans(int k, Vec<Point> data) : k(k), data(data) {
    dataDim = data[0].size();
    clusters.resize(k);
    clusterIds.resize(data.size(), -1);
  }

  // Set new data from `data`
  // Set new k from `k` or keep the old k if `k`==-1
  void setData(Vec<Point> data, int k = -1) {
    this->data = data;
    this->k = k == -1 ? this->k : k;
  }

  // Clusterize data
  void fit() {
    RNG rng(0, k);
    Set<int> used;

    // choose centroids
    for (int i = 0; i < k; ++i) {
      int idx = rng.generate();
      while (used.count(idx) == 1) {
        idx = rng.generate();
      }

      used.insert(idx);
      clusters[i].centroid = data[idx];
      clusters[i].points.push_back(data[idx]);
      clusterIds[idx] = i;
    }

    bool isArangmentChanged = true;
    int iters = 1;
    while (isArangmentChanged) {
      // For each point finds its nearest cluster
      for (int i = 0; i < data.size(); ++i) {
        int currCluster = clusterIds[i];
        int nearestCluster = getNearestCluster(data[i]);
        
        if (currCluster == nearestCluster) {
          continue;
        }

        isArangmentChanged = true;
        if (currCluster != -1) {
          auto& points = clusters[currCluster].points;
          std::remove(points.begin(), points.end(), data[i]);
        }

        clusterIds[i] = nearestCluster;
        clusters[nearestCluster].points.push_back(data[i]);
      }

      if (!isArangmentChanged) {
        break;
      }

      // Find the new centroids of the clusters
      for (int i = 0; i < k; ++i) {
        auto& cluster = clusters[i];
        if (cluster.points.size() == 0) {
          continue;
        }

        Point centroid;
        centroid.resize(dataDim, 0);
        for (int j = 0; j < dataDim; ++j) {
          for (int l = 0; l < cluster.points.size(); ++l) {
            centroid[j] += cluster.points[l][j];
          }
          centroid[j] /= cluster.points.size();
        }

        // centroid may not be in the dataset
        cluster.centroid = centroid;
      }

      if (iters >= Iters) {
         break;
      }
      
      ++iters;
      isArangmentChanged = false;
    }

    for (int i = 0; i < k; ++i) {
      printf("Cluster %d:\n", i + 1);
      for (int j = 0; j < clusters[i].points.size(); ++j) {
        printf("Point: ");
        for (int l = 0; l < dataDim; ++l) {
          printf("%f ", clusters[i].points[j][l]);
        }
        printf("\n");
      }
      printf("\n");
    }
    printf("Centroid sizes: ");
    for (int i = 0; i < k; ++i) {
      printf("%d ", clusters[i].points.size());
    }
    printf("\nK-Means fitting completeed in %d iterations!\n", iters);
  }

  // TODO: maybe find the nearest point and returns its cluster?
  // Predict id of cluster
  int predict(Point x) {
    return getNearestCluster(x);
  }
  
  const Cluster* getClusterByID(int id) {
    if (id < 0 || id >= k) {
      return nullptr;
    }

    return &clusters[id];
  }
  
private:
  int getNearestCluster(const Point &p) {
    float mindist = std::numeric_limits<float>::max();
    int clusterId = -1;

    for (int i = 0; i < k; ++i) {
      float dist = 0.f;
      for (int j = 0; j < dataDim; ++j) {
        dist += sqr(clusters[i].centroid[j] - p[j]);
      }

      if (dist < mindist) {
        mindist = dist;
        clusterId = i;
      }
    }

    return clusterId;
  }
};

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

  KMeans<1000> algo(3, data);
  algo.fit();
  // sample is taken directly from iris.arff
  int predictedClusterId = algo.predict({6.3f, 3.4f, 5.6f, 2.4f});
  auto cluster = *algo.getClusterByID(predictedClusterId);

  printf("prediction of (6.3, 3.4, 5.6, 2.4) cluster: %d\n", predictedClusterId);
  // for (int i = 0; i < cluster.points.size(); ++i) {
  //   for (int j = 0; j < cluster.points[0].size(); ++j) {
  //     printf("%f ", cluster.points[i][j]);
  //   }
  //   printf("\n");
  // }
  
  return 0;
}
