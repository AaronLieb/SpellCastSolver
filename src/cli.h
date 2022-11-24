#ifndef _CLI_H
#define _CLI_H

#include <iostream>
#include <string>

#include "Item.h"
#include "types.h"

namespace cli {
template <typename T>
inline void log(T msg) {
  std::cout << msg << "\n";
}

template <typename T, typename... Ts>
inline void log(T msg, Ts... rest) {
  std::cout << msg << " ";
  log(rest...);
}

inline void endPrompt() {
  log(std::string(50, '='));
  log("🟩 PRESS ENTER ONCE YOU'VE MODIFIED given.txt");
  log("🛑 Type s to STOP");
}

inline void clearConsole() {
  /* TO-DO: make this portable (-nix, win, mac) */
  system("clear");  // clear console window
}

inline bool getEndResponse() {
  std::string resp;
  std::getline(std::cin, resp);
  if (resp == "s") return false;
  return true;
}

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
  std::cout << item.value << " " << item.cword << "\n";
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

auto showGiven(auto lines, auto flags) -> void {
  cli::log("Clean Input:");
  for (string s : lines) {
    std::cout << s << "\n";
  }
  cli::log("\nFLAGS:");
  for (string s : flags) {
    cli::log(s);
  }
}

}  // namespace util

}  // namespace cli

#endif