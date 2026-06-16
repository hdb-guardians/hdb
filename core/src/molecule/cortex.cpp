#include <hdb/molecule/cortex.hpp>

#include <algorithm>
#include <random>
#include <unordered_map>
#include <vector>

namespace hdb {

Imagination Cortex::Imagine(
    const Engram& engram,
    const Nid& start,
    const Natural epochs,
    const Real creativity,
    const Impulse& impulse) {
  // --- Build lookup structures from the Engram ---

  std::unordered_map<Nid, const Neuron*> neuron_map;
  neuron_map.reserve(engram.neurons.size());
  for (const auto& n : engram.neurons) {
    neuron_map.emplace(n.name, &n);
  }

  // Adjacency list: Nid -> outgoing synapses
  std::unordered_map<Nid, std::vector<const Synapse*>> adj;
  for (const auto& s : engram.synapses) {
    adj[s.from].push_back(&s);
  }

  // --- Spreading activation ---
  // total_flux accumulates activation across all epochs.
  // wave carries the current epoch's activation front.

  std::unordered_map<Nid, Real> total_flux;
  total_flux[start] = Real{1};

  std::unordered_map<Nid, Real> wave;
  wave[start] = Real{1};

  thread_local std::mt19937 rng{std::random_device{}()};
  std::uniform_real_distribution<Real> noise{Real{-1}, Real{1}};

  for (Natural e = 0; e < epochs; ++e) {
    std::unordered_map<Nid, Real> next_wave;

    for (const auto& [nid, flux] : wave) {
      auto it = adj.find(nid);
      if (it == adj.end()) continue;

      for (const Synapse* s : it->second) {
        Real contribution = flux * impulse(*s);

        if (creativity > Real{0}) {
          contribution += creativity * noise(rng);
          contribution = std::max(Real{0}, contribution);
        }

        next_wave[s->to] += contribution;
      }
    }

    if (next_wave.empty()) break;

    // Accumulate into total_flux
    for (const auto& [nid, f] : next_wave) {
      total_flux[nid] += f;
    }

    wave = std::move(next_wave);
  }

  // --- Build Imagination from activated neurons ---

  Imagination result;
  result.reserve(total_flux.size());

  for (const auto& [nid, flux] : total_flux) {
    auto it = neuron_map.find(nid);
    if (it != neuron_map.end()) {
      result.push_back(Thought{*it->second, flux});
    }
  }

  // Sort by flux descending (most activated first)
  std::ranges::sort(result, [](const Thought& a, const Thought& b) {
    return a.flux > b.flux;
  });

  return result;
}

}  // namespace hdb
