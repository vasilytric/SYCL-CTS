// FIXME remove pragma after moving to cts
#pragma once

#include "common.h"

#include "EqualityComparable.h"
#include "LegacyIterator.h"

template <typename It>
void check_legacy_input_iterator_requirement(It valid_iterator,
                                             const size_t size_of_container,
                                             const std::string& type_name) {
  if (!iterator_verification_common::check_preconditions(
          valid_iterator, size_of_container, type_name)) {
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
    CHECK(type_traits::is_equality_comparable_v<It>);
  }
  {
    if constexpr (type_traits::is_equality_comparable_v<It>) {
      INFO("Iterator not equal operator could be convertible to bool");
      CHECK(type_traits::not_equal_convertible_to_bool<It>());
    }
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
      It j = valid_iterator;
      It i = valid_iterator;

      auto equivalent_value = *j;
      ++j;
      CHECK(std::is_convertible_v<decltype(*i++), decltype(equivalent_value)>);
    } else {
      INFO(
          "Iterator have not implemented prefix increment or equal comparable "
          "operator, so "
          "verification for pre increent and dereference return value has been "
          "skipped");
    }
  }
}
