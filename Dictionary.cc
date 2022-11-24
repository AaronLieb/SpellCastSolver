#include <bits/stdc++.h>

#include "PrefixTree.cc"

class Dictionary {
  std::unordered_set<std::string> dict;

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
};

static Dictionary dictionary("dictionary.txt");