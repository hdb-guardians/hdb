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
      const Nid& from_nid,
      const Nid& to_nid,
      std::span<const std::byte> actor,
      std::optional<std::span<const std::byte>> meta = std::nullopt);

 private:
  ISynapseTable& table_;
};

}  // namespace hdb
