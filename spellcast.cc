#include <bits/stdc++.h>

#include "Dictionary.cc"

#define MAXWORDSIZE 14
namespace rules {
const int DOUBLE = 0x2;
const int TRIPLE = 0x4;
const int MULTI = 0x6;
}  // namespace rules
namespace style {
const std::string color = "\u001b[32m";  // TBD
const std::string bold = "\u001b[1m";
const std::string black = "\u001b[30m";
const std::string white_bg = "\u001b[47m";
const std::string black_bg = "\u001b[40;1m";
const std::string reset = "\u001b[0m";
}  // namespace style

using Matrix = std::vector<std::string>;
using Seen = std::set<std::pair<int, int>>;

struct Item {
  std::pair<int, int> pos;
  std::string cword;
  Seen visited;
  int value;
  bool is_multi = false;
};

void printGridWord(const Matrix& lines, const Item& item) {
  for (int r{}; r < lines.size(); ++r) {
    for (int c{}; c < lines[r].size(); ++c) {
      bool is_in_word = item.visited.count({r, c});
      char chr = lines[r][c];
      std::cout << style::black_bg;
      std::cout << (is_in_word ? style::color + style::bold : style::black)
                << chr << " " << style::reset;
    }
    std::cout << "\n";
  }
}

void bfs(const Matrix& lines, const Matrix& flags, int sr, int sc,
         std::vector<Item>& results) {
  static std::unordered_set<std::string> found;

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
      f.visited.insert(f.pos);
      results.push_back(f);
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

  std::vector<Item> results;
  bfs(lines, flags, 0, 0, results);

  std::sort(begin(results), end(results),
            [](auto a, auto b) { return a.value > b.value; });

  int i = 3;
  for (Item& item : results) {
    std::cout << item.value << " " << item.cword << "\n";
    printGridWord(lines, item);
    if (!i--) break;
    std::cout << "\n";
  }

  return 0;
}