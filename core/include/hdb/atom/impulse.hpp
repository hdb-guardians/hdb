#pragma once

#include <hdb/quark/synapse.hpp>

namespace hdb {

using Impulse = std::function<Real(const Synapse&)>;

}  // namespace hdb
