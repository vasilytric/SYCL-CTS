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
void check_legacy_iterator_requirement(It valid_iterator,
                                       const size_t size_of_container,
                                       const std::string& type_name) {
  INFO("Verify named requiremnt Legacy Iterator: " + type_name);

  STATIC_CHECK(!std::is_same_v<It, void>);

  if (size_of_container < 2) {
    INFO("Container, that iterator belongs to, have to be at least size of 2");
    CHECK(false);
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
    INFO("Iterator have to have value_type member typedef");
    CHECK(type_traits::has_value_type_field_v<It>);
  }
  {
    INFO("Iterator have to have difference_type member typedef");
    CHECK(type_traits::has_difference_type_field_v<It>);
  }
  {
    INFO("Iterator have to have reference member typedef");
    CHECK(type_traits::has_reference_field_v<It>);
  }
  {
    INFO("Iterator have to have pointer member typedef");
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
