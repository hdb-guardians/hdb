#pragma once

#include <sqlite3.h>

#include <chrono>
#include <cstring>
#include <hdb/types.hpp>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace hdb::sqlite {

inline sqlite3_int64 ToSqlMoment(const Moment moment) {
  return static_cast<sqlite3_int64>(moment.time_since_epoch().count());
}

inline Moment FromSqlMoment(const sqlite3_int64 moment_ticks) {
  return Moment{Clock::duration{moment_ticks}};
}

inline void
BindText(sqlite3_stmt* stmt, const int index, const std::string& s) {
  sqlite3_bind_text(
      stmt, index, s.c_str(), static_cast<int>(s.size()), SQLITE_TRANSIENT);
}

inline void BindBlob(
    sqlite3_stmt* stmt,
    const int index,
    std::span<const std::byte> bytes) {
  if (bytes.empty()) {
    sqlite3_bind_blob(stmt, index, "", 0, SQLITE_TRANSIENT);
    return;
  }

  sqlite3_bind_blob(
      stmt,
      index,
      bytes.data(),
      static_cast<int>(bytes.size()),
      SQLITE_TRANSIENT);
}

inline std::vector<std::byte> ReadBlob(sqlite3_stmt* stmt, const int col) {
  const int size = sqlite3_column_bytes(stmt, col);
  std::vector<std::byte> out(static_cast<std::size_t>(size));
  if (size <= 0) {
    return out;
  }

  const void* blob = sqlite3_column_blob(stmt, col);
  if (blob == nullptr) {
    return {};
  }

  std::memcpy(out.data(), blob, static_cast<std::size_t>(size));
  return out;
}

inline std::optional<std::vector<std::byte>> ReadOptionalBlob(
    sqlite3_stmt* stmt,
    const int col) {
  if (sqlite3_column_type(stmt, col) == SQLITE_NULL) {
    return std::nullopt;
  }
  return ReadBlob(stmt, col);
}

}  // namespace hdb::sqlite
