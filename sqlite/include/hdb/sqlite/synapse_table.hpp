#pragma once

#include <memory>

#include <hdb/sqlite/context.hpp>
#include <hdb/store/synapse_table.hpp>

namespace hdb::sqlite {

class SqliteSynapseTable : public SynapseTable {
 public:
  explicit SqliteSynapseTable(std::shared_ptr<SqliteContext> ctx);

  std::optional<Synapse> insert(const Synapse& synapse) override;
  std::optional<Synapse> find_by_id(const Sid& name) const override;
  std::vector<Synapse> find_by_range(Moment since, Moment until) const override;

 private:
  std::shared_ptr<SqliteContext> ctx_;
};

}  // namespace hdb::sqlite
