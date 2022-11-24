#include <bits/stdc++.h>

#include "Dictionary.cc"

#define MAXWORDSIZE 14
namespace rules {
const int DOUBLE = 0x2;
const int TRIPLE = 0x4;
const int MULTI = 0x6;
}  // namespace rules

using Matrix = std::vector<std::string>;
using Seen = std::set<std::pair<int, int>>;

void dfs(const Matrix& lines, int r, int c, Seen visited,
         std::string cword = "") {
  static std::unordered_set<std::string> found;
  cword += lines[r][c];
  if (cword.size() >= MAXWORDSIZE) return;
  if (visited.count({r, c})) return;
  // std::cout << "cword: " << cword << "\n";
  // std::cout << "here at: " << r << " " << c << "\n";
  if (dictionary.contains(cword) and found.count(cword) == 0 and
      cword.size() > 2) {
    std::cout << "found: " << cword << "\n";
    found.insert(cword);
  }

  visited.insert({r, c});
  /*  visit neighbors */
  for (int nr = r - 1; nr < r + 2; ++nr) {
    if (nr < 0 || nr >= lines.size()) continue;
    for (int nc = c - 1; nc < c + 2; ++nc) {  // gen all pairs
      if (nc < 0 || nc >= lines[0].size()) continue;
      dfs(lines, nr, nc, visited, cword);
    }
  }
}

void bfs(const Matrix& lines, const Matrix& flags, int sr, int sc,
         std::vector<std::pair<std::string, int>>& results) {
  static std::unordered_set<std::string> found;

  struct Item {
    std::pair<int, int> pos;
    std::string cword;
    Seen visited;
    int value;
    bool is_multi = false;
  };

  std::queue<Item> Q;
  for (int i{}; i < 5; ++i) {
    for (int j{}; j < 5; ++j) {
      Q.push({{i, j}, "", {}, 0});  // pos, cword, {visited}, value
    }
  }

  while (!Q.empty()) {
    Item f = Q.front();
    Q.pop();
    char to_add = lines[f.pos.first][f.pos.second];
    char flag = flags[f.pos.first][f.pos.second];
    f.cword += to_add;  // cleaned
    if (flag == 'X')
      f.is_multi = true;
    else {
      f.value += dictionary.getCharValue(to_add) *
                 (flags[f.pos.first][f.pos.second] - 48);
    }

    if (!dictionary.isPrefix(f.cword)) continue;
    if (f.cword.size() >= MAXWORDSIZE) continue;
    if (f.visited.count(f.pos)) continue;
    if (dictionary.contains(f.cword) and found.count(f.cword) == 0 and
        f.cword.size() > 2) {
      int value = f.value * (f.is_multi ? 2 : 1);
      results.push_back({f.cword, value});
      // std::cout << value << " " << f.cword << "\n";
      found.insert(f.cword);
    }

    f.visited.insert(f.pos);
    /*  visit neighbors */
    int r, c;
    std::tie(r, c) = f.pos;  // maybe doesn't workjhk
    for (int nr = r - 1; nr < r + 2; ++nr) {
      if (nr < 0 || nr >= lines.size()) continue;
      for (int nc = c - 1; nc < c + 2; ++nc) {  // gen all pairs
        if (nc < 0 || nc >= lines[0].size()) continue;
        Q.push({{nr, nc},
                f.cword,
                f.visited,
                f.value});  // pos, cword, {visited}, value, ismulti
      }
    }
  }
}

int main() {
  Matrix lines;
  Matrix flags(5, "11111");
  std::string line;
  while (std::cin >> line) {
    std::string clean = "";
    bool offset = 0;
    for (int x{}; x < line.size(); ++x) {
      if (line[x] == '2' or line[x] == '3' or line[x] == 'X') {
        flags[lines.size()][x - offset] = line[x];
        offset = 1;
        continue;
      }
      // 2abyXpt
      clean += line[x];
    }
    lines.push_back(clean);
  }

  for (string s : lines) {
    std::cout << s << "\n";
  }
  std::cout << "\nFLAGS\n";
  for (string s : flags) {
    std::cout << s << "\n";
  }

  /* start a dfs at each starting position */
  // { /* dfs */
  //   for (int r{}; r < 5; ++r) {
  //     for (int c{}; c < 5; ++c) {
  //       std::set<std::pair<int, int>> visited;  // construct new visi per
  //       trav. dfs(lines, r, c, visited);
  //     }
  //   }
  // }

  std::vector<std::pair<std::string, int>> results;

  bfs(lines, flags, 0, 0, results);

  std::sort(begin(results), end(results),
            [](auto a, auto b) { return a.second > b.second; });

  // std::cout << results[0].first << " " << results[0].second << "\n";
  for (auto [word, val] : results) {
    std::cout << word << " " << val << "\n";
  }
  // auto result = dictionary.prefix.startsWith("meaningfulness");
  // std::cout << std::boolalpha << result << "\n";
  // auto result = dictionary.contains("meaningfulness");
  // std::cout << std::boolalpha << result << "\n";

  return 0;
}