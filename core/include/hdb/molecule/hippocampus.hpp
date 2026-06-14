#pragma once

#include <cstddef>
#include <optional>
#include <span>
#include <vector>

#include <hdb/types.hpp>
#include <hdb/store/ineuron_table.hpp>
#include <hdb/store/isynapse_table.hpp>
#include <hdb/store/iabstract_table.hpp>

namespace hdb {

struct Resonance {
  Nid nid;
  Scalar fidelity;
};

struct Engram {
  std::vector<NeuronRecord> neurons;
  std::vector<SynapseRecord> synapses;
};

class Hippocampus {
 public:
  Hippocampus(
      INeuronTable& neurons,
      ISynapseTable& synapses,
      IAbstractTable& abstracts);

  std::vector<Resonance> Resonate(
      std::span<const std::byte> stimulus,
      const std::size_t limit = 10);

  std::optional<Engram> Reminisce(
      const TimePoint since = TimePoint::min(),
      const TimePoint until = TimePoint::max());

 private:
  INeuronTable& neurons_;
  ISynapseTable& synapses_;
  IAbstractTable& abstracts_;
};

}  // namespace hdb
