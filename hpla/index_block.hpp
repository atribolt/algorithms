#pragma once

#include "point.hpp"


struct index_block
{
public:
  struct iterator;

public:
  using value_type = ssize_t;
  using point_type = point<value_type>;

public:
  index_block(point_type beg, point_type end);

  iterator begin();
  iterator end();

  value_type count() const;

public:
  point_type start;
  point_type size;
};


struct index_block::iterator {
public:
  iterator(index_block& block, point_type start);

  point_type operator*();

  bool operator==(const iterator& other) const;
  bool operator!=(const iterator& other) const;

  iterator& operator++();

  value_type _iter;
  index_block& block;
};
