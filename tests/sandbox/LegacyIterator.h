// FIXME remove pragma after moving to cts
#pragma once

#include "common.h"

template <typename It>
void check_legacy_iterator_requirement(It valid_iterator,
                                       const size_t size_of_container,
                                       const std::string& type_name) {
  if (!iterator_verification_common::check_preconditions(
          valid_iterator, size_of_container, type_name)) {
    return;
  }

  {
    INFO("Iterator have to be copy constructble");
    CHECK(std::is_copy_constructible_v<It>);
  }
  {
    INFO("Iterator have to be copy assignble");
    CHECK(std::is_copy_assignable_v<It>);
  }
  {
    INFO("Iterator have to be destrcutble");
    CHECK(std::is_destructible_v<It>);
  }
  {
    INFO("Iterator have to be swappable");
    CHECK(std::is_swappable_v<It>);
  }
  {
    INFO("Iterator have to have value_type member");
    CHECK(type_traits::has_value_type_field_v<It>);
  }
  {
    INFO("Iterator have to have difference_type member");
    CHECK(type_traits::has_difference_type_field_v<It>);
  }
  {
    INFO("Iterator have to have reference member");
    CHECK(type_traits::has_reference_field_v<It>);
  }
  {
    INFO("Iterator have to have pointer member");
    CHECK(type_traits::has_pointer_field_v<It>);
  }
  {
    INFO("Iterator have to implement prefix increment operator");
    CHECK(type_traits::can_pre_increment_v<It>);
    if constexpr (type_traits::can_pre_increment_v<It>) {
      INFO("Have to be return reference after using prefix increment operator");
      It lval = valid_iterator;
      CHECK(std::is_same_v<decltype(++lval), It&>);
    }
  }
  {
    INFO("Iterator have to be dereferenceble");
    CHECK(type_traits::is_dereferenceable_v<It>);
  }
}
