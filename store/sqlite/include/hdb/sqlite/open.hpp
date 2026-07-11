#pragma once

#include <memory>
#include <string>
#include <tuple>

#include <hdb/store/dream_table.hpp>
#include <hdb/store/neuron_table.hpp>
#include <hdb/store/synapse_table.hpp>

namespace hdb::sqlite {

std::string resolve_vec_extension_path(const std::string& path);

std::tuple<
    std::shared_ptr<NeuronTable>,
    std::shared_ptr<SynapseTable>,
    std::shared_ptr<DreamTable>>
open_sqlite(
    const std::string& db_path,
    const std::string& sqlite_vec_extension_path = "");

}  // namespace hdb::sqlite
