#include "../common/common.h"
#include <iostream>
#include <vector>

#include "EqualityComparable.h"
#include "LegacyInputIterator.h"
#include "LegacyIterator.h"
#include "LegacyOutputIterator.h"

class EqualOpImplemented {
 public:
  bool operator==(const EqualOpImplemented&) const { return true; }
};
class EqualOpNotImplemented {
 public:
  bool operator==(const EqualOpImplemented&) = delete;
};

class UserDefinedIterator {
 public:
  std::vector<int> v;
  size_t index = 0;
  UserDefinedIterator(std::initializer_list<int> l) : v(l){};
  UserDefinedIterator& operator++() {
    index++;
    return *this;
  };
  UserDefinedIterator& operator++(int) {
    index++;
    return *this;
  };
  int& operator*() { return v[index]; };
  bool operator==(const UserDefinedIterator& rhs) const {
    return rhs.index == this->index;
  }
  bool operator!=(const UserDefinedIterator& rhs) const {
    return rhs.index != this->index;
  }
};

template <>
struct std::iterator_traits<UserDefinedIterator> {
  using value_type = int;
  using difference_type = int;
  using pointer = int*;
  using reference = int&;
};

TEST_CASE("Equal operator") {
  CHECK_FALSE(type_traits::is_equality_comparable_v<EqualOpNotImplemented>);
  CHECK(type_traits::is_equality_comparable_v<EqualOpImplemented>);
  CHECK(type_traits::is_equality_comparable_v<std::vector<int>::iterator>);
}

TEST_CASE("Legacy iterator check") {
  std::vector v{1, 2, 3};
  check_legacy_iterator_requirement(v.begin(), v.size(),
                                    "vector<int>::iterator");
  UserDefinedIterator user_iterator{1, 2, 3};
  check_legacy_iterator_requirement(user_iterator, 3, "UserDefinedIterator");
}

TEST_CASE("Legacy input iterator check") {
  std::vector v{1, 2, 3};
  check_legacy_input_iterator_requirement(v.begin(), v.size(),
                                          "vector<int>::iterator");
  UserDefinedIterator inst{1, 2, 3};
  check_legacy_input_iterator_requirement(inst, 3, "UserDefinedIterator");
}

TEST_CASE("Equality comparable check") {
  std::vector v{1, 2, 3, 4, 5};
  check_equality_comparable_requirement(v.begin(), "vector<int>::iterator");
}

TEST_CASE("Legacy output iterator check") {
  std::vector v{1, 2, 3, 4, 5};
  check_legacy_output_iterator_requirement(v.begin(), v.size(),
                                           "vector<int>::iterator");
}
