#include <sqlite3.h>

#include <hdb/sqlite/dream_table.hpp>
#include <hdb/sqlite/utils.hpp>
#include <stdexcept>

namespace hdb::sqlite {

SqliteDreamTable::SqliteDreamTable(std::shared_ptr<SqliteContext> ctx)
    : ctx_(std::move(ctx)) {}

std::optional<Dream> SqliteDreamTable::insert(const Dream& dream) {
  static constexpr const char* kSql =
      "INSERT INTO dreams(name, actor, neuron, payload, moment, meta) "
      "VALUES(?1, ?2, ?3, ?4, ?5, ?6);";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(ctx_->handle(), kSql, -1, &stmt, nullptr) !=
      SQLITE_OK) {
    return std::nullopt;
  }

  BindText(stmt, 1, dream.name);
  BindBlob(stmt, 2, dream.actor);
  BindText(stmt, 3, dream.neuron);
  BindBlob(stmt, 4, dream.payload);
  sqlite3_bind_int64(stmt, 5, ToSqlMoment(dream.moment));
  if (dream.meta.has_value()) {
    BindBlob(stmt, 6, *dream.meta);
  } else {
    sqlite3_bind_null(stmt, 6);
  }

  const int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  if (rc != SQLITE_DONE) {
    return std::nullopt;
  }
  return dream;
}

std::vector<Resonance> SqliteDreamTable::find(
    std::span<const std::byte> payload,
    const Natural limit) const {
  static constexpr const char* kSql =
      "SELECT neuron, "
      "1.0 / (1.0 + vec_distance_l2(payload, ?1)) AS fidelity "
      "FROM dreams "
      "ORDER BY vec_distance_l2(payload, ?1) ASC "
      "LIMIT ?2;";

  sqlite3_stmt* stmt = nullptr;
  const int prepare_rc =
      sqlite3_prepare_v2(ctx_->handle(), kSql, -1, &stmt, nullptr);
  if (prepare_rc != SQLITE_OK) {
    throw std::runtime_error(
        "hdb::sqlite::SqliteDreamTable: query prepare failed: sqlite-vec "
        "resonance query");
  }

  BindBlob(stmt, 1, payload);
  sqlite3_bind_int64(stmt, 2, static_cast<sqlite3_int64>(limit));

  std::vector<Resonance> out;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Resonance r;
    r.neuron = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    r.fidelity = static_cast<Real>(sqlite3_column_double(stmt, 1));
    out.push_back(std::move(r));
  }

  sqlite3_finalize(stmt);
  return out;
}

}  // namespace hdb::sqlite
