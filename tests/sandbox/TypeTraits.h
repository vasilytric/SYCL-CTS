// FIXME remove pragma after moving to cts
#pragma once 

#include <type_traits>

namespace type_traits {
template <typename T, typename = void>
struct is_dereferenceable : std::false_type {};

template <typename T>
struct is_dereferenceable<T, std::void_t<decltype(*std::declval<T>())>>
    : std::true_type {};

template <typename T>
using is_dereferenceable_t = typename is_dereferenceable<T>::type;

template <typename T>
inline constexpr bool is_dereferenceable_v = is_dereferenceable<T>::value;


template <typename T, typename = void>
struct has_equality_operator : std::false_type {};

// Not working as expected!
// vector<int>::iterator returns false
template <typename T>
struct has_equality_operator<T, std::void_t<decltype(std::declval<T>().operator==(std::declval<T>()))>>
    : std::true_type {};

template <typename T>
using has_equality_operator_t = typename has_equality_operator<T>::type;

template <typename T>
inline constexpr bool has_equality_operator_v = has_equality_operator<T>::value;
}  // namespace type_traits
