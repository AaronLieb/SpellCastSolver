#include <bits/stdc++.h>

#include "Dictionary.cc"

#define MAXWORDSIZE 8

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

void bfs(const Matrix& lines, int r, int c) {
  std::queue<char> Q;
  Seen visited;
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
  for (int r{}; r < 5; ++r) {
    for (int c{}; c < 5; ++c) {
      std::set<std::pair<int, int>> visited;  // construct new visi per trav.
      dfs(lines, r, c, visited);
    }
  }

  // auto result = dictionary.contains("applx");
  // std::cout << std::boolalpha << result << "\n";

  return 0;
}