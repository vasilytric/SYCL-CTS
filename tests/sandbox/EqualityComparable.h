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
constexpr bool check_equality_comparable_requirement(It valid_iterator) {
  // static_assert(type_traits::has_equality_operator_v<It>);
  
  // Expect to fail a compilation if equal operator is not implemented
  static_cast<void>((valid_iterator == valid_iterator));

  It a = valid_iterator;
  It b = valid_iterator;
  It c = valid_iterator;
  const It const_a = valid_iterator;
  const It const_b = valid_iterator;
  const It const_c = valid_iterator;

  bool result = false;

  {
    bool a_eq_b = a == b;
    bool b_eq_a = b == a;
    bool b_eq_c = b == c;
    bool a_eq_c = a == c;

    result = a_eq_b;
    result &= b_eq_a;
    result &= b_eq_c;
    result &= a_eq_c;

    bool returns_bool = false;
    returns_bool = std::is_same_v<decltype((a == b)), bool>;
    returns_bool &= std::is_same_v<decltype((b == a)), bool>;
    returns_bool &= std::is_same_v<decltype((b == c)), bool>;
    returns_bool &= std::is_same_v<decltype((a == c)), bool>;

    result &= returns_bool;
  }

  {
    bool const_a_eq_const_b = const_a == const_b;
    bool const_b_eq_const_c = const_b == const_c;
    bool const_a_eq_const_c = const_a == const_c;
    bool const_b_eq_const_a = const_b == const_a;

    result &= const_a_eq_const_b;
    result &= const_b_eq_const_c;
    result &= const_a_eq_const_c;
    result &= const_b_eq_const_a;

    bool returns_bool = false;
    returns_bool = std::is_same_v<decltype((const_a == const_b)), bool>;
    returns_bool &= std::is_same_v<decltype((const_b == const_c)), bool>;
    returns_bool &= std::is_same_v<decltype((const_a == const_c)), bool>;
    returns_bool &= std::is_same_v<decltype((const_b == const_a)), bool>;

    result &= returns_bool;
  }

  return result;
}

// class EqualityComparableIterator {
//  public:
//   bool operator==(const EqualityComparableIterator &other) { return false; }
// };
