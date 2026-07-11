#pragma once

#include <cstddef>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

#include <hdb/quark/neuron.hpp>
#include <hdb/quark/synapse.hpp>

namespace hdb {

struct Engram {
  struct AdjacencyEntry {
    std::optional<std::size_t> neuron_index;
    std::vector<std::size_t> synapse_indices;
  };

  Engram(std::vector<Neuron> neurons, std::vector<Synapse> synapses)
      : neurons_(std::move(neurons)), synapses_(std::move(synapses)) {
    adjacency_by_nid_.clear();
    adjacency_by_nid_.reserve(neurons_.size());
    for (std::size_t i = 0; i < neurons_.size(); ++i) {
      adjacency_by_nid_[neurons_[i].name].neuron_index = i;
    }

    for (std::size_t i = 0; i < synapses_.size(); ++i) {
      adjacency_by_nid_[synapses_[i].source].synapse_indices.push_back(i);
    }
  }

  const std::vector<Neuron>& Neurons() const noexcept { return neurons_; }

  const std::vector<Synapse>& Synapses() const noexcept { return synapses_; }

  const std::unordered_map<Nid, AdjacencyEntry>& AdjacencyByNid()
      const noexcept {
    return adjacency_by_nid_;
  }

 private:
  std::vector<Neuron> neurons_;
  std::vector<Synapse> synapses_;
  std::unordered_map<Nid, AdjacencyEntry> adjacency_by_nid_;
};

}  // namespace hdb
