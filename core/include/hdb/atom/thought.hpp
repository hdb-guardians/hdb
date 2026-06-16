#pragma once

#include <hdb/quark/neuron.hpp>
#include <hdb/types.hpp>

namespace hdb {

struct Thought {
  Neuron neuron;
  Real flux;
};

}  // namespace hdb
