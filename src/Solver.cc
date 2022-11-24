#include "Solver.h"

void Solver::bfs(const Matrix& lines, const Matrix& flags,
                 std::vector<Item>& results) {
  static std::unordered_set<std::string> found;

  std::queue<Item> Q;
  for (int i{}; i < 5; ++i) {
    for (int j{}; j < 5; ++j) {
      Q.push({{i, j}, "", {}, 0});  // pos, cword, {visited}, value
    }
  }

  while (!Q.empty()) {
    Item f = Q.front();
    Q.pop();
    if (f.visited.count(f.pos)) continue;

    char to_add = lines[f.pos.first][f.pos.second];
    char flag = flags[f.pos.first][f.pos.second];

    if (USEREPLACE && !f.has_replaced) {
      for (char chr = 'a'; chr <= 'z'; ++chr) {
        if (chr == to_add) continue;
        Item cf = f;
        cf.cword += chr;
        cf.replacement = {cf.pos, chr};
        cf.value -= dictionary.getCharValue(to_add) *
                    (flags[cf.pos.first][cf.pos.second] - 48);
        cf.has_replaced = true;
        Q.push(cf);
      }
    }

    if (f.pos != f.replacement.first) f.cword += to_add;  // cleaned

    if (f.cword.size() >= MAXWORDSIZE) continue;
    if (!dictionary.isPrefix(f.cword)) continue;
    if (flag == 'X') {
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
        Item cf = f;
        cf.pos = {nr, nc};
        Q.push(cf);  // pos, cword, {visited}, value, ismulti
      }
    }
  }
}  // end Solver::bfs

void Solver::start() {
  this->running = true;
  while (this->running) {
    auto [lines, flags] = utils::openGiven();

    if (DEBUG) cli::util::showGiven(lines, flags);

    std::vector<Item> results;
    bfs(lines, flags, results);

    utils::sortByHeuristic(results);

    auto no_replace = utils::topNWithKReplacements(results, 3, 0);
    auto one_replace = utils::topNWithKReplacements(results, 3, 1);

    for (const Item& item : one_replace) {
      cli::util::printGridWord(lines, item);
    }

    for (const Item& item : no_replace) {
      cli::util::printGridWord(lines, item);
    }
    // int i = 5, max_depth = 100;
    // bool used_non_replacement = false;
    // for (Item& item : results) {
    //   if (item.has_replaced == false) used_non_replacement = true;
    //   if (i == 1 && !used_non_replacement) {
    //     if (!--max_depth) break;
    //     continue;
    //   }
    //   std::cout << item.value << " " << item.cword << "\n";
    //   cli::util::printGridWord(lines, item);
    //   if (!--i) {
    //     break;
    //   }
    //   std::cout << "\n";
    // }

    cli::endPrompt();
    this->running = cli::getEndResponse();  // type 's' to stop!
    cli::clearConsole();

  }  // end main while
}  // end Solver::start