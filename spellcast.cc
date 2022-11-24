#include <bits/stdc++.h>

#include "Dictionary.cc"

#define MAXWORDSIZE 15
#define REPLACE_COST 10
#define LONGWORD_BONUS 20
#define LONGWORD_MIN 8
#define USEREPLACE true
#define DOUBLE '2'
#define TRIPLE '3'
#define MULTI 'X'
#define DEBUG false

namespace style {
const std::string green = "\u001b[32m";  // green
const std::string bold = "\u001b[1m";
const std::string black = "\u001b[30m";
const std::string white_bg = "\u001b[47m";
const std::string black_bg = "\u001b[40;1m";
const std::string reset = "\u001b[0m";
const std::string red = "\u001b[31m";
}  // namespace style

using Matrix = std::vector<std::string>;
using Seen = std::set<std::pair<int, int>>;
using Replacement = std::pair<std::pair<int, int>, char>;

struct Item {
  std::pair<int, int> pos;
  std::string cword;
  Seen visited;
  int value;
  bool is_multi;
  bool has_replaced;
  Replacement replacement;
};

void printGridWord(const Matrix& lines, const Item& item) {
  for (int r{}; r < lines.size(); ++r) {
    for (int c{}; c < lines[r].size(); ++c) {
      bool is_in_word = item.visited.count({r, c});
      char chr = lines[r][c];
      std::string color = style::green;
      Replacement rep = item.replacement;

      if (item.has_replaced &&
          (rep.first.first == r && rep.first.second == c)) {
        color = style::red;
        chr = rep.second;
      }

      std::cout << style::black_bg;
      std::cout << (is_in_word ? color + style::bold : style::black) << chr
                << " " << style::reset;
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
    if (f.visited.count(f.pos)) continue;

    char to_add = lines[f.pos.first][f.pos.second];
    char flag = flags[f.pos.first][f.pos.second];

    if (USEREPLACE && !f.has_replaced) {
      for (char chr = 'a'; chr <= 'z'; ++chr) {
        if (chr == to_add) continue;
        Item cf = f;
        cf.cword += chr;
        cf.replacement = {cf.pos, chr};
        cf.value -= dictionary.getCharValue(to_add) *
                    (flags[cf.pos.first][cf.pos.second] - 48);
        cf.has_replaced = true;
        Q.push(cf);
      }
    }

    if (f.pos != f.replacement.first) f.cword += to_add;  // cleaned

    if (f.cword.size() >= MAXWORDSIZE) continue;
    if (!dictionary.isPrefix(f.cword)) continue;
    if (flag == 'X') {
      f.is_multi = true;
      f.value += dictionary.getCharValue(to_add);
    } else {
      f.value += dictionary.getCharValue(to_add) *
                 (flags[f.pos.first][f.pos.second] - 48);
    }
    if (dictionary.contains(f.cword) and found.count(f.cword) == 0 and
        f.cword.size() > 2) {
      f.visited.insert(f.pos);
      int old_val = f.value;
      f.value <<= f.is_multi;  // mult by 2 (fast lol) fuk u lol
      f.value += (f.cword.size() >= LONGWORD_MIN) * LONGWORD_BONUS;
      results.push_back(f);
      f.value = old_val;  // copy magic
      found.insert(f.cword);
    }

    f.visited.insert(f.pos);
    /*  visit neighbors */
    int r, c;
    r = f.pos.first;
    c = f.pos.second;
    for (int nr = r - 1; nr < r + 2; ++nr) {
      if (nr < 0 || nr >= lines.size()) continue;
      for (int nc = c - 1; nc < c + 2; ++nc) {  // gen all pairs
        if (nc < 0 || nc >= lines[0].size()) continue;
        if (nr == r && nc == c) continue;  // ?
        Item cf = f;
        cf.pos = {nr, nc};
        Q.push(cf);  // pos, cword, {visited}, value, ismulti
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
      if (line[x] == DOUBLE or line[x] == TRIPLE or line[x] == MULTI) {
        flags[lines.size()][x - offset] = line[x];
        offset = 1;
        continue;
      }
      // 2abyXpt
      clean += line[x];
    }
    lines.push_back(clean);
  }
  if (DEBUG) {
    for (string s : lines) {
      std::cout << s << "\n";
    }
    std::cout << "\nFLAGS\n";
    for (string s : flags) {
      std::cout << s << "\n";
    }
  }

  std::vector<Item> results;
  bfs(lines, flags, 0, 0, results);

  std::sort(begin(results), end(results), [](auto a, auto b) {
    a.value -= a.has_replaced ? REPLACE_COST : 0;
    b.value -= b.has_replaced ? REPLACE_COST : 0;
    return a.value > b.value;
  });

  int i = 5, max_depth = 100;
  bool used_non_replacement = false;
  for (Item& item : results) {
    if (item.has_replaced == false) used_non_replacement = true;
    if (i == 1 && !used_non_replacement) {
      if (!--max_depth) break;
      continue;
    }
    std::cout << item.value << " " << item.cword << "\n";
    printGridWord(lines, item);
    if (!--i) {
      break;
    }
    std::cout << "\n";
  }

  return 0;
}
