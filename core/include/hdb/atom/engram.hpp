#pragma once

#include <hdb/quark/neuron.hpp>
#include <hdb/quark/synapse.hpp>

namespace hdb {

struct Engram {
  std::vector<Neuron> neurons;
  std::vector<Synapse> synapses;
};

}  // namespace hdb
