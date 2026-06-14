#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include <hdb/types.hpp>

namespace hdb {

struct NeuronRecord {
  Nid nid;
  std::vector<std::byte> actor;
  std::vector<std::byte> payload;
  TimePoint timestamp;
  std::optional<std::vector<std::byte>> meta;
};

class INeuronTable {
 public:
  virtual ~INeuronTable() = default;

  virtual std::optional<Nid> insert(const NeuronRecord&) = 0;
  virtual std::optional<NeuronRecord> find(const Nid&) const = 0;
  virtual std::vector<NeuronRecord> find_range(
      const TimePoint since,
      const TimePoint until) const = 0;
};

}  // namespace hdb
