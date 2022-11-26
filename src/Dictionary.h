#ifndef _DICTIONARY_H
#define _DICTIONARY_H

#include <bits/stdc++.h>

#include "PrefixTree.h"
#include "env.h"
#include "types.h"

class Dictionary {
  std::unordered_set<std::string> dict;
  int values[26] = {1, 4, 5, 3, 1, 5, 3, 4, 1, 7, 6, 3, 4,
                    2, 1, 4, 8, 2, 2, 2, 4, 5, 5, 7, 4, 8};

 public:
  PrefixTree prefix_tree;
  Dictionary() = delete;

  Dictionary(std::string fname) {
    std::ifstream fp(fname);
    std::string word;
    while (fp >> word) {
      dict.insert(word);
      prefix_tree.insert(word);
    }
  }

  bool contains(const std::string& word) const { return dict.count(word) != 0; }
  bool contains(std::string&& word) const { return dict.count(word); }
  bool isPrefix(const std::string& prefix) const {
    return prefix_tree.startsWith(prefix);
  }

  int getCharValue(char c, char flag) const {
    if (flag == MULTI) return this->values[c - 'a'];
    return this->values[c - 'a'] * (flag - '0');
  }
};

#endif