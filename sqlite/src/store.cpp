#include <memory>

#include <hdb/sqlite/context.hpp>
#include <hdb/sqlite/dream_table.hpp>
#include <hdb/sqlite/neuron_table.hpp>
#include <hdb/sqlite/store.hpp>
#include <hdb/sqlite/synapse_table.hpp>

namespace hdb::sqlite {

SqliteStore make_sqlite_store(
    const std::string& db_path,
    const std::string& sqlite_vec_extension_path) {
  auto ctx =
      std::make_shared<SqliteContext>(db_path, sqlite_vec_extension_path);
  ctx->initialize_schema();

  SqliteStore store;
  store.ctx_ = ctx;
  store.neurons =
      std::shared_ptr<NeuronTable>(std::make_shared<SqliteNeuronTable>(*ctx));
  store.synapses =
      std::shared_ptr<SynapseTable>(std::make_shared<SqliteSynapseTable>(*ctx));
  store.dreams =
      std::shared_ptr<DreamTable>(std::make_shared<SqliteDreamTable>(*ctx));
  return store;
}

}  // namespace hdb::sqlite
