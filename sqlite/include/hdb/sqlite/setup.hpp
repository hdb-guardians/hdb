#pragma once

#include <hdb/api/context.hpp>
#include <string>

namespace hdb::sqlite {

void append_sqlite_context(
    hdb::api::Context& ctx,
    const std::string& db_path,
    const std::string& sqlite_vec_extension_path = "");

}  // namespace hdb::sqlite
