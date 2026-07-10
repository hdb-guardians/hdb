#pragma once

#include <cstddef>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include <hdb/molecule/cortex.hpp>
#include <hdb/molecule/hippocampus.hpp>
#include <hdb/molecule/prefrontal.hpp>
#include <hdb/molecule/thalamus.hpp>
#include <hdb/standard/sqlite_context.hpp>
#include <hdb/standard/sqlite_dream_table.hpp>
#include <hdb/standard/sqlite_neuron_table.hpp>
#include <hdb/standard/sqlite_synapse_table.hpp>

namespace hdb::standard {

class Session {
 public:
  explicit Session(
      const std::string& db_path,
      const std::string& sqlite_vec_extension_path);

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
      const Natural limit = 10);

  std::optional<Engram> Reminisce(
      const Moment since = Moment::min(),
      const Moment until = Moment::max());

  Imagination Imagine(
      const Engram& engram,
      const Nid& start,
      const Natural epochs,
      const Real creativity,
      const Impulse& impulse);

 private:
  SqliteContext _ctx;
  SqliteNeuronTable _neurons;
  SqliteSynapseTable _synapses;
  SqliteDreamTable _dreams;
  Prefrontal prefrontal_;
  Thalamus thalamus_;
  Hippocampus hippocampus_;
  Cortex cortex_;
};

}
