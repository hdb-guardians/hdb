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
      : _neurons(std::move(neurons)), _synapses(std::move(synapses)) {
    _adjacency_by_nid.clear();
    _adjacency_by_nid.reserve(_neurons.size());
    for (std::size_t i = 0; i < _neurons.size(); ++i) {
      _adjacency_by_nid[_neurons[i].name].neuron_index = i;
    }

    for (std::size_t i = 0; i < _synapses.size(); ++i) {
      _adjacency_by_nid[_synapses[i].from].synapse_indices.push_back(i);
    }
  }

  const std::vector<Neuron>& Neurons() const noexcept { return _neurons; }

  const std::vector<Synapse>& Synapses() const noexcept { return _synapses; }

  const std::unordered_map<Nid, AdjacencyEntry>& AdjacencyByNid()
      const noexcept {
    return _adjacency_by_nid;
  }

 private:
  std::vector<Neuron> _neurons;
  std::vector<Synapse> _synapses;
  std::unordered_map<Nid, AdjacencyEntry> _adjacency_by_nid;
};

}
