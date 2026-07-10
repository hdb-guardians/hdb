#pragma once

#include <memory>
#include <string>

#include <hdb/store/dream_table.hpp>
#include <hdb/store/neuron_table.hpp>
#include <hdb/store/synapse_table.hpp>

namespace hdb::sqlite {

class SqliteContext;

struct SqliteStore {
  std::shared_ptr<NeuronTable> neurons;
  std::shared_ptr<SynapseTable> synapses;
  std::shared_ptr<DreamTable> dreams;

 private:
  std::shared_ptr<SqliteContext> ctx_;

  friend SqliteStore make_sqlite_store(
      const std::string& db_path,
      const std::string& sqlite_vec_extension_path);
};

SqliteStore make_sqlite_store(
    const std::string& db_path,
    const std::string& sqlite_vec_extension_path = "");

}  // namespace hdb::sqlite
