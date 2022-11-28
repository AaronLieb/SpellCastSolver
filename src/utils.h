#ifndef _UTILS_H
#define _UTILS_H

#include <fstream>

#include "Item.h"
#include "cli.h"
#include "err.h"
#include "types.h"

namespace utils {

static Matrix openGiven() {
  std::ifstream fin;
  try {
    fin.open("given.txt");
  } catch (const std::ifstream::failure& exception) {
    cli::log("Could not open file.");
    return {};
  }
  cli::log("💾 Opened File ✅");

  Matrix cells(BOARD_SIZE, std::vector<Cell>{BOARD_SIZE, Cell{}});
  std::string line;

  if (DEBUG) cli::log("🔧 ENV::DEBUG ENABLED 🔧");
  cli::log("🧠 CALCULATING 🧠");

  int num_lines = 0;
  while (fin >> line) {
    if (num_lines >= BOARD_SIZE) {
      error::promptBadInputSize(num_lines + 1);
      return openGiven();
    }
    std::string clean = "";
    int flags_seen = 0;
    for (int x{}; x < line.size(); ++x) {
      char& chr = line[x];
      if (chr == DOUBLE or chr == TRIPLE or chr == MULTI) {
        cells[num_lines][x - flags_seen].flag = chr;
        flags_seen += 1;
        continue;
      }

      if (isupper(chr)) {
        cells[num_lines][x - flags_seen].is_gem = true;
        chr = tolower(chr);
      }

      if (chr < 'a' || chr > 'z') {
        error::promptBadInput(chr);
        return openGiven();  // retry!
      }

      clean += chr;
      cells[num_lines][x - flags_seen].letter = chr;
    }
    num_lines++;
  }

  if (num_lines != 5) {
    error::promptBadInputSize(num_lines);
    return openGiven();
  }

  return cells;
}

auto showTopNWithKReplacements(const Matrix& cells, auto& results, int n, int k) -> void {
  for (auto&& result : results[k]) {
    cli::util::printGridWord(cells, result);
    if (!--n) break;
  }
}

}  // namespace utils
#endif
