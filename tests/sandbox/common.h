// FIXME remove pragma after moving to cts
#pragma once

#include "../common/common.h"

#include "TypeTraits.h"

#include <chrono>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

namespace iterator_verification_common {

template <typename It>
bool check_preconditions(It valid_iterator, const size_t size_of_container,
                         const std::string& type_name) {
  INFO("Verify named requiremnt Legacy Input Iterator for: " + type_name);
  STATIC_CHECK(!std::is_same_v<It, void>);

  if (size_of_container < 2) {
    INFO("Container, that iterator belongs to, have to be at least size of 2");
    CHECK(false);
    return false;
  }
  return true;
}

}  // namespace iterator_verification_common
