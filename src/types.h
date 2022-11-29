#ifndef _TYPES_H
#define _TYPES_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "Cell.h"
#include "env.h"

using Matrix = std::vector<std::vector<Cell>>;
using Seen = std::set<std::pair<int, int>>;
using Replacements = std::map<std::pair<int, int>, char>;
using smallint = uint8_t;

#define Results \
  std::map</* replace_count: */ int, std::set<Item, std::greater<Item>>>
#endif
