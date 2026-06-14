#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include <hdb/types.hpp>

namespace hdb {

struct Abstract {
  Aid name;
  std::vector<std::byte> actor;
  Nid neuron;
  std::vector<std::byte> payload;
  TimePoint timestamp;
  std::optional<std::vector<std::byte>> meta;
};

struct Resonance {
  Nid neuron;
  Scalar fidelity;
};

}  // namespace hdb
