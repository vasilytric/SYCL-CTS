#include <iterator>
#include <type_traits>
#include <utility>

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
}  // namespace type_traits

template <typename It>
bool check_legacy_iterator_requirement() {
  static_cast(!std::is_same_v<It, void>);

  bool result = false;

  bool is_copy_constructible = std::is_copy_constructible_v<It>;
  bool is_copy_assignable = std::is_copy_assignable_v<It>;
  bool is_destructible = std::is_destructible_v<It>;
  bool is_swappable = std::is_swappable_v<It>;

  result = is_copy_constructible;
  result &= is_copy_assignable;
  result &= is_destructible;
  result &= is_swappable;

  // Expect to fail a compilation if iterator has no member typedefs (?)
  using iter_tr = std::iterator_traits<It>;
  typename iter_tr::value_type;
  typename iter_tr::difference_type;
  typename iter_tr::reference;
  typename iter_tr::pointer;

  auto lval = It();
  bool incrementable_lval = std::is_same_v<decltype(++lval), It&>;
  bool incrementable_rval = std::is_same_v<++(std::declval<It>()), It&>;
  result &= incrementable_lval;
  result &= incrementable_rval;

  bool is_dereferenceable = type_traits::is_dereferenceable_v<It>;
  result &= is_dereferenceable;

  return is_dereferenceable;
}
