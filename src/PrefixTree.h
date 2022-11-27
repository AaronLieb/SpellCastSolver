#ifndef _PREFIXTREE_H
#define _PREFIXTREE_H

#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
  TrieNode* children[26];
  bool isWord;
  int height;
  TrieNode() {
    isWord = false;
    for (int i = 0; i < 26; i++) {
      children[i] = nullptr;
    }
  }

  // destructor
  ~TrieNode() {
    for (int i = 0; i < 26; i++) {
      if (children[i]) {
        delete children[i];
      }
    }
  }
};

class PrefixTree {  // thanks robot overlords
  TrieNode* root;

 public:
  PrefixTree() { root = new TrieNode(); }

  void insert(string word) {
    TrieNode* node = root;
    int i{};
    for (char c : word) {
      ++i;
      if (node->children[c - 'a'] == NULL) {
        node->children[c - 'a'] = new TrieNode();
      }
      node->height = std::max(node->height, static_cast<int>(word.size() - i));
      node = node->children[c - 'a'];
    }
    node->isWord = true;
  }

  TrieNode* search(string word) const {
    TrieNode* node = root;
    for (char c : word) {
      if (node->children[c - 'a'] == NULL) {
        return nullptr;
      }
      node = node->children[c - 'a'];
    }
    return node;
  }

  bool startsWith(string prefix) const {
    TrieNode* node = root;
    for (char c : prefix) {
      if (node->children[c - 'a'] == NULL) {
        return false;
      }
      node = node->children[c - 'a'];
    }
    return true;
  }

  int getHeightOfPrefix(string prefix) const {
    TrieNode* find = this->search(prefix);
    if (find == nullptr) return -1;  // probably should throw here?
    return find->height;
  }
};

#endif