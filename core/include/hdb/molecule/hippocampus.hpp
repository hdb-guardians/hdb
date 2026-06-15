#pragma once

#include <optional>
#include <span>

#include <hdb/atom/neuron.hpp>
#include <hdb/atom/synapse.hpp>
#include <hdb/atom/engram.hpp>
#include <hdb/atom/resonance.hpp>
#include <hdb/store/neuron_table.hpp>
#include <hdb/store/synapse_table.hpp>
#include <hdb/store/abstract_table.hpp>

namespace hdb {

class Hippocampus {
 public:
  explicit Hippocampus(
      NeuronTable& neurons,
      SynapseTable& synapses,
      AbstractTable& abstracts);

  std::vector<Resonance> Resonate(
      std::span<const std::byte> stimulus,
      const Natural limit = 10);

  std::optional<Engram> Reminisce(
      const TimePoint since = TimePoint::min(),
      const TimePoint until = TimePoint::max());

 private:
  NeuronTable& neurons_;
  SynapseTable& synapses_;
  AbstractTable& abstracts_;
};

}  // namespace hdb
