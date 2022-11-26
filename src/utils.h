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

  if (DEBUG) cli::log("🔧 ENV::DEBUG ENABLED 🔧");
  cli::log("🧠 CALCULATING 🧠");

  while (fin >> line) {
    std::string clean = "";
    int flags_seen = 0;
    for (int x{}; x < line.size(); ++x) {
      if (line[x] == DOUBLE or line[x] == TRIPLE or line[x] == MULTI) {
        flags[lines.size()][x - flags_seen] = line[x];
        flags_seen += 1;
        continue;
      }
      clean += line[x];
    }
    lines.push_back(clean);
  }
  if (lines.size() != 5) {
    cli::log("⚠⚠⚠ YOU ONLY ENTERED", lines.size(), "LINES... NEED 5 ⚠⚠⚠");
    throw(std::out_of_range("bad input"));
  }
  return {lines, flags};
}

auto showTopNWithKReplacements(const Matrix& lines, const Matrix& flags,
                               auto& results, int n, int k) -> void {
  for (auto&& result : results[k]) {
    cli::util::printGridWord(lines, flags, result);
    if (!--n) break;
  }
}

}  // namespace utils
#endif