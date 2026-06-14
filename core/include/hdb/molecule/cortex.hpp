#pragma once

#include <cstddef>
#include <functional>
#include <vector>

#include <hdb/atom/neuron.hpp>
#include <hdb/atom/synapse.hpp>
#include <hdb/molecule/hippocampus.hpp>

namespace hdb {

using Impulse = std::function<Real(const Synapse&)>;

struct Thought {
  Neuron neuron;
  Real flux;
};

using Imagination = std::vector<Thought>;

struct ImagineParams {
  Nid start;
  Natural epochs;
  Real creativity;
};

class Cortex {
 public:
  Imagination Imagine(
      const Engram& engram,
      const ImagineParams& params,
      const Impulse& impulse);
};

}  // namespace hdb
