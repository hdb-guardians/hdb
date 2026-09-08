#include <sqlite3.h>

#include <cstddef>
#include <hdb/sqlite/dream_table.hpp>
#include <hdb/sqlite/utils.hpp>
#include <stdexcept>

namespace hdb::sqlite {

SqliteDreamTable::SqliteDreamTable(std::shared_ptr<SqliteContext> ctx)
    : ctx_(std::move(ctx)) {}

std::optional<Dream> SqliteDreamTable::insert(const Dream& dream) {
  sqlite3* db = ctx_->handle();

  const std::size_t expected_floats = ctx_->dream_dimension();
  if (dream.payload.size() != expected_floats * sizeof(float)) {
    return std::nullopt;
  }

  if (sqlite3_exec(db, "BEGIN;", nullptr, nullptr, nullptr) != SQLITE_OK) {
    return std::nullopt;
  }

  static constexpr const char* kInsertDream =
      "INSERT INTO dreams(name, actor, neuron, moment, meta) "
      "VALUES(?1, ?2, ?3, ?4, ?5);";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db, kInsertDream, -1, &stmt, nullptr) != SQLITE_OK) {
    sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    return std::nullopt;
  }

  BindText(stmt, 1, dream.name);
  BindBlob(stmt, 2, dream.actor);
  BindText(stmt, 3, dream.neuron);
  sqlite3_bind_int64(stmt, 4, ToSqlMoment(dream.moment));
  if (dream.meta.has_value()) {
    BindBlob(stmt, 5, *dream.meta);
  } else {
    sqlite3_bind_null(stmt, 5);
  }

  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  if (rc != SQLITE_DONE) {
    sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    return std::nullopt;
  }

  const sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);

  static constexpr const char* kInsertVec =
      "INSERT INTO dreams_vec(rowid, payload) VALUES(?1, ?2);";

  sqlite3_stmt* vec_stmt = nullptr;
  if (sqlite3_prepare_v2(db, kInsertVec, -1, &vec_stmt, nullptr) != SQLITE_OK) {
    sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    return std::nullopt;
  }

  sqlite3_bind_int64(vec_stmt, 1, rowid);
  BindBlob(vec_stmt, 2, dream.payload);

  rc = sqlite3_step(vec_stmt);
  sqlite3_finalize(vec_stmt);
  if (rc != SQLITE_DONE) {
    sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    return std::nullopt;
  }

  if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
    sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    return std::nullopt;
  }

  return dream;
}

std::vector<Resonance> SqliteDreamTable::find(
    std::span<const std::byte> payload,
    const std::size_t limit) const {
  static constexpr const char* kSql =
      "SELECT d.neuron, "
      "1.0 / (1.0 + v.distance) AS fidelity "
      "FROM dreams_vec AS v "
      "JOIN dreams AS d ON d.rowid = v.rowid "
      "WHERE v.payload MATCH ?1 AND k = ?2 "
      "ORDER BY v.distance ASC;";

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
