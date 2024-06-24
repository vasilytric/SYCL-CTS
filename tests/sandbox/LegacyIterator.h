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
    INFO("Iterator have to have iterator_category member");
    CHECK(type_traits::has_iterator_category_field_v<It>);
  }
  {
    INFO("Iterator have to implement prefix increment operator");
    CHECK(type_traits::can_pre_increment_v<It>);
  }
  {
    INFO(
        "Iterator have to implement prefix increment operator and its return "
        "value is equal to expected one");
    if constexpr (type_traits::can_pre_increment_v<It>) {
      CHECK(type_traits::pre_incr_ret_val<It>());
    } else {
      INFO(
          "Iterator have not implemented prefix increment operator, so "
          "verification for pre increent return value type has been skipped");
    }
  }
  {
    INFO("Iterator have to be dereferenceble");
    CHECK(type_traits::is_dereferenceable_v<It>);
  }
}
