#ifndef _SOLVER_H
#define _SOLVER_H

#include <bits/stdc++.h>

#include "Dictionary.h"
#include "Item.h"
#include "types.h"
#include "utils.h"

#define MAXWORDSIZE 15
#define REPLACE_COST 10
#define LONGWORD_BONUS 20
#define LONGWORD_MIN 7
#define USEREPLACE true
#define DOUBLE '2'
#define TRIPLE '3'
#define MULTI 'X'
#define DEBUG true

class Solver {
 private:
 public:
  Solver() = default;
  void bfs(const Matrix& lines, const Matrix& flags,
           std::vector<Item>& results);
  void start();
};

#endif