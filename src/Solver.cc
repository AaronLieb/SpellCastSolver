#include "Solver.h"

void Solver::bfs(const Matrix& cells, Results& results) {
  std::queue<Item> Q;
  for (smallint i{}; i < 5; ++i) {
    for (smallint j{}; j < 5; ++j) {
      Q.push(Item({i, j}));
    }
  }

  while (!Q.empty()) {
    auto f = Q.front();
    Q.pop();
    if (f.seen()) continue;

    char to_add = cells[f.pos.first][f.pos.second].letter;
    char flag = cells[f.pos.first][f.pos.second].flag;
    bool gem = cells[f.pos.first][f.pos.second].is_gem;
    f.is_multi = (flag == MULTI) || f.is_multi;

    if (f.replace_count < REPLACE_LIMIT &&
        f.value > (f.replace_count * REPLACE_THRESHHOLD)) {
      for (char chr = 'a'; chr <= 'z'; ++chr) {
        if (chr == to_add) continue;
        if (!dictionary.hasChild(f.cword, chr)) continue;
        if (!dictionary.isPrefix(f.cword + chr)) continue;
        if (dictionary.valueHeuristic(f.cword + chr, REPLACE_HEURISTIC_DEPTH) <
            REPLACE_HEURISTIC_THRESHOLD)
          continue;
        auto cf = f;
        cf.cword += chr;
        cf.replacements[cf.pos] = chr;
        cf.replace_count += 1;
        Q.push(cf);
      }
    }

    if (!f.replace_count ||
        (f.replacements.find(f.pos) == f.replacements.end()))
      f.cword += to_add;
    if (f.cword.size() >= MAX_WORD_SIZE) continue;
    if (!dictionary.isPrefix(f.cword)) continue;

    f.value += dictionary.getCharValue(f.cword.back(), flag);
    f.gems += gem;

    f.visit();
    if (dictionary.contains(f.cword) && f.cword.size() >= MIN_WORD_SIZE) {
      f.store(results);
    }

    auto [r, c] = f.pos;
    for (int nr = r - 1; nr < r + 2; ++nr) {
      if (nr < 0 || nr >= cells.size()) continue;
      for (int nc = c - 1; nc < c + 2; ++nc) {  // gen all pairs
        if (nc < 0 || nc >= cells[0].size()) continue;
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
    auto cells = utils::openGiven();

    if (DEBUG) cli::util::showGiven(cells);

    Results results;
    bfs(cells, results);

    utils::showTopNWithKReplacements(cells, results, 1, 3);
    utils::showTopNWithKReplacements(cells, results, 1, 2);
    utils::showTopNWithKReplacements(cells, results, 1, 1);
    utils::showTopNWithKReplacements(cells, results, 1, 0);
  }  // end main while
}  // end Solver::start
