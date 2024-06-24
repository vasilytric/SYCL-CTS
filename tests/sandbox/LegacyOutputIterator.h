// FIXME remove pragma after moving to cts
#pragma once

#include "common.h"

#include "EqualityComparable.h"
#include "LegacyIterator.h"



template <typename It>
void check_legacy_output_iterator_requirement(It valid_iterator,
                                              const size_t size_of_container,
                                              const std::string type_name) {
  if (!iterator_verification_common::check_preconditions(
          valid_iterator, size_of_container, type_name)) {
    return;
  }

  check_legacy_iterator_requirement(valid_iterator, size_of_container,
                                    type_name);

  {
    INFO("Iterator have to be dereferenceable");
    CHECK(type_traits::is_dereferenceable_v<It>);
  }
  {
    INFO("Iterator have to implement prefix and postfix increment operator");
    CHECK(type_traits::can_pre_increment_v<It>);
    CHECK(type_traits::can_post_increment_v<It>);
  }

  constexpr bool precondition = type_traits::is_dereferenceable_v<It> &&
                                type_traits::can_pre_increment_v<It> &&
                                type_traits::can_post_increment_v<It>;

  if constexpr (precondition) {
    using value_t = typename std::iterator_traits<It>::value_type;

    {
      INFO("Dereferenced iterator have to be assignble with value_t");
      It r = valid_iterator;
      CHECK(std::is_assignable_v<decltype(*r), value_t>);
    }

    {
      INFO(
          "Iterator have to return reference after usage of prefix increment "
          "operator");
      It r = valid_iterator;
      CHECK(std::is_same_v<decltype(++r), It &>);
    }

    {
      INFO("Iterator have to be convertble to const It after increment");
      It r = valid_iterator;
      CHECK(std::is_convertible_v<decltype(++r), const It>);
    }

    {
      INFO(
          "Iterator have to be assignable with value_t after increment and "
          "dereferencing");
      It r = valid_iterator;
      CHECK(std::is_assignable_v<decltype(*r++), value_t>);
    }
    {
      INFO("Iterator have to be assignable with value_t after dereferencing");
      It r = valid_iterator;
      CHECK(std::is_assignable_v<decltype(*r), value_t>);
    }
  }
}
