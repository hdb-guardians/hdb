#pragma once

#include <hdb/sqlite/context.hpp>
#include <hdb/store/neuron_table.hpp>

namespace hdb::sqlite {

class SqliteNeuronTable : public NeuronTable {
 public:
  explicit SqliteNeuronTable(SqliteContext& ctx);

  std::optional<Neuron> insert(const Neuron& neuron) override;
  std::optional<Neuron> find(const Nid& name) const override;
  std::vector<Neuron> find(Moment since, Moment until) const override;

 private:
  SqliteContext& ctx_;
};

}  // namespace hdb::sqlite
