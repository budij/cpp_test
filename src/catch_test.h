/****
 * Copyright (c) 2015 - 2017 Advance Engineering Solutions Pty Ltd.
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "catch.hpp"

#define test_method(name, tag)                           TEST_CASE(name, tag)
#define test_section(message)                            SECTION(message)


///////////////////////////////////////////////////////////////////////////////////
// assert macros
#define assert_equal(message, expected, actual)          CHECK(expected == actual)
#define assert_not_equal(message, expected, actual)      CHECK(expected != actual)
#define assert_not_null(message, actual)                 CHECK((void*)nullptr, (void*)(actual))
#define assert_uint64_t_equal(message, expected, actual) CHECK(uint64_t(expected), uint64_t(actual))
#define assert_uint32_t_equal(message, expected, actual) CHECK(uint32_t(expected), uint32_t(actual))
#define assert_enum_equal(message, expected, actual)     assert_uint32_t_equal(message, expected, actual)
#define assert_size_t_equal(message, expected, actual)   CHECK(message, size_t(expected), size_t(actual))
#define assert_is_true(message, actual)                  CHECK((actual))
#define assert_is_false(message, actual)                 CHECK_FALSE((actual))
#define assert_pass(message)                             SUCCEED(message)
#define assert_fail(message)                             FAIL(message)
#define assert_string_empty(message, actual)             assert_equal(message, std::string(), actual)
#define assert_vector_equal(message, expected, actual)   \
   assert_size_t_equal(std::string(message) + std::string(": Size equals"), expected.size(), actual.size()); \
   if (expected.size(), actual.size())                   \
   {                                                     \
      for (size_t i = 0; i < expected.size(); i++)       \
      {                                                  \
         assert_equal(message, expected[i], actual[i]);  \
      }                                                  \
   }
#define assert_vector_empty(message, actual)             assert_size_t_equal(message, 0, actual.size())
#define assert_ptr_equal(message, expected, actual)      assert_equal(message, ((void*)expected), ((void*)actual))
#define assert_ptr_null(message, actual)                 assert_ptr_equal(message, nullptr, actual)
#define assert_ptr_not_equal(message, expected, actual)  assert_not_equal(message, ((void*)expected), ((void*)actual))
#define assert_ptr_not_null(message, actual)             assert_ptr_not_equal(message, nullptr, actual)


///////////////////////////////////////////////////////////////////////////////////
// require macros
#define require_equal(message, expected, actual)          REQUIRE(expected == actual)
#define require_not_equal(message, expected, actual)      REQUIRE(expected != actual)
#define require_not_null(message, actual)                 REQUIRE((void*)nullptr, (void*)(actual))
#define require_uint64_t_equal(message, expected, actual) REQUIRE(uint64_t(expected), uint64_t(actual))
#define require_uint32_t_equal(message, expected, actual) REQUIRE(uint32_t(expected), uint32_t(actual))
#define require_enum_equal(message, expected, actual)     require_uint32_t_equal(message, expected, actual)
#define require_size_t_equal(message, expected, actual)   REQUIRE(message, size_t(expected), size_t(actual))
#define require_is_true(message, actual)                  REQUIRE((actual))
#define require_is_false(message, actual)                 REQUIRE_FALSE((actual))
#define require_pass(message)                             SUCCEED(message)
#define require_fail(message)                             FAIL(message)
#define require_string_empty(message, actual)             require_equal(message, std::string(), actual)
#define require_vector_equal(message, expected, actual)   \
   require_size_t_equal(std::string(message) + std::string(": Size equals"), expected.size(), actual.size()); \
   if (expected.size(), actual.size())                    \
   {                                                      \
      for (size_t i = 0; i < expected.size(); i++)        \
      {                                                   \
         require_equal(message, expected[i], actual[i]);  \
      }                                                   \
   }
#define require_vector_empty(message, actual)             require_size_t_equal(message, 0, actual.size())
#define require_ptr_equal(message, expected, actual)      require_equal(message, ((void*)expected), ((void*)actual))
#define require_ptr_null(message, actual)                 require_ptr_equal(message, nullptr, actual)
#define require_ptr_not_equal(message, expected, actual)  require_not_equal(message, ((void*)expected), ((void*)actual))
#define require_ptr_not_null(message, actual)             require_ptr_not_equal(message, nullptr, actual)


 ///////////////////////////////////////////////////////////////////////////////////
 // useful macros
#define array_size(test_array, struct_type)              (sizeof(test_array) / sizeof(struct_type))
