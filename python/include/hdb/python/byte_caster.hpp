#pragma once

#include <pybind11/pybind11.h>

#include <cstddef>
#include <cstring>
#include <vector>

namespace pybind11 {
namespace detail {

// std::vector<std::byte> <-> Python bytes
template <>
struct type_caster<std::vector<std::byte>> {
 public:
  PYBIND11_TYPE_CASTER(std::vector<std::byte>, const_name("bytes"));

  bool load(handle src, bool) {
    if (!isinstance<bytes>(src)) {
      return false;
    }
    const char* buf = nullptr;
    Py_ssize_t len = 0;
    if (PyBytes_AsStringAndSize(src.ptr(), const_cast<char**>(&buf), &len) <
        0) {
      return false;
    }
    value.resize(static_cast<std::size_t>(len));
    if (len > 0) {
      std::memcpy(value.data(), buf, static_cast<std::size_t>(len));
    }
    return true;
  }

  static handle
  cast(const std::vector<std::byte>& src, return_value_policy, handle) {
    return PyBytes_FromStringAndSize(
        reinterpret_cast<const char*>(src.data()),
        static_cast<Py_ssize_t>(src.size()));
  }
};

}  // namespace detail
}  // namespace pybind11
