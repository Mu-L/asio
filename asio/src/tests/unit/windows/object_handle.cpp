//
// object_handle.cpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Disable autolinking for unit tests.
#if !defined(BOOST_ALL_NO_LIB)
#define BOOST_ALL_NO_LIB 1
#endif // !defined(BOOST_ALL_NO_LIB)

// Test that header file is self-contained.
#include "asio/windows/object_handle.hpp"

#include "asio/io_context.hpp"
#include "../archetypes/async_result.hpp"
#include "../unit_test.hpp"

//------------------------------------------------------------------------------

// windows_object_handle_compile test
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// The following test checks that all public member functions on the class
// windows::object_handle compile and link correctly. Runtime failures are
// ignored.

namespace windows_object_handle_compile {

void wait_handler(const asio::error_code&)
{
}

void test()
{
#if defined(ASIO_HAS_WINDOWS_OBJECT_HANDLE)
  using namespace asio;
  namespace win = asio::windows;

  try
  {
    io_context ioc;
    const io_context::executor_type ioc_ex = ioc.get_executor();
    archetypes::lazy_handler lazy;
    asio::error_code ec;

    // basic_object_handle constructors.

    win::object_handle handle1(ioc);
    HANDLE native_handle1 = INVALID_HANDLE_VALUE;
#if defined(ASIO_MSVC) && (_MSC_VER < 1910)
    // Skip this on older MSVC due to mysterious ambiguous overload errors.
#else
    win::object_handle handle2(ioc, native_handle1);
#endif

    win::object_handle handle3(ioc_ex);
    HANDLE native_handle2 = INVALID_HANDLE_VALUE;
    win::object_handle handle4(ioc_ex, native_handle2);

    win::object_handle handle5(std::move(handle4));

    win::basic_object_handle<io_context::executor_type> handle6(ioc);
    win::object_handle handle7(std::move(handle6));

    // basic_object_handle operators.

    handle1 = win::object_handle(ioc);
    handle1 = std::move(handle3);
    handle1 = std::move(handle6);

    // I/O object functions.

    win::object_handle::executor_type ex = handle1.get_executor();
    (void)ex;

    // basic_handle functions.

    win::object_handle::lowest_layer_type& lowest_layer
      = handle1.lowest_layer();
    (void)lowest_layer;

    const win::object_handle& handle8 = handle1;
    const win::object_handle::lowest_layer_type& lowest_layer3
      = handle8.lowest_layer();
    (void)lowest_layer3;

    HANDLE native_handle4 = INVALID_HANDLE_VALUE;
    handle1.assign(native_handle4);

    bool is_open = handle1.is_open();
    (void)is_open;

    handle1.close();
    handle1.close(ec);

    win::object_handle::native_handle_type native_handle3
      = handle1.native_handle();
    (void)native_handle3;

    handle1.cancel();
    handle1.cancel(ec);

    // basic_object_handle functions.

    handle1.wait();
    handle1.wait(ec);

    handle1.async_wait(&wait_handler);
    int i1 = handle1.async_wait(lazy);
    (void)i1;
  }
  catch (std::exception&)
  {
  }
#endif // defined(ASIO_HAS_WINDOWS_OBJECT_HANDLE)
}

} // namespace windows_object_handle_compile

//------------------------------------------------------------------------------

ASIO_TEST_SUITE
(
  "windows/object_handle",
  ASIO_COMPILE_TEST_CASE(windows_object_handle_compile::test)
)
