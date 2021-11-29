/*******************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
//  Provides helper types and tool for has_kernel_bundle_tests
//
*******************************************************************************/

#ifndef __SYCLCTS_TESTS_KERNELS_H
#define __SYCLCTS_TESTS_KERNELS_H

#include "../../util/kernel_restrictions.h"
#include "../common/common.h"
#include <stdint.h>

namespace kernels {
using namespace sycl_cts;

struct checkable_kernel {
  using val_type = unsigned long long;
  using accessor_t = sycl::accessor<val_type, 1, sycl::access_mode::read_write,
                                    sycl::target::global_buffer>;
  static constexpr val_type INIT_VAL = 1;
  static constexpr val_type EXPECTED_VAL = 2;
  static constexpr val_type DIFF_VAL = EXPECTED_VAL - INIT_VAL;
  accessor_t m_acc;

  checkable_kernel(accessor_t acc) : m_acc(acc) {}

  void calculate(sycl::item<1> item) const {
    if (item.get_linear_id() == 0) {
      m_acc[0] = EXPECTED_VAL;
    }
  }
};

struct kernel_cpu : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::cpu))]] {
    calculate(id);
  }
};

struct kernel_cpu_descriptor {
  using type = kernel_cpu;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::cpu});
    return restrs;
  }
};

// that kernel and struct using in multiple kernels in one test
struct kernel_cpu_second : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::cpu))]] {
    calculate(id);
  }
};

struct kernel_cpu_descriptor_second {
  using type = kernel_cpu_second;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::cpu});
    return restrs;
  }
};

struct kernel_gpu : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::gpu))]] {
    calculate(id);
  }
};

struct kernel_gpu_descriptor {
  using type = kernel_gpu;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::gpu});
    return restrs;
  }
};

// that kernel and struct using in multiple kernels in one test
struct kernel_gpu_second : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::gpu))]] {
    calculate(id);
  }
};

struct kernel_gpu_descriptor_second {
  using type = kernel_gpu_second;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::gpu});
    return restrs;
  }
};

struct kernel : checkable_kernel {
  void operator()(sycl::item<1> id) const { calculate(id); }
};

struct simple_kernel_descriptor {
  using type = kernel;
  static auto get_restrictions() { return util::kernel_restrictions(); }
};

struct kernel_second : checkable_kernel {
  void operator()(sycl::item<1> id) const { calculate(id); }
};

struct simple_kernel_descriptor_second {
  using type = kernel;
  static auto get_restrictions() { return util::kernel_restrictions(); }
};


struct kernel_accelerator : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::accelerator))]] {
    calculate(id);
  }
};

struct kernel_accelerator_descriptor {
  using type = kernel_accelerator;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::accelerator});
    return restrs;
  }
};

// that kernel and struct using in multiple kernels in one test
struct kernel_accelerator_second : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::accelerator))]] {
    calculate(id);
  }
};

struct kernel_accelerator_descriptor_second {
  using type = kernel_accelerator_second;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::accelerator});
    return restrs;
  }
};

struct kernel_custom : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::custom))]] {
    calculate(id);
  }
};

struct kernel_custom_descriptor {
  using type = kernel_custom;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::custom});
    return restrs;
  }
};

// TODO: sycl::aspect::emulated is not implemented in llvm
// struct kernel_emulated : checkable_kernel {
//   void operator()(sycl::item<1> id) const
//       [[sycl::requires(has(sycl::aspect::emulated))]] {
//   calculate(id);
// }
// };

// struct kernel_emulated_descriptor {
//   using type = kernel_emulated;
//   static auto get_restrictions() {
//     auto restrs =  util::kernel_restrictions();
//     restrs.set_aspects({sycl::aspect::emulated});
//     return restrs;
//   }
// };

// TODO: sycl::aspect::host_debuggable is not implemented in llvm
// struct kernel_host_debuggable : checkable_kernel {
//   void operator()(sycl::item<1> id) const
//       [[sycl::requires(has(sycl::aspect::host_debuggable))]] {
//   calculate(id);
// }
// };

// struct kernel_host_debuggable_descriptor {
//   using type = kernel_host_debuggable;
//   static auto get_restrictions() {
//     auto restrs =  util::kernel_restrictions();
//     restrs.set_aspects({sycl::aspect::host_debuggable});
//     return restrs;
//   }
// };

struct kernel_fp16 : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::fp16))]] {
    calculate(id);
  }
};

struct kernel_fp16_descriptor {
  using type = kernel_fp16;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::fp16});
    return restrs;
  }
};

struct kernel_fp64 : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::fp64))]] {
    calculate(id);
  }
};

struct kernel_fp64_descriptor {
  using type = kernel_fp64;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::fp64});
    return restrs;
  }
};

struct kernel_atomic64 : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::atomic64))]] {
    calculate(id);
  }
};

struct kernel_atomic64_descriptor {
  using type = kernel_atomic64;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::atomic64});
    return restrs;
  }
};

// fp16, fp64, atomic64 kernels without sycl::requires attribute but with
// explicit operations

struct kernel_fp16_no_attr : checkable_kernel {
  void operator()(sycl::item<1> id) const {
    if (id.get_linear_id() == 0) {
      const sycl::half fp = static_cast<sycl::half>(m_acc[0] + DIFF_VAL);
      m_acc[0] = fp;
    }
  }
};

struct kernel_fp16_no_attr_descriptor {
  using type = kernel_fp16_no_attr;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::fp16});
    return restrs;
  }
};

