#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <span>
#include <vector>

#include <hdb/molecule/cortex.hpp>
#include <hdb/molecule/hippocampus.hpp>
#include <hdb/molecule/prefrontal.hpp>
#include <hdb/molecule/thalamus.hpp>
#include <hdb/store/dream_table.hpp>
#include <hdb/store/neuron_table.hpp>
#include <hdb/store/synapse_table.hpp>

namespace hdb::api {

class Session {
 public:
  explicit Session(
      std::shared_ptr<NeuronTable> neurons,
      std::shared_ptr<SynapseTable> synapses,
      std::shared_ptr<DreamTable> dreams);

  std::optional<Neuron> Sprout(
      const Nid& name,
      std::span<const std::byte> actor,
      std::span<const std::byte> payload,
      std::optional<std::span<const std::byte>> meta = std::nullopt);

  std::optional<Neuron> Awaken(const Nid& name);

  std::optional<Synapse> Fire(
      const Sid& name,
      std::span<const std::byte> actor,
      const Nid& from,
      const Nid& to,
      std::optional<std::span<const std::byte>> meta = std::nullopt);

  std::optional<Dream> Consolidate(
      const Did& name,
      std::span<const std::byte> actor,
      const Nid& neuron,
      std::span<const std::byte> payload,
      std::optional<std::span<const std::byte>> meta = std::nullopt);

  std::vector<Resonance> Resonate(
      std::span<const std::byte> stimulus,
      Natural limit = 10);

  std::optional<Engram> Reminisce(
      Moment since = Moment::min(),
      Moment until = Moment::max());

  Imagination Imagine(
      const Engram& engram,
      const Nid& start,
      Natural epochs,
      Real creativity,
      const Impulse& impulse);

 private:
  std::shared_ptr<NeuronTable> neurons_;
  std::shared_ptr<SynapseTable> synapses_;
  std::shared_ptr<DreamTable> dreams_;
  Prefrontal prefrontal_;
  Thalamus thalamus_;
  Hippocampus hippocampus_;
  Cortex cortex_;
};

}  // namespace hdb::api
