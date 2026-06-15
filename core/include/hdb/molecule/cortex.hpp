#pragma once

#include <hdb/atom/impulse.hpp>
#include <hdb/atom/thought.hpp>
#include <hdb/atom/imagination.hpp>
#include <hdb/atom/imagine_params.hpp>
#include <hdb/atom/engram.hpp>

namespace hdb {

class Cortex {
 public:
  Imagination Imagine(
      const Engram& engram,
      const ImagineParams& params,
      const Impulse& impulse);
};

}  // namespace hdb
