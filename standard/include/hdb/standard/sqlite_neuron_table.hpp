#pragma once

#include <hdb/standard/sqlite_context.hpp>
#include <hdb/store/neuron_table.hpp>

namespace hdb::standard {

class SqliteNeuronTable : public NeuronTable {
 public:
  explicit SqliteNeuronTable(SqliteContext& ctx);

  std::optional<Neuron> insert(const Neuron& neuron) override;
  std::optional<Neuron> find(const Nid& name) const override;
  std::vector<Neuron> find(const Moment since, const Moment until)
      const override;

 private:
  SqliteContext& _ctx;
};

}
