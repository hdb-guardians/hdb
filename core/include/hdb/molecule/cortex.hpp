#pragma once

#include <hdb/atom/impulse.hpp>
#include <hdb/atom/thought.hpp>
#include <hdb/atom/imagination.hpp>
#include <hdb/atom/engram.hpp>

namespace hdb {

class Cortex {
 public:
  Imagination Imagine(
      const Engram& engram,
      const Nid& start,
      const Natural epochs,
      const Real creativity,
      const Impulse& impulse);
};

}  // namespace hdb
