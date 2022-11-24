#include <bits/stdc++.h>

#include "Dictionary.cc"

#define MAXWORDSIZE 20

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

void bfs(const Matrix& lines, int sr, int sc) {
  static std::unordered_set<std::string> found;

  struct Item {
    std::pair<int, int> pos;
    std::string cword;
    Seen visited;
  };

  std::queue<Item> Q;
  for (int i{}; i < 5; ++i) {
    for (int j{}; j < 5; ++j) {
      Q.push({{i, j}, "", {}});  // pos, cword, {visited}
    }
  }

  while (!Q.empty()) {
    Item f = Q.front();
    Q.pop();

    f.cword += lines[f.pos.first][f.pos.second];
    if (!dictionary.isPrefix(f.cword)) continue;
    if (f.cword.size() >= MAXWORDSIZE) continue;
    if (f.visited.count(f.pos)) continue;
    if (dictionary.contains(f.cword) and found.count(f.cword) == 0 and
        f.cword.size() > 2) {
      std::cout << "found: " << f.cword << "\n";
      found.insert(f.cword);
    }
    // zyx
    f.visited.insert(f.pos);
    /*  visit neighbors */
    int r, c;
    std::tie(r, c) = f.pos;  // maybe doesn't workjhk
    for (int nr = r - 1; nr < r + 2; ++nr) {
      if (nr < 0 || nr >= lines.size()) continue;
      for (int nc = c - 1; nc < c + 2; ++nc) {  // gen all pairs
        if (nc < 0 || nc >= lines[0].size()) continue;
        Q.push({{nr, nc}, f.cword, f.visited});  // pos, cword, {visited}
      }
    }
  }
}

int main() {
  Matrix lines(5, "");
  std::string line;
  for (int i{}; i < 5; ++i) {
    std::cin >> line;
    lines[i] = line;
    std::cout << line << "\n";
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

  bfs(lines, 0, 0);
  // auto result = dictionary.prefix.startsWith("meaningfulness");
  // std::cout << std::boolalpha << result << "\n";
  // result = dictionary.contains("meaningfulness");
  // std::cout << std::boolalpha << result << "\n";

  return 0;
}