// FIXME remove pragma after moving to cts
#pragma once

#include "../common/common.h"

#include "EqualityComparable.h"
#include "LegacyIterator.h"
#include "TypeTraits.h"

#include <type_traits>
#include <utility>
#include <vector>

template <typename It>
bool check_legacy_output_iterator_requirement(It valid_iterator) {
  using value_t = typename std::iterator_traits<It>::value_type;

  static_assert(!std::is_same_v<It, void>);

  bool result = false;

  bool is_legacy_iterator = check_legacy_iterator_requirement(valid_iterator);
  result = is_legacy_iterator;

  {
    // Expect to fail a compilation if not convertible
    It r = valid_iterator;
    value_t o = {};
    *r = o;
  }

  {
    It r = valid_iterator;
    bool returns_reference = std::is_same_v<decltype(++r), It&>;
    result &= returns_reference;
  }

  // Just used expression from cppreference: C++ LegacyOutputIterator named
  // requirement
  {
    // Expect to fail a compilation if not convertible
    It r = valid_iterator;
    const It temp = r;
    ++r;
  }

  {
    // Expect to fail a compilation if not convertible
    It r = valid_iterator;
    value_t o = {};
    *r++ = o;
  }

  return result;
}
