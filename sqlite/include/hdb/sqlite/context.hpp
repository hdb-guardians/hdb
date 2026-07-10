#pragma once

#include <string>

struct sqlite3;

namespace hdb::sqlite {

class SqliteContext {
 public:
  explicit SqliteContext(
      const std::string& db_path,
      const std::string& sqlite_vec_extension_path);
  ~SqliteContext();

  SqliteContext(const SqliteContext&) = delete;
  SqliteContext& operator=(const SqliteContext&) = delete;

  sqlite3* handle() const noexcept;

  void initialize_schema();

 private:
  sqlite3* db_ = nullptr;
};

}  // namespace hdb::sqlite
