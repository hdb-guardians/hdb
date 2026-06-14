#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include <hdb/types.hpp>

namespace hdb {

struct Synapse {
  Sid name;
  std::vector<std::byte> actor;
  Nid from;
  Nid to;
  TimePoint timestamp;
  std::optional<std::vector<std::byte>> meta;
};

}  // namespace hdb
