#pragma once

#include <vector>

#include <hdb/atom/neuron.hpp>
#include <hdb/atom/synapse.hpp>

namespace hdb {

struct Engram {
  std::vector<Neuron> neurons;
  std::vector<Synapse> synapses;
};

}  // namespace hdb
