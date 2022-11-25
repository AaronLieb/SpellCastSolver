#ifndef _UTILS_H
#define _UTILS_H

#include <fstream>

#include "Item.h"
#include "cli.h"
#include "types.h"

namespace utils {

struct Parsed {
  Matrix lines, flags;
};

static Parsed openGiven() {
  Parsed parsed;
  std::ifstream fin;
  try {
    fin.open("given.txt");
  } catch (const std::ifstream::failure& exception) {
    cli::log("Could not open file.");
    return parsed;
  }
  cli::log("💾 Opened File ✅");

  Matrix lines;
  Matrix flags(5, "11111");
  std::string line;
  cli::log("🧠 CALCULATING 🧠");

  while (fin >> line) {
    std::string clean = "";
    bool offset = 0;
    for (int x{}; x < line.size(); ++x) {
      if (line[x] == DOUBLE or line[x] == TRIPLE or line[x] == MULTI) {
        flags[lines.size()][x - offset] = line[x];
        offset = 1;
        continue;
      }
      clean += line[x];
    }
    lines.push_back(clean);
  }

  return {lines, flags};
}

auto sortByHeuristic(auto& to_sort) {
  std::sort(begin(to_sort), end(to_sort), [](auto a, auto b) {
    a.value -= a.has_replaced ? REPLACE_COST : 0;
    b.value -= b.has_replaced ? REPLACE_COST : 0;
    return a.value > b.value;
  });
}

auto topNWithKReplacements(auto& results, int n, int k) -> auto{
  int max_depth = 2048;
  typename std::decay<decltype(results)>::type topn;
  for (const Item& result : results) {
    /* TO-DO: change has_replaced to replace_count */
    if (result.has_replaced == k) {
      topn.push_back(result);
      if (!--n) break;
    }
    if (!--max_depth) break;  // fail-safe if no words found with k replaces
  }
  return topn;
}

}  // namespace utils
#endif