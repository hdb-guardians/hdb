#pragma once

#include <hdb/standard/sqlite_context.hpp>
#include <hdb/store/dream_table.hpp>

namespace hdb::standard {

class SqliteDreamTable : public DreamTable {
 public:
  explicit SqliteDreamTable(SqliteContext& ctx);

  std::optional<Dream> insert(const Dream& dream) override;
  std::vector<Resonance> find(
      std::span<const std::byte> payload,
      const Natural limit) const override;

 private:
  SqliteContext& ctx_;
};

}
