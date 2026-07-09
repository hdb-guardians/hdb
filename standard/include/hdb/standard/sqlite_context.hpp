#pragma once

#include <optional>
#include <string>

struct sqlite3;

namespace hdb::standard {

class SqliteContext {
 public:
  explicit SqliteContext(
      const std::string& db_path,
      std::optional<std::string> sqlite_vec_extension_path = std::nullopt);
  ~SqliteContext();

  SqliteContext(const SqliteContext&) = delete;
  SqliteContext& operator=(const SqliteContext&) = delete;

  sqlite3* handle() const noexcept;
  bool has_vec() const noexcept;

  void initialize_schema();

 private:
  sqlite3* db_ = nullptr;
  bool vec_enabled_ = false;
};

}
