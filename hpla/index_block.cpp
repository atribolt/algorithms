#include "index_block.hpp"


index_block::iterator::iterator(index_block &block, point_type start)
  : block(block)
{
  point_type offset = (start - block.start);
  _iter = offset.x * offset.y * offset.z;
}

index_block::point_type index_block::iterator::operator*()
{
  auto [sx, sy, sz] = block.size;

  value_type sxy = sx * sy; // 1z

  point_type idx;
  idx.x = _iter % sx;
  idx.z = _iter / sxy;
  idx.y = (_iter - idx.z*sxy) / sx;

  return idx + block.start;
}

bool index_block::iterator::operator==(const iterator &other) const
{
  return _iter == other._iter;
}

bool index_block::iterator::operator!=(const iterator &other) const
{
  return _iter != other._iter;
}

index_block::iterator &index_block::iterator::operator++()
{
  ++_iter;
  return *this;
}

index_block::index_block(point_type beg, point_type end)
{
  start = beg;
  size = end - beg;
}

index_block::iterator index_block::begin()
{
  return iterator(*this, start);
}

index_block::iterator index_block::end()
{
  return iterator(*this, start + size);
}

index_block::value_type index_block::count() const
{
  return size.x * size.y * size.z;
}
