#include "Solver.h"

void Solver::bfs(const Matrix& lines, const Matrix& flags,
                 std::vector<Item>& results) {
  std::unordered_set<std::string> found;

  std::queue<Item> Q;
  for (int i{}; i < 5; ++i) {
    for (int j{}; j < 5; ++j) {
      Q.push({{i, j}, "", {}, 0});  // pos, cword, {visited}, value
    }
  }

  while (!Q.empty()) {
    auto f = Q.front();
    Q.pop();
    if (f.visited.count(f.pos)) continue;

    char to_add = lines[f.pos.first][f.pos.second];
    char flag = flags[f.pos.first][f.pos.second];

    if (USE_REPLACE && !f.has_replaced) {
      for (char chr = 'a'; chr <= 'z'; ++chr) {
        if (chr == to_add) continue;
        auto cf = f;
        cf.cword += chr;
        cf.replacement = {cf.pos, chr};
        cf.value -= dictionary.getCharValue(to_add) *
                    (flags[cf.pos.first][cf.pos.second] - 48);
        cf.has_replaced = true;
        Q.push(cf);
      }
    }

    if (f.pos != f.replacement.first) f.cword += to_add;  // cleaned

    if (f.cword.size() >= MAX_WORD_SIZE) continue;
    if (!dictionary.isPrefix(f.cword)) continue;
    if (flag == MULTI) {
      f.is_multi = true;
      f.value += dictionary.getCharValue(to_add);
    } else {
      f.value += dictionary.getCharValue(to_add) *
                 (flags[f.pos.first][f.pos.second] - 48);
    }
    if (dictionary.contains(f.cword) and found.count(f.cword) == 0 and
        f.cword.size() > 2) {
      f.visited.insert(f.pos);
      int old_val = f.value;
      f.value <<= f.is_multi;  // mult by 2 (fast lol) fuk u lol
      f.value += (f.cword.size() >= LONGWORD_MIN) * LONGWORD_BONUS;
      results.push_back(f);
      f.value = old_val;  // copy magic
      found.insert(f.cword);
    }

    f.visited.insert(f.pos);
    /*  visit neighbors */
    int r, c;
    r = f.pos.first;
    c = f.pos.second;
    for (int nr = r - 1; nr < r + 2; ++nr) {
      if (nr < 0 || nr >= lines.size()) continue;
      for (int nc = c - 1; nc < c + 2; ++nc) {  // gen all pairs
        if (nc < 0 || nc >= lines[0].size()) continue;
        if (nr == r && nc == c) continue;  // ?
        auto cf = f;
        cf.pos = {nr, nc};
        Q.push(cf);  // pos, cword, {visited}, value, ismulti
      }
    }
  }
}  // end Solver::bfs

void Solver::start() {
  this->running = true;
  while (this->running) {
    cli::util::busyCheckForRewrite();
    auto [lines, flags] = utils::openGiven();
    // cli::util::checkRewrite();

    if (DEBUG) cli::util::showGiven(lines, flags);

    std::vector<Item> results;
    bfs(lines, flags, results);

    utils::sortByHeuristic(results);

    auto no_replace = utils::topNWithKReplacements(results, 2, 0);
    auto one_replace = utils::topNWithKReplacements(results, 2, 1);

    for (const auto& item : one_replace) {
      cli::util::printGridWord(lines, item);
    }

    for (const auto& item : no_replace) {
      cli::util::printGridWord(lines, item);
    }

  }  // end main while
}  // end Solver::start