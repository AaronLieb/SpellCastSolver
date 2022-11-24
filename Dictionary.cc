#include <bits/stdc++.h>

class Dictionary {
  std::unordered_set<std::string> dict;

 public:
  Dictionary() = delete;

  Dictionary(std::string fname) {
    std::ifstream fp(fname);
    std::string word;
    while (fp >> word) {
      dict.insert(word);
    }
  }

  bool contains(const std::string& word) { return dict.count(word) != 0; }
  bool contains(std::string&& word) { return dict.count(word); }
};

static Dictionary dictionary("dictionary.txt");