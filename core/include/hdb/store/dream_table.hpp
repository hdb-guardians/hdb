#pragma once

#include <optional>
#include <span>
#include <vector>

#include <hdb/quark/dream.hpp>
#include <hdb/quark/resonance.hpp>

namespace hdb {

class DreamTable {
 public:
  virtual ~DreamTable() = default;

  virtual std::optional<Dream> insert(const Dream&) = 0;
  virtual std::vector<Resonance> find(
      std::span<const std::byte> payload,
      const Natural limit) const = 0;
};

}  // namespace hdb
