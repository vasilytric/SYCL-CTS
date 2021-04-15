/*******************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
//  Copyright:	(c) 2017 by Codeplay Software LTD. All Rights Reserved.
//
*******************************************************************************/

#ifndef __SYCLCTS_TESTS_BUFFER_API_COMMON_H
#define __SYCLCTS_TESTS_BUFFER_API_COMMON_H

#include "../common/common.h"

namespace {
using namespace sycl_cts;

/** empty_kernel.
 * Empty kernel, required since command groups
 * are required to have a kernel.
 */
class empty_kernel {
 public:
  void operator()() const {}
};

/*!
@brief used to calculate the ranges based on the dimensionality of the buffer
*/
template <size_t dims>
inline void precalculate(cl::sycl::range<dims> &rangeIn,
                         cl::sycl::range<dims> &rangeOut, size_t &elementsCount,
                         unsigned elementsIn, unsigned elementsOut);

template <>
inline void precalculate<1>(cl::sycl::range<1> &rangeIn,
                            cl::sycl::range<1> &rangeOut, size_t &elementsCount,
                            unsigned elementsIn, unsigned elementsOut) {
  rangeIn = cl::sycl::range<1>(elementsIn);
  rangeOut = cl::sycl::range<1>(elementsOut);
  elementsCount = elementsOut;
}

template <>
inline void precalculate<2>(cl::sycl::range<2> &rangeIn,
                            cl::sycl::range<2> &rangeOut, size_t &elementsCount,
                            unsigned elementsIn, unsigned elementsOut) {
  rangeIn = cl::sycl::range<2>(elementsIn, elementsIn);
  rangeOut = cl::sycl::range<2>(elementsOut, elementsIn);
  elementsCount = (elementsOut * elementsIn);
}

template <>
inline void precalculate<3>(cl::sycl::range<3> &rangeIn,
                            cl::sycl::range<3> &rangeOut, size_t &elementsCount,
                            unsigned elementsIn, unsigned elementsOut) {
  rangeIn = cl::sycl::range<3>(elementsIn, elementsIn, elementsIn);
  rangeOut = cl::sycl::range<3>(elementsOut, elementsIn, elementsIn);
  elementsCount = (elementsOut * elementsIn * elementsIn);
}

/*!
@brief Used to produce and test the reinterpreted buffer denoted by the template
arguments. It does so by using the provided data array as a multidimensional
buffer
@tparam TIn the type of the original buffer
@tparam TOut the type of the reinterpreted buffer
*/
template <typename TIn, typename TOut>
class test_buffer_reinterpret {
 public:
  unsigned elementsIn, elementsOut;

  template <size_t dims>
  void check(TIn* data, util::logger& log) {
    auto rangeIn = util::get_cts_object::range<dims>::get(1, 1, 1);
    auto rangeOut = util::get_cts_object::range<dims>::get(1, 1, 1);
    size_t elementsCount = 0;
    precalculate<dims>(rangeIn, rangeOut, elementsCount, elementsIn,
                       elementsOut);

    cl::sycl::buffer<TIn, dims> a(data, rangeIn);
    auto r = a.template reinterpret<TOut, dims>(rangeOut);

    if (r.get_size() != (elementsCount * sizeof(TOut))) {
      FAIL(log, "Reinterpretation failed! The buffers have different size");
    }
  }
};

/**
 * @brief Test buffer reinterpret without specifying a range
 * @tparam TIn Underlying type of the input buffer
 * @tparam TOut Type to reinterpret to
 * @tparam inputDim Number of input dimensions
 * @tparam outputDim Number of output dimensions
 */
template <typename TIn, typename TOut, int inputDim, int outputDim>
class test_buffer_reinterpret_no_range {
 public:
  static_assert((outputDim == 1) ||
                    ((inputDim == outputDim) && (sizeof(TIn) == sizeof(TOut))),
                "Can only omit range when targetting 1D or when reinterpreting "
                "to a type of same size");

