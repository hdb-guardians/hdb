#pragma once

#include <memory>

#include <hdb/sqlite/context.hpp>
#include <hdb/store/neuron_table.hpp>

namespace hdb::sqlite {

class SqliteNeuronTable : public NeuronTable {
 public:
  explicit SqliteNeuronTable(std::shared_ptr<SqliteContext> ctx);

  std::optional<Neuron> insert(const Neuron& neuron) override;
  std::optional<Neuron> find_by_id(const Nid& name) const override;
  std::vector<Neuron> find_by_range(Moment since, Moment until) const override;

 private:
  std::shared_ptr<SqliteContext> ctx_;
};

}  // namespace hdb::sqlite
