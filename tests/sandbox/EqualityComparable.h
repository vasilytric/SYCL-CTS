// FIXME remove pragma after moving to cts
#pragma once

#include "../common/common.h"

#include "TypeTraits.h"

#include <chrono>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

template <typename It>
void check_equality_comparable_requirement(It valid_iterator,
                                           const std::string& type_name) {
  INFO("Verify named requiremnt Equality Comparable for: " + type_name);

  STATIC_CHECK(!std::is_same_v<It, void>);

  {
    INFO("Have to be able to be compared with equal operator");
    CHECK(type_traits::is_equality_comparable_v<It>);
  }

  // It will delete branch from code in compile time to not fail a compilation
  if constexpr (type_traits::is_equality_comparable_v<It>) {
    It a = valid_iterator;
    It b = valid_iterator;
    It c = valid_iterator;
    const It const_a = valid_iterator;
    const It const_b = valid_iterator;
    const It const_c = valid_iterator;

    {
      INFO(
          "Non-const copies of one object are actually equal during comparing");
      CHECK(a == b);
      CHECK(b == a);
      CHECK(b == c);
      CHECK(a == c);
    }
    {
      INFO(
          "Non-const copies of one object returns convertble to bool value "
          "after compairing");
      CHECK(std::is_convertible_v<decltype((a == b)), bool>);
      CHECK(std::is_convertible_v<decltype((b == a)), bool>);
      CHECK(std::is_convertible_v<decltype((b == c)), bool>);
      CHECK(std::is_convertible_v<decltype((a == c)), bool>);
    }

    {
      INFO("Const copies of one object are actually equal during comparing");
      CHECK(const_a == const_b);
      CHECK(const_b == const_c);
      CHECK(const_a == const_c);
      CHECK(const_b == const_a);
    }
    {
      INFO(
          "Const copies of one object returns convertble to bool value after "
          "compairing");
      CHECK(std::is_convertible_v<decltype((const_a == const_b)), bool>);
      CHECK(std::is_convertible_v<decltype((const_b == const_c)), bool>);
      CHECK(std::is_convertible_v<decltype((const_a == const_c)), bool>);
      CHECK(std::is_convertible_v<decltype((const_b == const_a)), bool>);
    }
  }
}
