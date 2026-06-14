#pragma once

#include <optional>
#include <vector>

#include <hdb/atom/neuron.hpp>

namespace hdb {

class NeuronTable {
 public:
  virtual ~NeuronTable() = default;

  virtual std::optional<Neuron> insert(const Neuron&) = 0;
  virtual std::optional<Neuron> find(const Nid&) const = 0;
  virtual std::vector<Neuron> find(const TimePoint since, const TimePoint until)
      const = 0;
};

}  // namespace hdb
