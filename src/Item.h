#ifndef _ITEM_H
#define _ITEM_H

#include "types.h"

struct Item {
  std::pair<smallint, smallint> pos;
  std::string cword;
  Seen visited;
  bool is_multi;
  int value;
  smallint replace_count, gems;
  Replacements replacements;

  Item(std::pair<smallint, smallint> _pos)
      : pos{_pos},
        cword{},
        visited{},
        is_multi{false},
        value{0},
        replace_count{0},
        gems{0},
        replacements{} {}

  void visit() { visited.insert(pos); }

  void store(Results& results) {
    auto old_value = value;
    value <<= is_multi;  // mult by 2 (fast lol) fuk u lol
    value += (cword.size() >= LONGWORD_MIN) * LONGWORD_BONUS;
    /* have we made this word before */
    auto& bucket = results[replace_count];
    bucket.insert(*this);
    value = old_value;
  }

  int getInferredValue() const {
    return value + (gems * GEM_VALUE) - (replace_count * REPLACE_COST);
  }

  bool seen() const { return visited.count(pos); }
};

static std::ostream& operator<<(std::ostream& os,
                                std::pair<smallint, smallint> p) {
  os << "(" << p.first << ", " << p.second << ")";
  return os;
}

static std::ostream& operator<<(std::ostream& os, const Item& item) {
  os << "{"
     << "cword: " << item.cword << ", pos: "
     << "(" << unsigned(item.pos.first) << ", " << unsigned(item.pos.second)
     << ")"
     << ", is_multi: " << item.is_multi
     << ", replace_count: " << unsigned(item.replace_count)
     << ", value: " << item.value << ", gems: " << unsigned(item.gems)
     << ", ivalue: " << (item.getInferredValue());
  os << ", visited: [";
  for (const auto& p : item.visited) {
    os << "(" << p.first << ", " << p.second << "), ";
  }
  os << "]";
  os << "}";
  return os;
}

static bool operator<(const Item& a, const Item& b) {
  return a.getInferredValue() < b.getInferredValue();
}

static bool operator>(const Item& a, const Item& b) {
  return a.getInferredValue() > b.getInferredValue();
}

static bool operator==(const Item& lhs, const Item& rhs) {
  return (lhs.cword == rhs.cword) && (lhs.value == rhs.value) &&
         (lhs.replace_count == rhs.replace_count) && (lhs.gems == rhs.gems);
}

#endif
