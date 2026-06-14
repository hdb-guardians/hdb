#pragma once

#include <cstddef>
#include <optional>
#include <span>
#include <utility>
#include <vector>

#include <hdb/types.hpp>

namespace hdb {

struct AbstractRecord {
  Aid aid;
  std::vector<std::byte> actor;
  Nid nid;
  std::vector<std::byte> payload;
  TimePoint timestamp;
  std::optional<std::vector<std::byte>> meta;
};

class IAbstractTable {
 public:
  virtual ~IAbstractTable() = default;

  virtual std::optional<Aid> insert(const AbstractRecord&) = 0;
  virtual std::vector<std::pair<Nid, Scalar>> search_nearest(
      std::span<const std::byte> payload,
      const std::size_t limit) const = 0;
};

}  // namespace hdb
