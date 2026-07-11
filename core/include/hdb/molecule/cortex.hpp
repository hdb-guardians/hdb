#pragma once

#include <hdb/atom/engram.hpp>
#include <hdb/atom/imagination.hpp>
#include <hdb/atom/impulse.hpp>

namespace hdb {

class Cortex {
 public:
  Imagination Imagine(
      const Engram& engram,
      const Nid& start,
      const std::size_t epochs,
      const Real creativity,
      const Impulse& impulse);
};

}  // namespace hdb
