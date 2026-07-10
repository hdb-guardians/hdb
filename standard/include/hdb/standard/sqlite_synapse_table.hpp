#pragma once

#include <hdb/standard/sqlite_context.hpp>
#include <hdb/store/synapse_table.hpp>

namespace hdb::standard {

class SqliteSynapseTable : public SynapseTable {
 public:
  explicit SqliteSynapseTable(SqliteContext& ctx);

  std::optional<Synapse> insert(const Synapse& synapse) override;
  std::optional<Synapse> find(const Sid& name) const override;
  std::vector<Synapse> find(const Moment since, const Moment until)
      const override;

 private:
  SqliteContext& _ctx;
};

}
