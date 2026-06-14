#pragma once

#include <cstddef>
#include <functional>
#include <vector>

#include <hdb/types.hpp>
#include <hdb/store/isynapse_table.hpp>
#include <hdb/molecule/hippocampus.hpp>

namespace hdb {

using Impulse = std::function<Scalar(const SynapseRecord&)>;

struct Thought {
  Nid nid;
  Scalar flux;
};

using Imagination = std::vector<Thought>;

struct ImagineParams {
  Nid start_nid;
  std::size_t epochs;
  Scalar creativity;
};

class Cortex {
 public:
  Imagination Imagine(
      const Engram& engram,
      const ImagineParams& params,
      const Impulse& impulse);
};

}  // namespace hdb
