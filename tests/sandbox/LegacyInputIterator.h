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
bool check_legacy_input_iterator_requirement(It valid_iterator) {
  using reference_t = typename std::iterator_traits<It>::reference;
  using value_t = typename std::iterator_traits<It>::value_type;

  static_assert(!std::is_same_v<It, void>);

  bool result = false;

  bool is_legacy_iterator = check_legacy_iterator_requirement(valid_iterator);
  bool is_equality_comparable =
      check_equality_comparable_requirement(valid_iterator);

  // static_assert(is_legacy_iterator);
  // static_assert(is_equality_comparable)

  result = is_legacy_iterator;
  result &= is_equality_comparable;

  {
    It j = valid_iterator;
    It i = valid_iterator;

    // As Legacy Iterator implements increment operator we can get two not equal
    // iterators
    i = ++i;

    bool i_not_equal_j = (i != j);
    bool returns_bool = std::is_same_v<decltype((i != j)), bool>;

    i_not_equal_j &= !(i == j);
    returns_bool &= std::is_same_v<decltype((i == j)), bool>;

    result &= i_not_equal_j;
    result &= returns_bool;
  }

  // check precondition
  if (type_traits::is_dereferenceable_v<It>) {
    It i = valid_iterator;
    It j = i;
    bool returns_reference = std::is_same_v<decltype(*i), reference_t>;

    // If i == j and (i, j) is in the domain of == then this is equivalent to *j
    returns_reference &= std::is_same_v<decltype(*j), reference_t>;

    // Expect to fail a compilation if not convertible
    value_t x = *i;

    result &= returns_reference;
  }

  if (type_traits::is_dereferenceable_v<It>) {
    // Can't check "iterator->m" or can I ???
  }

  if (type_traits::is_dereferenceable_v<It>) {
    It i = valid_iterator;

    bool returns_reference = std::is_same_v<decltype((++i)), It&>;

    result &= returns_reference;
  }

  {
    It i = valid_iterator;

    // Expect to fail a compilation if not convertible
    (void)i++;
    (void)++i;
  }

  {
    It i = valid_iterator;
    // Expect to fail a compilation if not convertible
    value_t x = *i;
    ++i;
  }

  return result;
}
