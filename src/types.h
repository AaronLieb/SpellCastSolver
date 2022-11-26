#ifndef _TYPES_H
#define _TYPES_H

#include <set>
#include <string>
#include <vector>

using Matrix = std::vector<std::string>;
using Seen = std::set<std::pair<int, int>>;
using Replacement = std::pair<std::pair<int, int>, char>;

#define Results \
  std::map</* replace_count: */ int, std::set<Item, std::greater<Item>>>
#endif