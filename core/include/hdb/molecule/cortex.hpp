#pragma once

#include <cstddef>
#include <functional>
#include <vector>

#include <hdb/atom/neuron.hpp>
#include <hdb/atom/synapse.hpp>
#include <hdb/molecule/hippocampus.hpp>

namespace hdb {

using Impulse = std::function<Scalar(const Synapse&)>;

struct Thought {
  Neuron neuron;
  Scalar flux;
};

using Imagination = std::vector<Thought>;

struct ImagineParams {
  Nid start;
  std::size_t epochs;
  Scalar creativity;
};

class Cortex {
 public:
  Imagination Imagine(
      const Engram& engram,
      const ImagineParams& params,
      const Impulse& impulse);
};

}  // namespace hdb
