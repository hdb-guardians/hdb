#pragma once

#include <optional>
#include <span>
#include <vector>

#include <hdb/atom/engram.hpp>
#include <hdb/store/dream_table.hpp>
#include <hdb/store/neuron_table.hpp>
#include <hdb/store/synapse_table.hpp>

namespace hdb {

class Hippocampus {
 public:
  explicit Hippocampus(
      NeuronTable& neurons,
      SynapseTable& synapses,
      DreamTable& dreams);

  std::vector<Resonance> Resonate(
      std::span<const std::byte> stimulus,
      const Natural limit = 10);

  std::optional<Engram> Reminisce(
      const Moment since = Moment::min(),
      const Moment until = Moment::max());

 private:
  NeuronTable& neurons;
  SynapseTable& synapses;
  DreamTable& dreams;
};

}  // namespace hdb
