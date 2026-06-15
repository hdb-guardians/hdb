#pragma once

#include <hdb/atom/neuron.hpp>
#include <hdb/types.hpp>

namespace hdb {

struct Thought {
  Neuron neuron;
  Real flux;
};

}  // namespace hdb
