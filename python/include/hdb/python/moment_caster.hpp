#pragma once

#include <pybind11/pybind11.h>

#include <datetime.h>

#include <chrono>
#include <cmath>

#include <hdb/types.hpp>

namespace pybind11 {
namespace detail {

// hdb::Moment <-> Python datetime.datetime.
template <>
struct type_caster<hdb::Moment> {
 public:
  PYBIND11_TYPE_CASTER(hdb::Moment, const_name("datetime.datetime"));

  bool load(handle src, bool) {
    if (!PyDateTimeAPI) {
      PyDateTime_IMPORT;
    }
    if (!src || !PyDateTime_Check(src.ptr())) {
      return false;
    }

    PyObject* obj = src.ptr();
    const auto ymd =
        std::chrono::year{PyDateTime_GET_YEAR(obj)} /
        std::chrono::month{static_cast<unsigned>(PyDateTime_GET_MONTH(obj))} /
        std::chrono::day{static_cast<unsigned>(PyDateTime_GET_DAY(obj))};
    if (!ymd.ok()) {
      return false;
    }

    auto wall = std::chrono::sys_days{ymd} +
                std::chrono::hours{PyDateTime_DATE_GET_HOUR(obj)} +
                std::chrono::minutes{PyDateTime_DATE_GET_MINUTE(obj)} +
                std::chrono::seconds{PyDateTime_DATE_GET_SECOND(obj)} +
                std::chrono::microseconds{PyDateTime_DATE_GET_MICROSECOND(obj)};

    if (PyDateTime_DATE_GET_TZINFO(obj) != Py_None) {
      const object utcoffset =
          reinterpret_borrow<object>(src).attr("utcoffset")();
      if (!utcoffset.is_none()) {
        const double offset_seconds =
            utcoffset.attr("total_seconds")().cast<double>();
        wall -= std::chrono::microseconds{
            static_cast<long long>(std::llround(offset_seconds * 1e6))};
      }
    }

    value = std::chrono::time_point_cast<hdb::Moment::duration>(wall);
    return true;
  }

  static handle cast(const hdb::Moment& src, return_value_policy, handle) {
    if (!PyDateTimeAPI) {
      PyDateTime_IMPORT;
    }

    const auto days = std::chrono::floor<std::chrono::days>(src);
    const std::chrono::year_month_day ymd{days};
    const std::chrono::hh_mm_ss<hdb::Moment::duration> tod{src - days};

    return PyDateTimeAPI->DateTime_FromDateAndTime(
        static_cast<int>(ymd.year()),
        static_cast<unsigned>(ymd.month()),
        static_cast<unsigned>(ymd.day()),
        static_cast<int>(tod.hours().count()),
        static_cast<int>(tod.minutes().count()),
        static_cast<int>(tod.seconds().count()),
        static_cast<int>(std::chrono::duration_cast<std::chrono::microseconds>(
                             tod.subseconds())
                             .count()),
        PyDateTime_TimeZone_UTC,
        PyDateTimeAPI->DateTimeType);
  }
};

}  // namespace detail
}  // namespace pybind11