struct kernel_fp64_no_attr : checkable_kernel {
  void operator()(sycl::item<1> id) const {
    if (id.get_linear_id() == 0) {
      const double fp = static_cast<double>(m_acc[0] + DIFF_VAL);
      m_acc[0] = fp;
    }
  }
};

struct kernel_fp64_no_attr_descriptor {
  using type = kernel_fp64_no_attr;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::fp64});
    return restrs;
  }
};

struct kernel_atomic64_no_attr : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::atomic64))]] {
    calculate(id);  // TODO: change this code to use sycl::atomic_ref
    // Looks like atomic_ref is not implemented yet
    // sycl::atomic_ref longAtomic(m_acc[0]);
    // longAtomic.fetchAdd(DIFF_VAL);
  }
};

struct kernel_atomic64_no_attr_descriptor {
  using type = kernel_atomic64_no_attr;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::atomic64});
    return restrs;
  }
};

struct kernel_image : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::image))]] {
    calculate(id);
  }
};

struct kernel_image_descriptor {
  using type = kernel_image;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::image});
    return restrs;
  }
};

struct kernel_online_compiler : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::online_compiler))]] {
    calculate(id);
  }
};

struct kernel_online_compiler_descriptor {
  using type = kernel_online_compiler;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::online_compiler});
    return restrs;
  }
};

struct kernel_online_linker : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::online_linker))]] {
    calculate(id);
  }
};

struct kernel_online_linker_descriptor {
  using type = kernel_online_linker;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::online_linker});
    return restrs;
  }
};

struct kernel_queue_profiling : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::queue_profiling))]] {
    calculate(id);
  }
};

struct kernel_queue_profiling_descriptor {
  using type = kernel_queue_profiling;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::queue_profiling});
    return restrs;
  }
};

struct kernel_usm_device_allocations : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::usm_device_allocations))]] {
    calculate(id);
  }
};

struct kernel_usm_device_allocations_descriptor {
  using type = kernel_usm_device_allocations;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::usm_device_allocations});
    return restrs;
  }
};

struct kernel_usm_host_allocations : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::usm_host_allocations))]] {
    calculate(id);
  }
};

struct kernel_usm_host_allocations_descriptor {
  using type = kernel_usm_host_allocations;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::usm_host_allocations});
    return restrs;
  }
};

struct kernel_usm_atomic_host_allocations : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::usm_atomic_host_allocations))]] {
    calculate(id);
  }
};

struct kernel_usm_atomic_host_allocations_descriptor {
  using type = kernel_usm_atomic_host_allocations;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::usm_atomic_host_allocations});
    return restrs;
  }
};

struct kernel_usm_shared_allocations : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::usm_shared_allocations))]] {
    calculate(id);
  }
};

struct kernel_usm_shared_allocations_descriptor {
  using type = kernel_usm_shared_allocations;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::usm_shared_allocations});
    return restrs;
  }
};

struct kernel_usm_atomic_shared_allocations : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::usm_atomic_shared_allocations))]] {
    calculate(id);
  }
};

struct kernel_usm_atomic_shared_allocations_descriptor {
  using type = kernel_usm_atomic_shared_allocations;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::usm_atomic_shared_allocations});
    return restrs;
  }
};

struct kernel_usm_system_allocations : checkable_kernel {
  void operator()(sycl::item<1> id) const
      [[sycl::requires(has(sycl::aspect::usm_system_allocations))]] {
    calculate(id);
  }
};

struct kernel_usm_system_allocations_descriptor {
  using type = kernel_usm_system_allocations;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_aspects({sycl::aspect::usm_system_allocations});
    return restrs;
  }
};

struct kernel_likely_supported_work_group_size : checkable_kernel {
  [[sycl::reqd_work_group_size(1)]] void operator()(sycl::item<1> id) const {
    calculate(id);
  }
};

struct kernel_likely_supported_work_group_size_descriptor {
  using type = kernel_likely_supported_work_group_size;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_work_group_size(sycl::id<1>{1});
    return restrs;
  }
};

struct kernel_likely_unsupported_work_group_size : checkable_kernel {
  [[sycl::reqd_work_group_size(SIZE_MAX)]] void operator()(
      sycl::item<1> id) const {
    calculate(id);
  }
};

struct kernel_likely_unsupported_work_group_size_descriptor {
  using type = kernel_likely_unsupported_work_group_size;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_work_group_size(sycl::id<1>{SIZE_MAX});
    return restrs;
  }
};

struct kernel_likely_unsupported_sub_group_size : checkable_kernel {
  [[sycl::reqd_sub_group_size(3)]] void operator()(sycl::item<1> id) const {
    calculate(id);
  }
};

struct kernel_likely_unsupported_sub_group_size_descriptor {
  using type = kernel_likely_unsupported_sub_group_size;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_sub_group_size(3);
    return restrs;
  }
};

struct kernel_likely_supported_sub_group_size : checkable_kernel {
  [[sycl::reqd_sub_group_size(32)]] void operator()(sycl::item<1> id) const {
    calculate(id);
  }
};

struct kernel_likely_supported_sub_group_size_descriptor {
  using type = kernel_likely_supported_sub_group_size;
  static auto get_restrictions() {
    auto restrs = util::kernel_restrictions();
    restrs.set_sub_group_size(32);
    return restrs;
  }
};

}  // namespace kernels

#endif  // __SYCLCTS_TESTS_KERNELS_H
