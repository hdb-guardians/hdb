#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include <hdb/types.hpp>

namespace hdb {

struct SynapseRecord {
  Sid sid;
  std::vector<std::byte> actor;
  Nid from_nid;
  Nid to_nid;
  TimePoint timestamp;
  std::optional<std::vector<std::byte>> meta;
};

class ISynapseTable {
 public:
  virtual ~ISynapseTable() = default;

  virtual std::optional<Sid> insert(const SynapseRecord&) = 0;
  virtual std::optional<SynapseRecord> find(const Sid&) const = 0;
  virtual std::vector<SynapseRecord> find_range(
      const TimePoint since,
      const TimePoint until) const = 0;
};

}  // namespace hdb
