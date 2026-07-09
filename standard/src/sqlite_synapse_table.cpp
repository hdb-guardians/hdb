#include <sqlite3.h>

#include <hdb/standard/sqlite_synapse_table.hpp>
#include <hdb/standard/sqlite_utils.hpp>

namespace hdb::standard {

SqliteSynapseTable::SqliteSynapseTable(SqliteContext& ctx) : ctx_(ctx) {}

std::optional<Synapse> SqliteSynapseTable::insert(const Synapse& synapse) {
  static constexpr const char* kSql =
      "INSERT INTO synapses(name, actor, from_id, to_id, moment, meta) "
      "VALUES(?1, ?2, ?3, ?4, ?5, ?6);";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(ctx_.handle(), kSql, -1, &stmt, nullptr) !=
      SQLITE_OK) {
    return std::nullopt;
  }

  BindText(stmt, 1, synapse.name);
  BindBlob(stmt, 2, synapse.actor);
  BindText(stmt, 3, synapse.from);
  BindText(stmt, 4, synapse.to);
  sqlite3_bind_int64(stmt, 5, ToSqlMoment(synapse.moment));
  if (synapse.meta.has_value()) {
    BindBlob(stmt, 6, *synapse.meta);
  } else {
    sqlite3_bind_null(stmt, 6);
  }

  const int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc != SQLITE_DONE) {
    return std::nullopt;
  }
  return synapse;
}

std::optional<Synapse> SqliteSynapseTable::find(const Sid& name) const {
  static constexpr const char* kSql =
      "SELECT name, actor, from_id, to_id, moment, meta "
      "FROM synapses WHERE name = ?1 LIMIT 1;";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(ctx_.handle(), kSql, -1, &stmt, nullptr) !=
      SQLITE_OK) {
    return std::nullopt;
  }

  BindText(stmt, 1, name);
  const int rc = sqlite3_step(stmt);
  if (rc != SQLITE_ROW) {
    sqlite3_finalize(stmt);
    return std::nullopt;
  }

  Synapse out;
  out.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
  out.actor = ReadBlob(stmt, 1);
  out.from = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
  out.to = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
  out.moment = FromSqlMoment(sqlite3_column_int64(stmt, 4));
  out.meta = ReadOptionalBlob(stmt, 5);

  sqlite3_finalize(stmt);
  return out;
}

std::vector<Synapse> SqliteSynapseTable::find(
    const Moment since,
    const Moment until) const {
  static constexpr const char* kSql =
      "SELECT name, actor, from_id, to_id, moment, meta "
      "FROM synapses WHERE moment >= ?1 AND moment <= ?2 ORDER BY moment ASC;";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(ctx_.handle(), kSql, -1, &stmt, nullptr) !=
      SQLITE_OK) {
    return {};
  }

  sqlite3_bind_int64(stmt, 1, ToSqlMoment(since));
  sqlite3_bind_int64(stmt, 2, ToSqlMoment(until));

  std::vector<Synapse> out;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Synapse s;
    s.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    s.actor = ReadBlob(stmt, 1);
    s.from = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    s.to = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    s.moment = FromSqlMoment(sqlite3_column_int64(stmt, 4));
    s.meta = ReadOptionalBlob(stmt, 5);
    out.push_back(std::move(s));
  }

  sqlite3_finalize(stmt);
  return out;
}

}
