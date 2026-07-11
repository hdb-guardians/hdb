#pragma once

#include <optional>
#include <vector>

#include <hdb/quark/synapse.hpp>

namespace hdb {

class SynapseTable {
 public:
  virtual ~SynapseTable() = default;

  virtual std::optional<Synapse> insert(const Synapse&) = 0;
  virtual std::optional<Synapse> find_by_id(const Sid&) const = 0;
  virtual std::vector<Synapse> find_by_range(
      const Moment since,
      const Moment until) const = 0;
};

}  // namespace hdb
