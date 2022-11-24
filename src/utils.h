#ifndef _UTILS_H
#define _UTILS_H

#include "Item.h"
#include "types.h"

namespace util {
namespace style {
const std::string green = "\u001b[32m";  // green
const std::string bold = "\u001b[1m";
const std::string black = "\u001b[30m";
const std::string white_bg = "\u001b[47m";
const std::string black_bg = "\u001b[40;1m";
const std::string reset = "\u001b[0m";
const std::string red = "\u001b[31m";
}  // namespace style

void static printGridWord(const Matrix& lines, const Item& item) {
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
}  // namespace util

#endif