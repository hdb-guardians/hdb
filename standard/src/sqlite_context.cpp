#include <sqlite3.h>

#include <hdb/standard/sqlite_context.hpp>
#include <stdexcept>
#include <string>

namespace {

void ExecOrThrow(sqlite3* db, const char* sql) {
  char* err = nullptr;
  const int rc = sqlite3_exec(db, sql, nullptr, nullptr, &err);
  if (rc == SQLITE_OK) {
    return;
  }

  std::string msg = err == nullptr ? "sqlite3_exec failed" : err;
  sqlite3_free(err);
  throw std::runtime_error(msg);
}

}

namespace hdb::standard {

SqliteContext::SqliteContext(
    const std::string& db_path,
    const std::string& sqlite_vec_extension_path) {
  const int rc = sqlite3_open_v2(
      db_path.c_str(),
      &_db,
      SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX,
      nullptr);
  if (rc != SQLITE_OK || _db == nullptr) {
    throw std::runtime_error("failed to open sqlite database");
  }

  ExecOrThrow(_db, "PRAGMA journal_mode = WAL;");
  ExecOrThrow(_db, "PRAGMA foreign_keys = ON;");

  sqlite3_enable_load_extension(_db, 1);
  char* err = nullptr;
  const int load_rc = sqlite3_load_extension(
      _db, sqlite_vec_extension_path.c_str(), nullptr, &err);

  if (load_rc != SQLITE_OK) {
    std::string msg = err == nullptr ? "failed to load sqlite-vec" : err;
    sqlite3_free(err);
    throw std::runtime_error(msg);
  }
}

SqliteContext::~SqliteContext() {
  if (_db != nullptr) {
    sqlite3_close(_db);
    _db = nullptr;
  }
}

sqlite3* SqliteContext::handle() const noexcept { return _db; }

void SqliteContext::initialize_schema() {
  ExecOrThrow(
      _db,
      "CREATE TABLE IF NOT EXISTS neurons("
      "name TEXT PRIMARY KEY,"
      "actor BLOB NOT NULL,"
      "payload BLOB NOT NULL,"
      "moment INTEGER NOT NULL,"
      "meta BLOB"
      ");");

  ExecOrThrow(
      _db, "CREATE INDEX IF NOT EXISTS idx_neurons_moment ON neurons(moment);");

  ExecOrThrow(
      _db,
      "CREATE TABLE IF NOT EXISTS synapses("
      "name TEXT PRIMARY KEY,"
      "actor BLOB NOT NULL,"
      "from_id TEXT NOT NULL,"
      "to_id TEXT NOT NULL,"
      "moment INTEGER NOT NULL,"
      "meta BLOB"
      ");");

  ExecOrThrow(
      _db,
      "CREATE INDEX IF NOT EXISTS idx_synapses_moment ON synapses(moment);");
  ExecOrThrow(
      _db,
      "CREATE INDEX IF NOT EXISTS idx_synapses_from_to ON "
      "synapses(from_id, to_id);");

  ExecOrThrow(
      _db,
      "CREATE TABLE IF NOT EXISTS dreams("
      "name TEXT PRIMARY KEY,"
      "actor BLOB NOT NULL,"
      "neuron TEXT NOT NULL,"
      "payload BLOB NOT NULL,"
      "moment INTEGER NOT NULL,"
      "meta BLOB"
      ");");

  ExecOrThrow(
      _db, "CREATE INDEX IF NOT EXISTS idx_dreams_moment ON dreams(moment);");
  ExecOrThrow(
      _db, "CREATE INDEX IF NOT EXISTS idx_dreams_neuron ON dreams(neuron);");
}

}
