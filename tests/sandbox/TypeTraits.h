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

template <typename T, typename = void>
struct is_non_equality_comparable : std::false_type {};

template <typename T>
struct is_non_equality_comparable<
    T, std::void_t<decltype(std::declval<T>() != std::declval<T>())>>
    : std::true_type {};

template <typename T>
using is_non_equality_comparable_t =
    typename is_non_equality_comparable<T>::type;

template <typename T>
inline constexpr bool is_non_equality_comparable_v =
    is_non_equality_comparable<T>::value;

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

template <typename T, typename = void>
struct has_iterator_category_field : std::false_type {};

template <typename T>
struct has_iterator_category_field<
    T, std::void_t<typename std::iterator_traits<T>::iterator_category>>
    : std::true_type {};

template <typename T>
using has_iterator_category_field_t = typename has_pointer_field<T>::type;

template <typename T>
inline constexpr bool has_iterator_category_field_v =
    has_pointer_field<T>::value;

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

template <typename T>
typename std::enable_if_t<
    !is_dereferenceable_v<T> && !has_value_type_field_v<T>, bool>
derefer_can_be_convertible_to_value_type() {
  return false;
}

template <typename T>
typename std::enable_if_t<
    is_dereferenceable_v<T> && has_value_type_field_v<T> &&
        std::is_convertible_v<decltype(*std::declval<T>()),
                              typename std::iterator_traits<T>::value_type>,
    bool>
derefer_can_be_convertible_to_value_type() {
  return true;
}

template <typename T>
typename std::enable_if_t<!is_equality_comparable_v<T>, bool>
not_equal_convertible_to_bool() {
  return false;
}

template <typename T>
typename std::enable_if_t<
    is_equality_comparable_v<T> &&
        std::is_convertible_v<decltype(std::declval<T>() != std::declval<T>()),
                              bool>,
    bool>
not_equal_convertible_to_bool() {
  return true;
}

template <typename T>
typename std::enable_if_t<!can_pre_increment_v<T>, bool> pre_incr_ret_val() {
  return false;
}

template <typename T>
typename std::enable_if_t<can_pre_increment_v<T> &&
                              std::is_same_v<decltype(++std::declval<T>()), T&>,
                          bool>
pre_incr_ret_val() {
  return true;
}

template <typename T>
typename std::enable_if_t<!can_post_increment_v<T>, bool> post_incr_ret_val() {
  return false;
}

template <typename T>
typename std::enable_if_t<can_post_increment_v<T> &&
                              std::is_same_v<decltype(std::declval<T>()++), T&>,
                          bool>
post_incr_ret_val() {
  return true;
}

template <typename T>
typename std::enable_if_t<
    !can_post_increment_v<T> &&
        !std::is_convertible_v<decltype(*std::declval<T>()++),
                               typename std::iterator_traits<T>::value_type>,
    bool>
dereferenced_post_incr_ret_val() {
  return false;
}

template <typename T>
typename std::enable_if_t<
    can_post_increment_v<T> &&
        std::is_convertible_v<decltype(*std::declval<T>()++),
                              typename std::iterator_traits<T>::value_type>,
    bool>
dereferenced_post_incr_ret_val() {
  return true;
}

}  // namespace type_traits
