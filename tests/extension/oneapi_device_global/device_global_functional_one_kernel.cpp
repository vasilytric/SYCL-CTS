/*******************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
//  Provides functional tests for device_global
//  Run one kernel multiple times on one device with the different
//  properies. The test reads a value from device_global instance and write the
//  new value. At the next run it expects to read the same value, that we wrote
//  before.
//
*******************************************************************************/

#include "../../common/common.h"
#include "../../common/type_coverage.h"
#include "device_global_common.h"
#include "type_pack.h"

#define TEST_NAME device_global_functional_one_kernel

namespace TEST_NAMESPACE {
using namespace sycl_cts;
#if defined(SYCL_EXT_ONEAPI_PROPERTY_LIST) && \
    defined(SYCL_EXT_ONEAPI_DEVICE_GLOBAL)
namespace oneapi = sycl::ext::oneapi;
using namespace device_global_common_functions;

namespace one_kernel_multiple_times {
template <typename T, typename prop_value_t>
oneapi::device_global<T, oneapi::property_list<prop_value_t>> dev_global;

template <typename T, property_tag tag>
struct kernel_read_then_write;

/**
 * @brief The class provide static functions to execute read and write
 * operations in the kernel
 */
template <typename T, typename prop_value_t, property_tag tag>
class read_and_write_in_kernel {
 public:
  /**
   * @brief The function reads value from device_global instance and then writes
   * new value in instance. Test will be failed if value from the device_global
   * instance not equal to default value
   */
  static inline void expect_def_val(util::logger& log,
                                    const std::string& type_name) {
    run(true,
        "Value read incorrectly from kernel on first invocation. Default "
        "value expected",
        log, type_name);
  }

  /**
   * @brief The function reads value from device_global instance and then writes
   * new value in instance. Test will be failed if value from device_global
   * instance not equal to T{1}
   */
  static inline void expect_new_val(util::logger& log,
                                    const std::string& type_name) {
    run(false,
        "Value read incorrectly from kernel on second invocation. "
        "Changed value expected",
        log, type_name);
  }

