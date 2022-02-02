/*******************************************************************************
//
//  SYCL 2020 Extension Conformance Test
//
//  Provides test for compile-time property_list::has_property
//
*******************************************************************************/

#include "../../common/common.h"

#define TEST_NAME property_list_has_property

namespace TEST_NAMESPACE {

using namespace sycl_cts;

/** test compile-time property_list::has_property
 */
class TEST_NAME : public util::test_base {
 public:
  /** return information about this test
   */
  void get_info(test_base::info &out) const override {
    set_test_info(out, TOSTRING(TEST_NAME), TEST_FILE);
  }

  /** execute the test
   */
  void run(util::logger &log) override {
#if !defined(SYCL_EXT_ONEAPI_PROPERTY_LIST)
    WARN("SYCL_EXT_ONEAPI_PROPERTY_LIST is not defined, test is skipped");
#elif !defined(SYCL_EXT_ONEAPI_DEVICE_GLOBAL)
    WARN("SYCL_EXT_ONEAPI_DEVICE_GLOBAL is not defined, test is skipped");
#else
    {
      using namespace sycl::ext::oneapi;
      property_list prop_list{device_image_scope_v, implement_in_csr_v<true>,
                              host_access_v<host_access::access::read>};

      if (!prop_list.has_property<device_image_scope>())
        FAIL(log, "property_list should have device_image_scope property");
      if (!prop_list.has_property<implement_in_csr>())
        FAIL(log, "property_list should have implement_in_csr property");
      if (!prop_list.has_property<host_access>())
        FAIL(log, "property_list should have host_access property");

      if (prop_list.has_property<init_mode>())
        FAIL(log, "property_list shouldn't have init_mode property");
    }
#endif
  }
};

// register this test with the test_collection.
util::test_proxy<TEST_NAME> proxy;

} /* namespace TEST_NAMESPACE */
