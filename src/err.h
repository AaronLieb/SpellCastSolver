#ifndef _ERR_H
#define _ERR_H

#include <iostream>

#include "cli.h"

namespace error {
static inline void promptBadInput(char bad) {
  try {
    throw(std::domain_error("⚠⚠⚠BAD INPUT... Check given.txt⚠⚠⚠\n"));
  } catch (const std::domain_error& err) {
    std::cerr << "Error occured: " << err.what() << std::endl;
    std::cerr << "ℹ This character caused the problem: \"" << bad << "\""
              << std::endl;
    cli::log("Go fix it and I'll automatically update!\n");
    cli::util::busyCheckForRewrite();
    cli::clearConsole();
  }
}

static inline void promptBadInputSize(int number_of_lines_entered) {
  try {
    throw(std::length_error("too many lines"));
  } catch (const std::length_error& err) {
    cli::log("⚠⚠⚠ YOU ENTERED", number_of_lines_entered,
             "LINES... MUST NEED 5 ⚠⚠⚠\n");
    cli::log("Go fix it and I'll automatically update!\n");
    cli::util::busyCheckForRewrite();
    cli::clearConsole();
  }
}

}  // namespace error

#endif