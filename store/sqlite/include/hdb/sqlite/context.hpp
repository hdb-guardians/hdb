#pragma once

#include <cstddef>
#include <string>

struct sqlite3;

namespace hdb::sqlite {

class SqliteContext {
 public:
  explicit SqliteContext(
      const std::string& db_path,
      const std::string& sqlite_vec_extension_path,
      std::size_t dream_dimension = 0);
  ~SqliteContext();

  SqliteContext(const SqliteContext&) = delete;
  SqliteContext& operator=(const SqliteContext&) = delete;

  sqlite3* handle() const noexcept;

  // Number of float32 components in `dreams.payload` vectors. Fixed at
  // schema-creation time because the sqlite-vec `vec0` virtual table
  // requires a static dimension per column.
  std::size_t dream_dimension() const noexcept;

  void initialize_schema();

 private:
  sqlite3* db_ = nullptr;
  std::size_t dream_dimension_ = 0;
};

}  // namespace hdb::sqlite
