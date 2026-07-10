#pragma once

#include <hdb/sqlite/context.hpp>
#include <hdb/store/synapse_table.hpp>

namespace hdb::sqlite {

class SqliteSynapseTable : public SynapseTable {
 public:
  explicit SqliteSynapseTable(SqliteContext& ctx);

  std::optional<Synapse> insert(const Synapse& synapse) override;
  std::optional<Synapse> find(const Sid& name) const override;
  std::vector<Synapse> find(Moment since, Moment until) const override;

 private:
  SqliteContext& ctx_;
};

}  // namespace hdb::sqlite
