#include "Solver.h"

void Solver::bfs(const Matrix& lines, const Matrix& flags, Results& results) {
  std::queue<Item> Q;
  // for (int i{}; i < 5; ++i) {
  //   for (int j{}; j < 5; ++j) {
  //     Q.push(Item({i, j}));
  //   }
  // }

  Q.push(Item({1, 1}));
  Q.push(Item({0, 0}));

  while (!Q.empty()) {
    auto f = Q.front();
    Q.pop();
    if (f.seen()) continue;

    char to_add = lines[f.pos.first][f.pos.second];
    char flag = flags[f.pos.first][f.pos.second];

    if (USE_REPLACE && f.replace_count < REPLACE_LIMIT) {
      for (char chr = 'a'; chr <= 'z'; ++chr) {
        if (chr == to_add) continue;
        auto cf = f;
        cf.cword += chr;
        cf.replacement = {cf.pos, chr};
        cf.value -= dictionary.getCharValue(to_add, flag);
        cf.replace_count += 1;
        Q.push(cf);
      }
    }
    if (!f.replace_count || !(f.pos == f.replacement.first))
      f.cword += to_add;  // figure this out... why need to check replace stuff?

    if (f.cword.size() >= MAX_WORD_SIZE) continue;
    if (!dictionary.isPrefix(f.cword)) continue;
    f.is_multi = (flag == MULTI);

    f.value += dictionary.getCharValue(to_add, flag);
    f.visit();
    if (dictionary.contains(f.cword) && f.cword.size() >= MIN_WORD_SIZE) {
      f.store(results);
    }

    auto [r, c] = f.pos;
    for (int nr = r - 1; nr < r + 2; ++nr) {
      if (nr < 0 || nr >= lines.size()) continue;

      for (int nc = c - 1; nc < c + 2; ++nc) {  // gen all pairs
        if (nc < 0 || nc >= lines[0].size()) continue;
        if (nr == r && nc == c) continue;

        auto cf = f;
        cf.pos = {nr, nc};
        Q.push(cf);
      }
    }
  }
}  // end Solver::bfs

void Solver::start() {
  this->running = true;
  while (this->running) {
    cli::util::busyCheckForRewrite();
    auto [lines, flags] = utils::openGiven();

    if (DEBUG) cli::util::showGiven(lines, flags);

    // std::vector<Item> results;
    Results results;
    // std::set<Item, utils::itemSortHeuristic> results;
    bfs(lines, flags, results);

    int i{2};
    for (const auto& item : results[0]) {
      cli::util::printGridWord(lines, item);
      if (!--i) break;
    }

    i = 2;
    for (const auto& item : results[1]) {
      cli::util::printGridWord(lines, item);
      if (!--i) break;
    }

    // utils::sortByHeuristic(results);

    // auto no_replace = utils::topNWithKReplacements(results, 2, 0);
    // auto one_replace = utils::topNWithKReplacements(results, 2, 1);

    // for (const auto& item : one_replace) {
    //   cli::util::printGridWord(lines, item);
    // }

    // for (const auto& item : no_replace) {
    //   cli::util::printGridWord(lines, item);
    // }
  }  // end main while
}  // end Solver::start