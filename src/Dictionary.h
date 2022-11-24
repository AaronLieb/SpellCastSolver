#ifndef _DICTIONARY_H
#define _DICTIONARY_H

#include <bits/stdc++.h>

#include "PrefixTree.h"

using Seen = std::set<std::pair<int, int>>;
using Matrix = std::vector<std::string>;

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

  bool contains(const std::string& word) { return dict.count(word) != 0; }
  bool contains(std::string&& word) { return dict.count(word); }
  bool isPrefix(const std::string& prefix) {
    return prefix_tree.startsWith(prefix);
  }

  int getCharValue(char c) { return this->values[c - 'a']; }
};

static Dictionary dictionary("dictionary.txt");

#endif