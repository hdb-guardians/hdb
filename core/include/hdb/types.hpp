#pragma once

#include <chrono>
#include <string>

namespace hdb {

using Id = std::string;
using Nid = Id;
using Sid = Id;
using Did = Id;

using Clock = std::chrono::system_clock;
using Moment = std::chrono::time_point<Clock, std::chrono::microseconds>;

using Real = double;

}  // namespace hdb
