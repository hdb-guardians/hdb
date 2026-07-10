#include <sqlite3.h>

#include <hdb/standard/sqlite_neuron_table.hpp>
#include <hdb/standard/sqlite_utils.hpp>

namespace hdb::standard {

SqliteNeuronTable::SqliteNeuronTable(SqliteContext& ctx) : _ctx(ctx) {}

std::optional<Neuron> SqliteNeuronTable::insert(const Neuron& neuron) {
  static constexpr const char* kSql =
      "INSERT INTO neurons(name, actor, payload, moment, meta) "
      "VALUES(?1, ?2, ?3, ?4, ?5);";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(_ctx.handle(), kSql, -1, &stmt, nullptr) !=
      SQLITE_OK) {
    return std::nullopt;
  }

  BindText(stmt, 1, neuron.name);
  BindBlob(stmt, 2, neuron.actor);
  BindBlob(stmt, 3, neuron.payload);
  sqlite3_bind_int64(stmt, 4, ToSqlMoment(neuron.moment));
  if (neuron.meta.has_value()) {
    BindBlob(stmt, 5, *neuron.meta);
  } else {
    sqlite3_bind_null(stmt, 5);
  }

  const int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc != SQLITE_DONE) {
    return std::nullopt;
  }
  return neuron;
}

std::optional<Neuron> SqliteNeuronTable::find(const Nid& name) const {
  static constexpr const char* kSql =
      "SELECT name, actor, payload, moment, meta "
      "FROM neurons WHERE name = ?1 LIMIT 1;";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(_ctx.handle(), kSql, -1, &stmt, nullptr) !=
      SQLITE_OK) {
    return std::nullopt;
  }

  BindText(stmt, 1, name);
  const int rc = sqlite3_step(stmt);
  if (rc != SQLITE_ROW) {
    sqlite3_finalize(stmt);
    return std::nullopt;
  }

  Neuron out;
  out.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
  out.actor = ReadBlob(stmt, 1);
  out.payload = ReadBlob(stmt, 2);
  out.moment = FromSqlMoment(sqlite3_column_int64(stmt, 3));
  out.meta = ReadOptionalBlob(stmt, 4);

  sqlite3_finalize(stmt);
  return out;
}

std::vector<Neuron> SqliteNeuronTable::find(
    const Moment since,
    const Moment until) const {
  static constexpr const char* kSql =
      "SELECT name, actor, payload, moment, meta "
      "FROM neurons WHERE moment >= ?1 AND moment <= ?2 ORDER BY moment ASC;";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(_ctx.handle(), kSql, -1, &stmt, nullptr) !=
      SQLITE_OK) {
    return {};
  }

  sqlite3_bind_int64(stmt, 1, ToSqlMoment(since));
  sqlite3_bind_int64(stmt, 2, ToSqlMoment(until));

  std::vector<Neuron> out;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Neuron n;
    n.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    n.actor = ReadBlob(stmt, 1);
    n.payload = ReadBlob(stmt, 2);
    n.moment = FromSqlMoment(sqlite3_column_int64(stmt, 3));
    n.meta = ReadOptionalBlob(stmt, 4);
    out.push_back(std::move(n));
  }

  sqlite3_finalize(stmt);
  return out;
}

}
