#pragma once

#include <chrono>
#include <cstddef>
#include <string>

namespace hdb {

#ifndef HDB_ID_TYPE
using Id = std::string;
#else
using Id = HDB_ID_TYPE;
#endif

using Nid = Id;
using Sid = Id;
using Did = Id;

#ifndef HDB_CLOCK_TYPE
using Clock = std::chrono::system_clock;
#else
using Clock = HDB_CLOCK_TYPE;
#endif

#ifndef HDB_MOMENT_TYPE
using Moment = Clock::time_point;
#else
using Moment = HDB_MOMENT_TYPE;
#endif

#ifndef HDB_REAL_TYPE
using Real = float;
#else
using Real = HDB_REAL_TYPE;
#endif

#ifndef HDB_NATURAL_TYPE
using Natural = std::size_t;
#else
using Natural = HDB_NATURAL_TYPE;
#endif

}  // namespace hdb
