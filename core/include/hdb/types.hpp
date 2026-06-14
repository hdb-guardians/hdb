#pragma once

#include <chrono>
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
    time_point<std::chrono::system_clock, std::chrono::microseconds>;
#else
using TimePoint = HDB_TIME_POINT_TYPE;
#endif

#ifndef HDB_SCALAR_TYPE
using Scalar = float;
#else
using Scalar = HDB_SCALAR_TYPE;
#endif

}  // namespace hdb
