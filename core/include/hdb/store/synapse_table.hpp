#pragma once

#include <optional>
#include <vector>

#include <hdb/atom/synapse.hpp>

namespace hdb {

class SynapseTable {
 public:
  virtual ~SynapseTable() = default;

  virtual std::optional<Synapse> insert(const Synapse&) = 0;
  virtual std::optional<Synapse> find(const Sid&) const = 0;
  virtual std::vector<Synapse> find(
      const TimePoint since,
      const TimePoint until) const = 0;
};

}  // namespace hdb
