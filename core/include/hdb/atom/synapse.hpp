#pragma once

#include <cstddef>
#include <optional>
#include <span>
#include <vector>

#include <hdb/types.hpp>
#include <hdb/store/isynapse_table.hpp>

namespace hdb {

class Synapse {
 public:
  explicit Synapse(ISynapseTable& table);

  std::optional<Sid> Fire(
      std::span<const std::byte> actor,
      const Nid& from_nid,
      const Nid& to_nid,
      std::optional<std::span<const std::byte>> meta = std::nullopt);

 private:
  ISynapseTable& table_;
};

}  // namespace hdb
