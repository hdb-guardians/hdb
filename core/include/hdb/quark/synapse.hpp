#pragma once

#include <hdb/types.hpp>

namespace hdb {

struct Synapse {
  Sid name;
  std::vector<std::byte> actor;
  Nid from;
  Nid to;
  Moment moment;
  std::optional<std::vector<std::byte>> meta;
};

}  // namespace hdb
