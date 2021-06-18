#ifndef GRAPH_CPP
#define GRAPH_CPP

#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>

template <class T>
class Graph {
private:
  using al = std::list<std::pair<T, std::size_t>>;
  using graph = std::unordered_map<T, al>;

  graph g;

  bool is_vertex(const T& vertex) const {
    return g.find(vertex) != g.end();
  }
  
public:
  Graph() {}

  void add_vertex(const T& vertex) {
    if(g.find(vertex) == g.end()) {
      g[vertex] = al();
    }
  }

  void add_edge(const T& start, const T& end, std::size_t weight) {
    add_vertex(start);
    add_vertex(end);

    g[start].push_back({end, weight});
    g[end].push_back({start, weight});
  }

  void BFS(const T& start) const {
    if(!is_vertex(start)) {
      return;
    }

    std::set<T> visited;
    std::queue<T> q;
    
    q.push(start);
    
    while (!q.empty()) {
      const T& curr = q.front();
      q.pop();

      std::cout << curr << " ";
      visited.insert(curr);
      
      for (auto it = g.at(curr).begin(); it != g.at(curr).end(); ++it) {
        if(visited.count(it->first) == 0) {
          q.push(it->first);
          visited.insert(it->first);
        }
      }
    }
    std::cout << std::endl;
  }

  void DFS(const T& start) const {
    if(!is_vertex(start)) {
      return;
    }

    std::set<T> visited;
    std::stack<T> s;
    s.push(start);
    visited.insert(start);
    
    std::cout << start << " ";
    
    while(!s.empty()) {
      const T& curr = s.top();

      auto it = g.at(curr).begin();
      while(it != g.at(curr).end() && visited.count(it->first)) {
        ++it;
      }

      if(it == g.at(curr).end()) {
        s.pop();
      } else {
        std::cout << it->first << " ";
        visited.insert(it->first);
        s.push(it->first);
      }
    }
    std::cout << std::endl;
  }

  bool has_path(const T& start, const T& end) const {
    if(!is_vertex(start) || !is_vertex(end)) {
      return false;
    }

    std::set<T> visited;
    std::queue<T> path;
    path.push(start);
    visited.insert(start);

    while(!path.empty()) {
      const T& curr = path.front();

      if(curr == end) {
        return true;
      }
      path.pop();

      for(auto it = g.at(curr).begin(); it != g.at(curr).end(); ++it) {
        if(!visited.count(it->first)) {
          path.push(it->first);
        }
      }
    }
    return false;
    
  }

  // Finds path as if the graph wasn't weighted 
  std::stack<T> find_path(const T& start, const T& end) const {
    std::stack<T> path;
    // If either vertex does not exist return empty path
    if (!is_vertex(start) || !is_vertex(end)) {
      return path;
    }

    std::set<T> visited;
    std::queue<T> q;
    std::stack<std::pair<T, T>> edges;
    q.push(start);

    // BFS the graph adding each edge until we reach the end vertex
    T curr = start;
    while(!q.empty() && (curr = q.front()) != end) {
      q.pop();
      for(auto it = g.at(curr).begin(); it != g.at(curr).end(); ++it) {
        if(!visited.count(it->first)) {
          q.push(it->first);
          visited.insert(it->first);
          edges.push({curr, it->first});
        }
      }
    }

    // If BFS didn't reach the end vertex there is no path
    if(curr != end) {
      return path;
    }

    // Follow the edges to figure out the path
    std::pair<T, T> e;
    while(!edges.empty()) {
      e = edges.top();
      edges.pop();
      if(curr == e.second) {
        path.push(curr);
        curr = e.first;
      }
    }

    // Since we begin with the start vertex we
    // can't have put it in the path stack
    // following the edges only - so let's do it manually
    path.push(start);

    // reverse the stack
    std::stack<T> ordered_path;
    while(!path.empty()) {
      ordered_path.push(path.top());
      path.pop();
    }

    return ordered_path;
  }

  void print() const {
    for(auto it = g.begin(); it != g.end(); ++it) {
      std::cout << it->first << ": { ";
      for(auto lit = it->second.begin(); lit != it->second.end(); ++lit) {
        std::cout << '(' << lit->first << ", " << lit->second << ") ";
      }
      std::cout << " }\n";
    }
  }
};

template <class T>
void print_path(std::stack<T> path) {
  while(!path.empty()) {
    std::cout << path.top() << " ";
    path.pop();
  }
  std::cout << std::endl;
}

#endif
