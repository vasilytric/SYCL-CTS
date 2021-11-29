/*******************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
//  This test calls sycl::has_kernel_bundle<all bundle_state>(ctx, devices) with
//  zero devices.
//
//  The test verifies that the exception with sycl::errc::invalid was thrown.
//
*******************************************************************************/

#include "../common/common.h"
#include "has_kernel_bundle_zero_devices.h"

#define TEST_NAME has_kernel_bundle_zero_devices_dev

namespace TEST_NAMESPACE {
using namespace sycl_cts;
using namespace sycl_cts::tests::has_kernel_bundle;
using namespace kernels;

class TEST_NAME : public sycl_cts::util::test_base {
 public:
  /** return information about this test
   */
  void get_info(test_base::info &out) const override {
    set_test_info(out, TOSTRING(TEST_NAME), TEST_FILE);
  }

  /** execute the test
   */
  void run(util::logger &log) override {
    sycl::device dev = util::get_cts_object::device();
    sycl::context ctx(dev.get_platform().get_devices());

    check::zero_devices<simple_kernel_descriptor, sycl::bundle_state::input,
                 overload::id::ctx_dev>{}(log, ctx, dev);
    check::zero_devices<simple_kernel_descriptor, sycl::bundle_state::object,
                 overload::id::ctx_dev>{}(log, ctx, dev);
    check::zero_devices<simple_kernel_descriptor, sycl::bundle_state::executable,
                 overload::id::ctx_dev>{}(log, ctx, dev);
  }
};

// construction of this proxy will register the above test
util::test_proxy<TEST_NAME> proxy;

}  // namespace TEST_NAMESPACE
