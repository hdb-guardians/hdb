#pragma once

#include <optional>
#include <vector>

#include <hdb/types.hpp>

namespace hdb {

struct Neuron {
  Nid name;
  std::vector<std::byte> actor;
  std::vector<std::byte> payload;
  Moment moment;
  std::optional<std::vector<std::byte>> meta;
};

}  // namespace hdb
