#pragma once

#include <cstddef>
#include <optional>
#include <span>
#include <vector>

#include <hdb/types.hpp>
#include <hdb/store/iabstract_table.hpp>

namespace hdb {

class Thalamus {
 public:
  explicit Thalamus(IAbstractTable& table);

  std::optional<Aid> Consolidate(
      const Nid& nid,
      std::span<const std::byte> actor,
      std::span<const std::byte> payload,
      std::optional<std::span<const std::byte>> meta = std::nullopt);

 private:
  IAbstractTable& table_;
};

}  // namespace hdb
