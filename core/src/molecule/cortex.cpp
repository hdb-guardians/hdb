#include <algorithm>
#include <random>
#include <unordered_map>

#include <hdb/molecule/cortex.hpp>

namespace hdb {

Imagination Cortex::Imagine(
    const Engram& engram,
    const Nid& start,
    const Natural epochs,
    const Real creativity,
    const Impulse& impulse) {
  const auto& synapses = engram.Synapses();
  const auto& neurons = engram.Neurons();
  const auto& adjacency_by_nid = engram.AdjacencyByNid();

  std::unordered_map<Nid, Real> flux_map;
  flux_map[start] = Real{1};

  std::unordered_map<Nid, Real> wave_map;
  wave_map[start] = Real{1};

  thread_local std::mt19937 rng{std::random_device{}()};
  std::uniform_real_distribution<Real> noise{Real{-1}, Real{1}};

  for (Natural epoch = 0; epoch < epochs; ++epoch) {
    std::unordered_map<Nid, Real> _wave_map;

    for (const auto& [nid, flux] : wave_map) {
      auto i = adjacency_by_nid.find(nid);
      if (i == adjacency_by_nid.end()) continue;

      for (const std::size_t synapse_index : i->second.synapse_indices) {
        const Synapse& synapse = synapses[synapse_index];
        Real contribution = flux * impulse(synapse);

        if (creativity != Real{0}) {
          contribution += creativity * noise(rng);
        }

        _wave_map[synapse.to] += contribution;
      }
    }

    if (_wave_map.empty()) break;

    for (const auto& [nid, f] : _wave_map) {
      flux_map[nid] += f;
    }

    wave_map = std::move(_wave_map);
  }

  Imagination imagination;
  imagination.reserve(flux_map.size());

  for (const auto& [nid, flux] : flux_map) {
    auto i = adjacency_by_nid.find(nid);
    if (i != adjacency_by_nid.end() && i->second.neuron_index.has_value()) {
      imagination.push_back(Thought{neurons[*i->second.neuron_index], flux});
    }
  }

  std::ranges::sort(imagination, [](const Thought& a, const Thought& b) {
    return a.flux > b.flux;
  });

  return imagination;
}

}
