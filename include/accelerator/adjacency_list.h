#pragma once

#include <index/index.h>
#include <index/query_iterable.h>

#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct AdjacencyList
{
  std::unordered_set<Index, IndexHash> keys{};
  std::unordered_multimap<Index, Index> data{};

  [[maybe_unused]] inline void AddEdge(Index a, Index b) noexcept
  {
    keys.insert(a);
    keys.insert(b);
    data.insert({ a, b });
    data.insert({ b, a });
  }

  [[nodiscard]] [[maybe_unused]] inline auto Query(Index key) noexcept
  {
    auto [beg, end] = data.equal_range(key);
    return QueryIterable{ beg, end };
  }

  [[nodiscard]] [[maybe_unused]] inline auto QueryIslands(auto consumer) noexcept
  {
    // DFS

    std::stack<Index> s{};
    std::vector<Index> island{};
    std::unordered_multimap<Index, Index> island_edges{};

    while (!keys.empty()) {
      s.push(*keys.begin());
      while (!s.empty()) {
        auto v = s.top();
        s.pop();
        if (keys.erase(v)) {
          island.push_back(v);
          for (auto [_, w] : Query(v)) {
            s.push(w);
            island_edges.emplace(v, w);
          }
        }
      }

      consumer(island, island_edges);

      island_edges.clear();
      island.clear();
    }
  }

  void Clear()
  {
    keys.clear();
    data.clear();
  }
};