  static void check(TIn* data, const size_t inputElemsPerDim,
                    util::logger& log) {
    cl::sycl::range<inputDim> rangeIn =
        getRange<inputDim>(sizeof(TOut) * inputElemsPerDim / sizeof(TIn));
    cl::sycl::buffer<TIn, inputDim> buf1(data, rangeIn);

    auto buf2 = buf1.template reinterpret<TOut>();

    if (buf2.get_count() != buf1.get_count()) {
      const auto msg = "Reinterpret buffer has wrong count: " +
                       std::to_string(buf2.get_count()) +
                       " != " + std::to_string(buf1.get_count());
      FAIL(log, msg);
    }
    if (buf2.get_size() != buf1.get_size()) {
      const auto msg = "Reinterpret buffer has wrong size: " +
                       std::to_string(buf2.get_size()) +
                       " != " + std::to_string(buf1.get_size());
      FAIL(log, msg);
    }
    if (buf2.get_range() != buf1.get_range()) {
      FAIL(log, "Reinterpret buffer has wrong range");
    }
  }
};

/**
 * @brief Performs the actual check for reinterpreted buffers
 *        without providing a range, specialization for reinterpreting to 1D.
 * @tparam TIn Underlying type of the input buffer
 * @tparam TOut Type to reinterpret to
 * @tparam inputDim Number of input dimensions
 */
template <typename TIn, typename TOut, int inputDim>
class test_buffer_reinterpret_no_range<TIn, TOut, inputDim, 1> {
 public:
  static void check(TIn* data, const size_t inputElemsPerDim,
                    util::logger& log) {
    cl::sycl::range<inputDim> rangeIn =
        getRange<inputDim>(sizeof(TOut) * inputElemsPerDim / sizeof(TIn));
    cl::sycl::buffer<TIn, inputDim> buf1{data, rangeIn};
    const auto expectedOutputCount = buf1.get_size() / sizeof(TOut);

    auto buf2 = buf1.template reinterpret<TOut, 1>();

    if (buf2.get_count() != expectedOutputCount) {
      const auto msg = "Reinterpret buffer has wrong count: " +
                       std::to_string(buf2.get_count()) +
                       " != " + std::to_string(expectedOutputCount);
      FAIL(log, msg);
    }
    if (buf2.get_size() != buf1.get_size()) {
      const auto msg = "Reinterpret buffer has wrong size: " +
                       std::to_string(buf2.get_size()) +
                       " != " + std::to_string(buf1.get_size());
      FAIL(log, msg);
    }
    if (buf2.get_range() != cl::sycl::range<1>{expectedOutputCount}) {
      FAIL(log, "Reinterpret buffer has wrong range");
    }
  }
};

/**
 * @brief Helper class for flipping the signedness of a type.
 *
 * Required because make_signed and similar fail to instantiate
 * for types that don't have an inherent signedness.
 *
 * @tparam T Input type to have its signedness flipped
 * @tparam isIntegral Whether the type is integral or not
 */
template <class T, bool isIntegral>
struct flip_signedness_helper {
  /// Just return the same type
  using type = T;
};

/**
 * @brief Helper class for flipping the signedness of a type,
 *        specialization for integral types.
 * @tparam T
 */
template <class T>
struct flip_signedness_helper<T, true> {
  /// Make the type signed or unsigned based on the input type
  using type =
      typename std::conditional<std::is_signed<T>::value,
                                typename std::make_unsigned<T>::type,
                                typename std::make_signed<T>::type>::type;
};

/**
 * @brief Flips the signedness of the data type, if possible.
 *        Otherwise returns the same type.
 * @tparam T Input type to have its signedness flipped
 */
template <class T>
using flip_signedness_t =
    typename flip_signedness_helper<T, std::is_integral<T>::value>::type;

/**
 * Generic buffer API test function
 */
template <typename T, int size, int dims, typename alloc>
void test_buffer(util::logger &log, cl::sycl::range<dims> &r,
                 cl::sycl::id<dims> &i) {
  try {
    cl::sycl::unique_ptr_class<T[]> data(new T[size]);
    std::fill(data.get(), (data.get() + size), 0);

    // Create a default offset with indices 0.
    cl::sycl::id<dims> offset;

    /* create a SYCL buffer from the host buffer */
    cl::sycl::buffer<T, dims, alloc> buf(data.get(), r);

    /* check the buffer returns a range */
    auto ret_range = buf.get_range();
    check_return_type<cl::sycl::range<dims>>(log, ret_range,
                                             "cl::sycl::buffer::get_range()");

    /* Check alias types */
    {
      {
        check_type_existence<typename cl::sycl::buffer<T, dims>::value_type>
            typeCheck;
        (void)typeCheck;
      }
      {
        check_type_existence<typename cl::sycl::buffer<T, dims>::reference>
            typeCheck;
        (void)typeCheck;
      }
      {
        check_type_existence<
            typename cl::sycl::buffer<T, dims>::const_reference>
            typeCheck;
        (void)typeCheck;
      }
      {
        check_type_existence<typename cl::sycl::buffer<
            T, dims, cl::sycl::buffer_allocator>::allocator_type>
            typeCheck;
        (void)typeCheck;
      }
    }

    /* Check that ret_range is the correct size */
    for (int i = 0; i < dims; ++i) {
      if (ret_range[i] != r[i]) {
        FAIL(log,
             "cl::sycl::buffer::get_range does not return "
             "the correct range size!");
      }
    }

    /* check the buffer returns the correct element count */
    auto count = buf.get_count();
    check_return_type<size_t>(log, count, "cl::sycl::buffer::get_count()");

    if (count != size) {
      FAIL(log,
           "cl::sycl::buffer::get_count() does not return "
           "the correct number of elements");
    }

    /* check the buffer returns the correct byte size */
    auto ret_size = buf.get_size();
    check_return_type<size_t>(log, ret_size, "cl::sycl::buffer::get_size()");

    if (ret_size != size * sizeof(T)) {
      FAIL(log,
           "cl::sycl::buffer::get_size() does not return "
           "the correct size of the buffer");
    }

    auto q = util::get_cts_object::queue();

    /* check the buffer returns the correct type of accessor */
    q.submit([&](cl::sycl::handler& cgh) {
      auto acc =
          buf.template get_access<cl::sycl::access::mode::read_write>(cgh);
      check_return_type<
          cl::sycl::accessor<T, dims, cl::sycl::access::mode::read_write,
                             cl::sycl::access::target::global_buffer>>(
          log, acc, "cl::sycl::buffer::get_access<read_write>(handler&)");
      cgh.single_task(empty_kernel());
    });

    /* check the buffer returns the correct type of accessor */
    q.submit([&](cl::sycl::handler& cgh) {
      auto acc =
          buf.template get_access<cl::sycl::access::mode::read,
                                  cl::sycl::access::target::constant_buffer>(
              cgh);
      check_return_type<
          cl::sycl::accessor<T, dims, cl::sycl::access::mode::read,
                             cl::sycl::access::target::constant_buffer>>(
          log, acc,
          "cl::sycl::buffer::get_access<read, constant_buffer>(handler&)");
      cgh.single_task(empty_kernel());
    });

    /* check the buffer returns the correct type of accessor */
    {
      auto acc = buf.template get_access<cl::sycl::access::mode::read_write>();
      check_return_type<
          cl::sycl::accessor<T, dims, cl::sycl::access::mode::read_write,
                             cl::sycl::access::target::host_buffer>>(
          log, acc, "cl::sycl::buffer::get_access<read_write, host_buffer>()");
    }

    /* check the buffer returns the correct type of accessor */
    q.submit([&](cl::sycl::handler& cgh) {
      auto acc = buf.template get_access<cl::sycl::access::mode::read_write>(
          cgh, r, offset);
      check_return_type<
          cl::sycl::accessor<T, dims, cl::sycl::access::mode::read_write,
                             cl::sycl::access::target::global_buffer>>(
          log, acc,
          "cl::sycl::buffer::get_access<read_write, global_buffer>(handler&, "
          "range<>, id<>)");
      cgh.single_task(empty_kernel());
    });

    /* check the buffer returns the correct type of accessor */
    {
      auto acc = buf.template get_access<cl::sycl::access::mode::read_write>(
          r, offset);
      check_return_type<
          cl::sycl::accessor<T, dims, cl::sycl::access::mode::read_write,
                             cl::sycl::access::target::host_buffer>>(
          log, acc,
          "cl::sycl::buffer::get_access<read_write, host_buffer>(range<>, "
          "id<>)");
    }

    /* check get_allocator() */
    {
      cl::sycl::buffer<T, dims, alloc> bufAlloc(data.get(), r);

      auto allocator = bufAlloc.get_allocator();

      check_return_type<alloc>(log, allocator, "get_allocator()");

      auto ptr = allocator.allocate(1);
      if (ptr == nullptr) {
        FAIL(log, "get_allocator() returned an invalid allocator ");
      }
      allocator.deallocate(ptr, 1);
    }

    /* check is_sub_buffer() */
    {
      cl::sycl::buffer<T, dims> buf(r);
      cl::sycl::range<dims> sub_r = r;
      sub_r[0] = r[0] - i[0];
      cl::sycl::buffer<T, dims> buf_sub(buf, i, sub_r);
      auto isSubBuffer = buf_sub.is_sub_buffer();
      check_return_type<bool>(log, isSubBuffer, "is_sub_buffer()");
    }

    /* check buffer properties */
    {
      cl::sycl::mutex_class mutex;
      auto context = util::get_cts_object::context();
      const cl::sycl::property_list pl{
          cl::sycl::property::buffer::use_mutex(mutex),
          cl::sycl::property::buffer::context_bound(context)};

      cl::sycl::buffer<T, dims> buf(r, pl);

      /* check has_property() */

      auto hasUseMutexProperty =
          buf.template has_property<cl::sycl::property::buffer::use_mutex>();
      check_return_type<bool>(log, hasUseMutexProperty,
                              "has_property<use_mutex>()");

      auto hasContentBoundProperty = buf.template has_property<
          cl::sycl::property::buffer::context_bound>();
      check_return_type<bool>(log, hasContentBoundProperty,
                              "has_property<context_bound>()");

      /* check get_property() */

      auto useMutexProperty =
          buf.template get_property<cl::sycl::property::buffer::use_mutex>();
      check_return_type<cl::sycl::property::buffer::use_mutex>(
          log, useMutexProperty, "get_property<use_mutex>()");
      check_return_type<cl::sycl::mutex_class*>(
          log, useMutexProperty.get_mutex_ptr(),
          "cl::sycl::property::buffer::use_mutex::get_mutex_ptr()");

      auto contentBoundProperty = buf.template get_property<
          cl::sycl::property::buffer::context_bound>();
      check_return_type<cl::sycl::property::buffer::context_bound>(
          log, contentBoundProperty, "get_property<context_bound>()");
      check_return_type<cl::sycl::context>(
          log, contentBoundProperty.get_context(),
          "cl::sycl::property::buffer::context_bound::get_context()");
    }

    q.wait_and_throw();
  } catch (const cl::sycl::exception& e) {
    log_exception(log, e);
    cl::sycl::string_class errorMsg =
        "a SYCL exception was caught: " + cl::sycl::string_class(e.what());
    FAIL(log, errorMsg.c_str());
  }
}

/**
* @brief Tests reinterpreting a buffer
* @tparam T Underlying data type of the input buffer
* @tparam numDims Number of input dimensions
* @param log Logger object
*/
template <typename T, int numDims>
void test_type_reinterpret(util::logger &log) {
  static constexpr size_t inputElemsPerDim = 4;
  std::vector<uint8_t> reinterpretInputData(sizeof(T) * inputElemsPerDim *
                                            numDims);
  using ReinterpretT = flip_signedness_t<T>;

  // Check reinterpreting with a range
  static constexpr auto numElems = inputElemsPerDim * numDims;
  test_buffer_reinterpret<uint8_t, T>{sizeof(T) * numElems, numElems}
      .template check<numDims>(reinterpretInputData.data(), log);

  // Check reinterpreting without a range to 1D
  test_buffer_reinterpret_no_range<uint8_t, T, numDims, 1>::check(
      reinterpretInputData.data(), inputElemsPerDim, log);

  // Check reinterpreting without a range to the same dimension
  test_buffer_reinterpret_no_range<T, ReinterpretT, numDims, numDims>::check(
      reinterpret_cast<T *>(reinterpretInputData.data()), inputElemsPerDim,
      log);
}

template <typename T> class check_buffer_api_for_type {
  template <typename alloc> void check_with_alloc(util::logger &log) {
    const int size = 8;
    cl::sycl::range<1> range1d(size);
    cl::sycl::range<2> range2d(size, size);
    cl::sycl::range<3> range3d(size, size, size);

    cl::sycl::id<1> id1d(2);
    cl::sycl::id<2> id2d(2, 0);
    cl::sycl::id<3> id3d(2, 0, 0);

    test_buffer<T, size, 1, alloc>(log, range1d, id1d);
    test_buffer<T, size * size, 2, alloc>(log, range2d, id2d);
    test_buffer<T, size * size * size, 3, alloc>(log, range3d, id3d);

    /* check reinterpret() */
    test_type_reinterpret<T, 1>(log);
    test_type_reinterpret<T, 2>(log);
    test_type_reinterpret<T, 3>(log);
  }

public:
  void operator()(util::logger &log, const std::string &typeName) {
    log.note("testing: " + typeName);
    check_with_alloc<cl::sycl::buffer_allocator>(log);
    check_with_alloc<std::allocator<T>>(log);
  }
};

} // namespace
#endif // __SYCLCTS_TESTS_BUFFER_API_COMMON_H