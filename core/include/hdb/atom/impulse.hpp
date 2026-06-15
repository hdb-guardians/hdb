#pragma once

#include <functional>

#include <hdb/atom/synapse.hpp>
#include <hdb/types.hpp>

namespace hdb {

using Impulse = std::function<Real(const Synapse&)>;

}  // namespace hdb
