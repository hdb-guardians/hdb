#pragma once

#include <cstddef>
#include <optional>
#include <span>
#include <vector>

#include <hdb/atom/neuron.hpp>
#include <hdb/atom/synapse.hpp>
#include <hdb/atom/abstract.hpp>
#include <hdb/store/neuron_table.hpp>
#include <hdb/store/synapse_table.hpp>
#include <hdb/store/abstract_table.hpp>

namespace hdb {

struct Engram {
  std::vector<Neuron> neurons;
  std::vector<Synapse> synapses;
};

class Hippocampus {
 public:
  Hippocampus(
      NeuronTable& neurons,
      SynapseTable& synapses,
      AbstractTable& abstracts);

  std::vector<Resonance> Resonate(
      std::span<const std::byte> stimulus,
      const std::size_t limit = 10);

  std::optional<Engram> Reminisce(
      const TimePoint since = TimePoint::min(),
      const TimePoint until = TimePoint::max());

 private:
  NeuronTable& neurons_;
  SynapseTable& synapses_;
  AbstractTable& abstracts_;
};

}  // namespace hdb
