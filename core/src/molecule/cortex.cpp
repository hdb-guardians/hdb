#include <hdb/molecule/cortex.hpp>

namespace hdb {

Imagination Cortex::Imagine(
    const Engram& engram,
    const Nid& start,
    const Natural epochs,
    const Real creativity,
    const Impulse& impulse) {
  std::unordered_map<Nid, const Neuron*> neuron_map;
  neuron_map.reserve(engram.neurons.size());
  for (const auto& n : engram.neurons) {
    neuron_map.emplace(n.name, &n);
  }

  std::unordered_map<Nid, std::vector<const Synapse*>> synapse_map;
  for (const auto& s : engram.synapses) {
    synapse_map[s.from].push_back(&s);
  }

  std::unordered_map<Nid, Real> flux_map;
  flux_map[start] = Real{1};

  std::unordered_map<Nid, Real> wave_map;
  wave_map[start] = Real{1};

  thread_local std::mt19937 rng{std::random_device{}()};
  std::uniform_real_distribution<Real> noise{Real{-1}, Real{1}};

  for (Natural epoch = 0; epoch < epochs; ++epoch) {
    std::unordered_map<Nid, Real> _wave_map;

    for (const auto& [nid, flux] : wave_map) {
      auto i = synapse_map.find(nid);
      if (i == synapse_map.end()) continue;

      for (const Synapse* s : i->second) {
        Real contribution = flux * impulse(*s);

        if (creativity != Real{0}) {
          contribution += creativity * noise(rng);
        }

        _wave_map[s->to] += contribution;
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
    auto i = neuron_map.find(nid);
    if (i != neuron_map.end()) {
      imagination.push_back(Thought{*i->second, flux});
    }
  }

  std::ranges::sort(imagination, [](const Thought& a, const Thought& b) {
    return a.flux > b.flux;
  });

  return imagination;
}

}
