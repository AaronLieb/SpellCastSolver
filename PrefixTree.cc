#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
  TrieNode* children[26];
  bool isWord;
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
    for (char c : word) {
      if (node->children[c - 'a'] == NULL) {
        node->children[c - 'a'] = new TrieNode();
      }
      node = node->children[c - 'a'];
    }
    node->isWord = true;
  }

  TrieNode* search(string word) {
    TrieNode* node = root;
    for (char c : word) {
      if (node->children[c - 'a'] == NULL) {
        return nullptr;
      }
      node = node->children[c - 'a'];
    }
    return node;
  }

  int children(string word) {
    TrieNode* f = search(word);
    int kids = 0;
    for (int i{}; i < 26; ++i) {
      if (f->children[i] != nullptr) kids += 1 << i;
    }
    return kids;
  }

  bool startsWith(string prefix) {
    TrieNode* node = root;
    for (char c : prefix) {
      if (node->children[c - 'a'] == NULL) {
        return false;
      }
      node = node->children[c - 'a'];
    }
    return true;
  }
};
