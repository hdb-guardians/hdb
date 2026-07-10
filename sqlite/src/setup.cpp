#include <memory>

#include <hdb/sqlite/context.hpp>
#include <hdb/sqlite/dream_table.hpp>
#include <hdb/sqlite/neuron_table.hpp>
#include <hdb/sqlite/setup.hpp>
#include <hdb/sqlite/synapse_table.hpp>
#include <hdb/store/dream_table.hpp>
#include <hdb/store/neuron_table.hpp>
#include <hdb/store/synapse_table.hpp>

namespace hdb::sqlite {

void append_sqlite_context(
    hdb::api::Context& ctx,
    const std::string& db_path,
    const std::string& sqlite_vec_extension_path) {
  auto db = std::make_shared<SqliteContext>(db_path, sqlite_vec_extension_path);
  db->initialize_schema();
  ctx.put<SqliteContext>(db);

  ctx.put<NeuronTable>(std::shared_ptr<NeuronTable>(
      std::make_shared<SqliteNeuronTable>(*ctx.get<SqliteContext>())));
  ctx.put<SynapseTable>(std::shared_ptr<SynapseTable>(
      std::make_shared<SqliteSynapseTable>(*ctx.get<SqliteContext>())));
  ctx.put<DreamTable>(std::shared_ptr<DreamTable>(
      std::make_shared<SqliteDreamTable>(*ctx.get<SqliteContext>())));
}

}  // namespace hdb::sqlite
