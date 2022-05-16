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
struct is_equality_comparable : std::false_type {};

template <typename T>
struct is_equality_comparable<
    T, std::void_t<decltype(std::declval<T>() == std::declval<T>())>>
    : std::true_type {};

template <typename T>
using is_equality_comparable_t = typename is_equality_comparable<T>::type;

template <typename T>
inline constexpr bool is_equality_comparable_v =
    is_equality_comparable<T>::value;
////////////////////////////////////////////////////
template <typename T, typename = void>
struct has_value_type_field : std::false_type {};

template <typename T>
struct has_value_type_field<
    T, std::void_t<typename std::iterator_traits<T>::value_type>>
    : std::true_type {};

template <typename T>
using has_value_type_field_t = typename has_value_type_field<T>::type;

template <typename T>
inline constexpr bool has_value_type_field_v = has_value_type_field<T>::value;

template <typename T, typename = void>
struct has_difference_type_field : std::false_type {};

template <typename T>
struct has_difference_type_field<
    T, std::void_t<typename std::iterator_traits<T>::difference_type>>
    : std::true_type {};

template <typename T>
using has_difference_type_field_t = typename has_difference_type_field<T>::type;

template <typename T>
inline constexpr bool has_difference_type_field_v =
    has_difference_type_field<T>::value;

template <typename T, typename = void>
struct has_reference_field : std::false_type {};

template <typename T>
struct has_reference_field<
    T, std::void_t<typename std::iterator_traits<T>::reference>>
    : std::true_type {};

template <typename T>
using has_reference_field_t = typename has_reference_field<T>::type;

template <typename T>
inline constexpr bool has_reference_field_v = has_reference_field<T>::value;

template <typename T, typename = void>
struct has_pointer_field : std::false_type {};

template <typename T>
struct has_pointer_field<T,
                         std::void_t<typename std::iterator_traits<T>::pointer>>
    : std::true_type {};

template <typename T>
using has_pointer_field_t = typename has_pointer_field<T>::type;

template <typename T>
inline constexpr bool has_pointer_field_v = has_pointer_field<T>::value;
////////////////////////////////////////////////////

template <typename T, typename = void>
struct can_pre_increment : std::false_type {};

template <typename T>
struct can_pre_increment<T, std::void_t<decltype(++std::declval<T>())>>
    : std::true_type {};

template <typename T>
using can_pre_increment_t = typename can_pre_increment<T>::type;

template <typename T>
inline constexpr bool can_pre_increment_v = can_pre_increment<T>::value;

template <typename T, typename = void>
struct can_post_increment : std::false_type {};

template <typename T>
struct can_post_increment<T, std::void_t<decltype(std::declval<T>()++)>>
    : std::true_type {};

template <typename T>
using can_post_increment_t = typename can_post_increment<T>::type;

template <typename T>
inline constexpr bool can_post_increment_v = can_post_increment<T>::value;
}  // namespace type_traits
