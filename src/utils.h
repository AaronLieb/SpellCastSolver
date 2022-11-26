#ifndef _UTILS_H
#define _UTILS_H

#include <fstream>

#include "Item.h"
#include "cli.h"
#include "err.h"
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
    if (int num_lines = lines.size(); num_lines >= 5) {
      error::promptBadInputSize(num_lines + 1);
      return openGiven();
    }
    std::string clean = "";
    int flags_seen = 0;
    for (int x{}; x < line.size(); ++x) {
      if (line[x] == DOUBLE or line[x] == TRIPLE or line[x] == MULTI) {
        flags[lines.size()][x - flags_seen] = line[x];
        flags_seen += 1;
        continue;
      }

      /* TODO... support uppercase for gems */
      if (line[x] < 'a' || line[x] > 'z') {
        error::promptBadInput(line[x]);
        return openGiven();  // retry!
      }

      clean += line[x];
    }
    lines.push_back(clean);
  }

  if (int num_lines = lines.size(); num_lines != 5) {
    error::promptBadInputSize(num_lines);
    return openGiven();
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