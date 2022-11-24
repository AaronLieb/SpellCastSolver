#ifndef _ITEM_H
#define _ITEM_H

#include "types.h"

struct Item {
  std::pair<int, int> pos;
  std::string cword;
  Seen visited;
  int value;
  bool is_multi;
  bool has_replaced;
  Replacement replacement;
};

#endif