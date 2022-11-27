#ifndef _ITEM_H
#define _ITEM_H

#include "types.h"

struct Item {
  std::pair<int, int> pos;
  std::string cword;
  Seen visited;
  int value;
  bool is_multi;
  int replace_count;
  Replacement replacement;

  Item(std::pair<int, int> _pos)
      : pos(_pos),
        cword(),
        visited{},
        value(0),
        is_multi(false),
        replace_count{0},
        replacement{} {}

  void visit() { visited.insert(pos); }

  void store(Results& results) {
    int old_value = value;
    value <<= is_multi;  // mult by 2 (fast lol) fuk u lol
    value += (cword.size() >= LONGWORD_MIN) * LONGWORD_BONUS;
    /* have we made this word before */
    auto& bucket = results[replace_count];
    bucket.insert(*this);
    value = old_value;
  }

  bool seen() const { return visited.count(pos); }
};

static std::ostream& operator<<(std::ostream& os, std::pair<int, int> p) {
  os << "(" << p.first << ", " << p.second << ")";
  return os;
}

static std::ostream& operator<<(std::ostream& os, const Item& item) {
  os << "{"
     << "cword: " << item.cword << ", pos: "
     << "(" << item.pos.first << ", " << item.pos.second << ")"
     << ", is_multi: " << item.is_multi
     << ", replace_count: " << item.replace_count << ", value: " << item.value;
  os << ", visited: [";
  for (const auto& p : item.visited) {
    os << "(" << p.first << ", " << p.second << "), ";
  }
  os << "]";
  os << "}";
  return os;
}

static bool operator<(const Item& a, const Item& b) {
  return (a.value - a.replace_count) < (b.value - b.replace_count);
}

static bool operator>(const Item& a, const Item& b) {
  return (a.value - a.replace_count) > (b.value - b.replace_count);
}

static bool operator==(const Item& lhs, const Item& rhs) {
  return (lhs.cword == rhs.cword) && (lhs.value == rhs.value) &&
         (lhs.replace_count == rhs.replace_count);
}

#endif