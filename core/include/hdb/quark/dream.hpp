#pragma once

#include <hdb/types.hpp>

namespace hdb {

struct Dream {
  Did name;
  std::vector<std::byte> actor;
  Nid neuron;
  std::vector<std::byte> payload;
  Moment moment;
  std::optional<std::vector<std::byte>> meta;
};

}
