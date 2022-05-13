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
constexpr bool check_legacy_iterator_requirement(It valid_iterator) {
  static_assert(!std::is_same_v<It, void>);

  bool result = false;

  bool is_copy_constructible = std::is_copy_constructible_v<It>;
  bool is_copy_assignable = std::is_copy_assignable_v<It>;
  bool is_destructible = std::is_destructible_v<It>;
  bool is_swappable = std::is_swappable_v<It>;

  result = is_copy_constructible;
  result &= is_copy_assignable;
  result &= is_destructible;
  result &= is_swappable;

  // Expect to fail a compilation if iterator has no member typedefs (?)
  using iter_tr = std::iterator_traits<std::remove_reference_t<It>>;
  typename iter_tr::value_type value_type;
  typename iter_tr::difference_type difference_type;
  // typename iter_tr::reference reference; // 'reference' declared as
  // reference but not initialized
  typename iter_tr::pointer pointer;

  It lval = valid_iterator;
  // Expect to fail a compilation if iterator has no increment operator
  bool incrementable_lval = std::is_same_v<decltype(++lval), It&>;
  //   bool incrementable_rval = std::is_same_v<++(std::declval<It>()), It&>;
  result &= incrementable_lval;
  //   result &= incrementable_rval;

  bool is_dereferenceable = type_traits::is_dereferenceable_v<It>;
  result &= is_dereferenceable;

  return result;
}
