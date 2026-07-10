#pragma once

#include <optional>
#include <vector>

#include <hdb/quark/neuron.hpp>

namespace hdb {

class NeuronTable {
 public:
  virtual ~NeuronTable() = default;

  virtual std::optional<Neuron> insert(const Neuron&) = 0;
  virtual std::optional<Neuron> find(const Nid&) const = 0;
  virtual std::vector<Neuron> find(const Moment since, const Moment until)
      const = 0;
};

}
