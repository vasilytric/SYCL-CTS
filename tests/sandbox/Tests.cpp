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

class LegacyIterator {
 public:
  LegacyIterator& operator++() { return *this; };
  LegacyIterator* operator*() { return this; };
};

template <>
struct std::iterator_traits<LegacyIterator> {
  using value_type = int;
  using difference_type = int;
  using pointer = int;
  using reference = int&;
};

TEST_CASE("Equal operator") {
  // Not working as expected!
  std::cout << "Equal operator" << std::endl;
  std::cout << type_traits::has_equality_operator_v<
                   EqualOpImplemented> << std::endl;  // Expect 0
  std::cout << type_traits::has_equality_operator_v<
                   std::vector<int>::iterator> << std::endl;  // Expect 1
  std::cout << "----------------" << std::endl;
}

TEST_CASE("Legacy iterator check") {
  std::cout << "Legacy iterator check" << std::endl;
  std::vector v{1, 2, 3};
  CHECK(check_legacy_iterator_requirement(v.begin()));
  LegacyIterator user_iterator;
  CHECK(check_legacy_iterator_requirement(user_iterator));
  std::cout << "----------------" << std::endl;
}

TEST_CASE("Legacy input iterator check") {
  std::cout << "Legacy iterator check" << std::endl;
  std::vector v{1, 2, 3};
  CHECK(check_legacy_input_iterator_requirement(v.begin()));
  std::cout << "----------------" << std::endl;
}

TEST_CASE("Equality comparable check") {
  std::cout << "Equality comparable check" << std::endl;
  std::vector v{1, 2, 3, 4, 5};
  CHECK(check_equality_comparable_requirement(v.begin()));
  EqualOpImplemented user_class;
  CHECK(check_equality_comparable_requirement(user_class));
  std::cout << "----------------" << std::endl;
}

TEST_CASE("Legacy output iterator check") {
  std::cout << "Legacy output iterator check" << std::endl;
  std::vector v{1, 2, 3, 4, 5};
  CHECK(check_legacy_output_iterator_requirement(v.begin()));
  std::cout << "----------------" << std::endl;
}
