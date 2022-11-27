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
  int getValue(const std::string& word) const {
    int value{};
    for (char c : word) value += values[c - 'a'];
    return value;
  }
  int heightHeuristic(const std::string& prefix) const {
    return prefix_tree.getHeightOfPrefix(prefix);
  }

  int valueHeuristic(const std::string& prefix, int n) const {
    // float value = static_cast<float>(this->getValue(prefix));
    int value = this->getValue(prefix);
    TrieNode* find = prefix_tree.search(prefix);
    if (find == nullptr) return -1;  // not a prefix

    std::queue<TrieNode*> Q;
    Q.push(find);
    while (!Q.empty() && n--) {
      TrieNode* front = Q.front();
      Q.pop();
      for (char c = 'a'; c <= 'z'; ++c) {
        TrieNode* child = front->children[c - 'a'];
        if (child == NULL) continue;
        value += values[c - 'a'];
        Q.push(child);
      }
    }

    return value;
  }

  int getCharValue(char c, char flag) const {
    if (flag == MULTI) return this->values[c - 'a'];
    if (flag != DOUBLE && flag != TRIPLE && flag != '1') {
      throw(std::domain_error("⚠⚠⚠ bad flag... recheck given.txt⚠⚠⚠⚠\n"));
    }
    return this->values[c - 'a'] * (flag - '0');
  }
};

#endif