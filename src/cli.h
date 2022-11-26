#ifndef _CLI_H
#define _CLI_H

/*
  evil macro
  source:
  stackoverflow.com/questions/32226300/make-variadic-macro-method-which-prints-all-variables-names-and-values
*/
#define debug(...)                                         \
  do {                                                     \
    if (DEBUG) show(std::cout, #__VA_ARGS__, __VA_ARGS__); \
  } while (0);

template <typename H1>
std::ostream& show(std::ostream& out, const char* label, H1&& value) {
  return out << label << "=" << std::forward<H1>(value) << '\n';
}

template <typename H1, typename... T>
std::ostream& show(std::ostream& out, const char* label, H1&& value,
                   T&&... rest) {
  const char* pcomma = strchr(label, ',');
  return show(out.write(label, pcomma - label)
                  << "=" << std::forward<H1>(value) << ',',
              pcomma + 1, std::forward<T>(rest)...);
}

#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>

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

inline void clearConsole() {
  /* TO-DO: make this portable (-nix, win, mac) */
  system("clear");  // clear console window
}

namespace util {
namespace style {
const std::string green = "\u001b[32m";  // green
const std::string bold = "\u001b[1m";
const std::string black = "\u001b[30m";
const std::string white_bg = "\u001b[47m";
const std::string yellow_bg = "\u001b[43m";
const std::string magenta_bg = "\u001b[45m";
const std::string gray_bg = "\u001b[100m";
const std::string black_bg = "\u001b[40;1m";
const std::string reset = "\u001b[0m";
const std::string red = "\u001b[31m";
}  // namespace style

static inline auto printGridWord(const Matrix& lines, const Matrix& flags,
                                 const auto& item) -> void {
  if (DEBUG)
    log("item", item);
  else
    log(item.value, item.cword);

  for (int r{}; r < lines.size(); ++r) {
    for (int c{}; c < lines[r].size(); ++c) {
      bool is_in_word = item.visited.count({r, c});
      char chr = lines[r][c];
      std::string color = style::green;
      std::string bg_color = style::black_bg;
      Replacement rep = item.replacement;

      if (item.replace_count &&
          (rep.first.first == r && rep.first.second == c)) {
        color = style::red;
        chr = rep.second;
      }

      if (flags[r][c] == MULTI)
        bg_color = style::yellow_bg;
      else if (flags[r][c] == DOUBLE)
        bg_color = style::gray_bg;
      else if (flags[r][c] == TRIPLE)
        bg_color = style::white_bg;

      std::cout << (DEBUG ? bg_color : style::black_bg);
      std::cout << (is_in_word ? color + style::bold : style::black) << chr
                << " " << style::reset;
    }
    std::cout << "\n";
  }
}

static inline auto showGiven(auto lines, auto flags) -> void {
  cli::log("Clean Input:");
  Item dummy({-1, -1});
  printGridWord(lines, flags, dummy);
}

template <typename TP>
static std::time_t to_time_t(TP tp) {
  using namespace std::chrono;
  auto sctp = time_point_cast<std::chrono::system_clock::duration>(
      tp - TP::clock::now() + std::chrono::system_clock::now());
  return std::chrono::system_clock::to_time_t(sctp);
}

inline auto getLastFileWriteTime() -> std::time_t {
  std::filesystem::path fpath = "given.txt";
  auto ftime = std::filesystem::last_write_time(fpath);
  std::time_t time = to_time_t(ftime);
  return time;
}

static auto checkRewrite() -> bool {
  static std::time_t last_write{-1};
  std::time_t curr_time = getLastFileWriteTime();
  if (last_write == curr_time) return false;
  // cli::log("🚫 File Not Changed From Previous Run 🚫");
  last_write = curr_time;
  return true;
}

inline auto sleep(unsigned ms) -> void {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

static auto busyCheckForRewrite() -> void {
  // static std::vector<std::string> anim{"--", "\\", "|", "/"};
  static std::string anim{"📚📕📗📘📖"};
  int i{};
  std::cout << "\n📝 Waiting For Change to given.txt ...  ";
  while (!checkRewrite()) {
    std::cout << anim[i++ % anim.size()] << std::flush;
    sleep(250);
    if (i % 20 == 0 and i)
      std::cout << std::string(10, '\b') << std::string(10, ' ')
                << std::string(10, '\b') << std::flush;
    // std::cout << "\b";
  }
  clearConsole();
}

}  // namespace util

}  // namespace cli

#endif