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
using Aid = Id;

#ifndef HDB_TIME_POINT_TYPE
using TimePoint = std::chrono::
    time_point<std::chrono::system_clock, std::chrono::milliseconds>;
#else
using TimePoint = HDB_TIME_POINT_TYPE;
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
