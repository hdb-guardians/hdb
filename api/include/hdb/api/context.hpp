#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace hdb::api {

class Context {
 public:
  template <typename T>
  void put(std::shared_ptr<T> value) {
    entries_[std::type_index(typeid(T))] =
        std::static_pointer_cast<void>(std::move(value));
  }

  template <typename T>
  std::shared_ptr<T> get() const {
    const auto it = entries_.find(std::type_index(typeid(T)));
    if (it == entries_.end()) {
      throw std::out_of_range{
          std::string{"hdb::api::Context: entry not found: "} +
          typeid(T).name()};
    }
    return std::static_pointer_cast<T>(it->second);
  }

  template <typename T>
  bool has() const {
    return entries_.contains(std::type_index(typeid(T)));
  }

 private:
  std::unordered_map<std::type_index, std::shared_ptr<void>> entries_;
};

}
