#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <string>

#include <hdb/sqlite/context.hpp>
#include <hdb/sqlite/dream_table.hpp>
#include <hdb/sqlite/neuron_table.hpp>
#include <hdb/sqlite/open.hpp>
#include <hdb/sqlite/synapse_table.hpp>

namespace hdb::sqlite {

std::string resolve_vec_extension_path(const std::string& path) {
  if (!path.empty()) {
    return path;
  }
  const char* from_env = std::getenv("HDB_SQLITE_VEC_EXTENSION");
  if (from_env != nullptr && from_env[0] != '\0') {
    return std::string{from_env};
  }
  throw std::runtime_error(
      "hdb::sqlite: sqlite-vec extension path is required. "
      "Pass it explicitly or set HDB_SQLITE_VEC_EXTENSION.");
}

std::tuple<
    std::shared_ptr<NeuronTable>,
    std::shared_ptr<SynapseTable>,
    std::shared_ptr<DreamTable>>
open_sqlite(
    const std::string& db_path,
    const std::string& sqlite_vec_extension_path) {
  auto ctx =
      std::make_shared<SqliteContext>(db_path, sqlite_vec_extension_path);
  ctx->initialize_schema();
  return {
      std::shared_ptr<NeuronTable>(std::make_shared<SqliteNeuronTable>(ctx)),
      std::shared_ptr<SynapseTable>(std::make_shared<SqliteSynapseTable>(ctx)),
      std::shared_ptr<DreamTable>(std::make_shared<SqliteDreamTable>(ctx)),
  };
}

}  // namespace hdb::sqlite
