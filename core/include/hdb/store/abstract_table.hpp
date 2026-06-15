#pragma once

#include <optional>
#include <span>
#include <vector>

#include <hdb/atom/abstract.hpp>
#include <hdb/atom/resonance.hpp>

namespace hdb {

class AbstractTable {
 public:
  virtual ~AbstractTable() = default;

  virtual std::optional<Abstract> insert(const Abstract&) = 0;
  virtual std::vector<Resonance> find(
      std::span<const std::byte> payload,
      const Natural limit) const = 0;
};

}  // namespace hdb
