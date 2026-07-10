#pragma once

#include <memory>

#include <hdb/sqlite/context.hpp>
#include <hdb/store/dream_table.hpp>

namespace hdb::sqlite {

class SqliteDreamTable : public DreamTable {
 public:
  explicit SqliteDreamTable(std::shared_ptr<SqliteContext> ctx);

  std::optional<Dream> insert(const Dream& dream) override;
  std::vector<Resonance> find(std::span<const std::byte> payload, Natural limit)
      const override;

 private:
  std::shared_ptr<SqliteContext> ctx_;
};

}  // namespace hdb::sqlite