 private:
  /**
   * @brief The function reads value from device_global instance and then
   * writes new value in instance
   * @param expect_def_value Flag that shows if default value expected after
   * read in kernel or modified - T{1} value expected
   * @param error_info String with additional info to display, when test fails
   * @param type_name Name of testing type for display if test fails
   */
  static inline void run(const bool expect_def_value,
                         const std::string& error_info, util::logger& log,
                         const std::string& type_name) {
    // Default value of type T in case if we expect to read default value
    T def_val{};

    // Changed value of type T in case if we expect to read modified value
    T new_val{};
    value_helper<T>::change_val(new_val);

    // is_read_correct will be set to true if device_global value is equal to
    // the expected value inside kernel
    bool is_read_correct{false};
    {
      // Creating result buffer
      sycl::buffer<bool, 1> is_read_corr_buf(&is_read_correct,
                                             sycl::range<1>(1));
      auto queue = util::get_cts_object::queue();
      queue.submit([&](sycl::handler& cgh) {
        using kernel = kernel_read_then_write<T, tag>;
        auto is_read_correct_acc =
            is_read_corr_buf.template get_access<sycl::access_mode::write>(cgh);

        // Kernel that will compare device_global variable with expected and
        // then write new value
        cgh.single_task<kernel>([=] {
          if (expect_def_value) {
            is_read_correct_acc[0] = value_helper<T>::compare_val(
                dev_global<T, prop_value_t>, def_val);
          } else {
            is_read_correct_acc[0] = value_helper<T>::compare_val(
                dev_global<T, prop_value_t>, new_val);
          }
          value_helper<T>::change_val(dev_global<T, prop_value_t>);
        });
      });
      queue.wait_and_throw();
    }
    if (is_read_correct == false) {
      FAIL(log, get_case_description(
                    "device_global: Running one kernel multiple times",
                    error_info, type_name));
    }
  }
};

/**
 * @brief The function tests that the device_global value is correctly read and
 * changed from a single kernel executed multiple times
 * @tparam T Type of underlying device_global value
 * @tparam prop_value_t Type of property_value that included in property_list
 * @tparam tag For kernel naming
 */
template <typename T, typename prop_value_t, property_tag tag>
void run_test(util::logger& log, const std::string& type_name) {
  using VerifierT = read_and_write_in_kernel<T, prop_value_t, tag>;
  // At first run expecting default values
  VerifierT::expect_def_val(log, type_name);

  // At the second run expect changed value
  VerifierT::expect_new_val(log, type_name);
}
/**
 * @brief The function runs a test with properties that can include in
 * device_global property_list
 * @tparam T Type of underlying value in device_global
 */
template <typename T>
void run_tests_with_properties(sycl_cts::util::logger& log,
                               const std::string& type_name) {
  // Using a property_tag for kernel name

  // Run without any properies
  run_test<T, oneapi::property_value<>, property_tag::none>(log, type_name);

  {
    using oneapi::device_image_scope;
    // Run with device_image_scope property
    run_test<T, device_image_scope::value_t, property_tag::dev_image_scope>(
        log, type_name);
  }

  {
    using oneapi::host_access;
    // Run with different host_access properies
    run_test<T, host_access::value_t<host_access::access::read>,
             property_tag::host_access_r>(log, type_name);
    run_test<T, host_access::value_t<host_access::access::write>,
             property_tag::host_access_w>(log, type_name);
    run_test<T, host_access::value_t<host_access::access::read_write>,
             property_tag::host_access_r_w>(log, type_name);
    run_test<T, host_access::value_t<host_access::access::none>,
             property_tag::host_access_none>(log, type_name);
  }

  {
    using oneapi::init_mode;
    // Run with different init_mode properies
    run_test<T, init_mode::value_t<init_mode::trigger::reprogram>,
             property_tag::init_mode_trig_reset>(log, type_name);
    run_test<T, init_mode::value_t<init_mode::trigger::reset>,
             property_tag::init_mode_trig_reprogram>(log, type_name);
  }

  {
    using oneapi::implement_in_csr;
    // Run with different implement_in_csr properies
    run_test<T, implement_in_csr::value_t<true>,
             property_tag::impl_in_csr_true>(log, type_name);
    run_test<T, implement_in_csr::value_t<false>,
             property_tag::impl_in_csr_false>(log, type_name);
  }
}

}  // namespace one_kernel_multiple_times

template <typename T>
class check_device_global_one_kernel_for_type {
 public:
  inline void operator()(sycl_cts::util::logger& log,
                         const std::string& type_name) const {
    using namespace one_kernel_multiple_times;
    // Run tests with T and array T
    run_tests_with_properties<T>(log, type_name);
    run_tests_with_properties<T[5]>(log, type_name);
  }
};
#endif

/** test device_global functional
 */
class TEST_NAME : public sycl_cts::util::test_base {
 public:
  /** return information about this test
   */
  void get_info(test_base::info& out) const override {
    set_test_info(out, TOSTRING(TEST_NAME), TEST_FILE);
  }

  /** execute the test
   */
  void run(util::logger& log) override {
#if !defined(SYCL_EXT_ONEAPI_PROPERTY_LIST)
    WARN("SYCL_EXT_ONEAPI_PROPERTY_LIST is not defined, test is skipped");
#elif !defined(SYCL_EXT_ONEAPI_DEVICE_GLOBAL)
    WARN("SYCL_EXT_ONEAPI_DEVICE_GLOBAL is not defined, test is skipped");
#else
    for_all_types<check_device_global_one_kernel_for_type>(types, log);
#endif
  }
};

// construction of this proxy will register the above test
util::test_proxy<TEST_NAME> proxy;
}  // namespace TEST_NAMESPACE
