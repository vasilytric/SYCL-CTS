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
void check_legacy_input_iterator_requirement(It valid_iterator,
                                             const size_t size_of_container,
                                             const std::string& type_name) {
  INFO("Verify named requiremnt Legacy Input Iterator for: " + type_name);
  STATIC_CHECK(!std::is_same_v<It, void>);

  if (size_of_container < 2) {
    INFO("Container, that iterator belongs to, have to be at least size of 2");
    CHECK(false);
    return;
  }

  check_legacy_iterator_requirement(valid_iterator, size_of_container,
                                    type_name);
  check_equality_comparable_requirement(valid_iterator, type_name);

  It j = valid_iterator;
  It i = valid_iterator;

  // As "Legacy Iterator" implements increment operator we can get two not
  // equal iterators
  i = ++i;
  {
    INFO(
        "Two not equal iterators have to return the same value with NOT EQUAL "
        "operator");
    CHECK();
    CHECK((!(i == j)) == (i != j));
  }
  {
    INFO(
        "Two not equal iterators have to return implicit convertble to "
        "bool value with NOT EQUAL operator");
    CHECK(std::is_convertible_v<decltype((i != j)), bool>);
    CHECK(std::is_convertible_v<decltype((i == j)), bool>);
  }

  {
    INFO("Iterator have to be copy comparable with another one");
    CHECK(is_equality_comparable_v<It>);
  }
  {
    if constexpr (is_equality_comparable_v<It>) {
      INFO("Iterator not equal operator could be convertible to bool");
      CHECK(not_equal_convertible_to_bool<It>());
    }
  }
  else {
    INFO(
        "Iterator have not implemented not equal operator, so verification for "
        "not equal return value type has been skipped");
  }
  {
    if constexpr (type_traits::can_pre_increment_v<It>) {
      INFO(
        "Iterator have to implement prefix increment operator and its return "
        "value type is equal to iterator::value_type&");
      CHECK(type_traits::pre_incr_ret_val<It>());
    } else {
      INFO(
          "Iterator have not implemented prefix increment operator, so "
          "verification for pre increent return value type has been skipped");
    }
  }
  {
    INFO("Iterator have to implement postfix increment operator ");
    CHECK(type_traits::can_post_increment_v<It>);
  }
  {
    INFO("Iterator have to implement postfix increment operator ");
    if constexpr (type_traits::can_post_increment_v<It> &&
                  type_traits::is_equality_comparable_v<It>) {
      auto tmp_it = valid_iterator;
      auto equivalent_value = *valid_iterator;
      ++valid_iterator;
      CHECK(*tmp_it++ == equivalent_value);
    } else {
      INFO(
          "Iterator have not implemented prefix increment or equal comparable "
          "operator, so "
          "verification for pre increent and dereference return value has been "
          "skipped");
    }
  }
}
