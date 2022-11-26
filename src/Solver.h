#ifndef _SOLVER_H
#define _SOLVER_H

#include <bits/stdc++.h>

#include "Dictionary.h"
#include "Item.h"
#include "cli.h"
#include "env.h"
#include "types.h"
#include "utils.h"

class Solver {
 private:
  bool running;
  Dictionary dictionary;

 public:
  Solver() : running(false), dictionary("dictionary.txt") {}

  void bfs(const Matrix& lines, const Matrix& flags, std::vector<Item>& results);
  void start();
};

#endif