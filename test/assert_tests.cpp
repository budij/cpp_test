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
#include "unit_test.h"

using namespace aes::test;
using namespace aes::test::log;
using my_logger = logger_base<std::stringstream, std::stringstream>;
using my_assert = assert_base<my_logger>;

namespace
{
   const std::string expected_file_name = "assert_tests.cpp";
}

test_method(assert_constructor_tests, "Testing the assert constructor")
{
   test_section("Testing the normal constructor")
   {
      std::stringstream out;
      std::stringstream error;
      my_logger log(out, error);
      my_assert a(log);
      assert_equal("Total number of passed test is still 0", uint64_t(0), a.passed());
      assert_equal("Total number of failed test is still 0", uint64_t(0), a.failed());
      assert_equal("Total number of tests is still 0", uint64_t(0), a.total());
   }
}

namespace
{
   template <typename T>
   struct assert_test_struct
   {
      T input1_;
      T input2_;
      std::string message_;
      bool expected_;
      uint64_t passed_;
      uint64_t failed_;
      uint64_t total_;
   };

   std::vector<assert_test_struct<std::string>> equal_assert_inputs =
   {
      { "", "", "Same empty string, the assert should be successful", true, 1, 0, 1 },
      { "abc", "", "Assert contains different strings", false, 0, 1, 1 },
      { "abc", "ABC", "Assert contains different strings", false, 0, 1, 1 },
      { "ABC", "ABC", "Assert contains same strings", true, 1, 0, 1 },
   };
   std::vector<assert_test_struct<std::string>> not_equal_assert_inputs =
   {
      { "", "", "Same empty string, the assert should be successful", false, 0, 1, 1 },
      { "abc", "", "Assert contains different strings", true, 1, 0, 1 },
      { "abc", "ABC", "Assert contains different strings", true, 1, 0, 1 },
      { "ABC", "ABC", "Assert contains same strings", false, 0, 1, 1 },
   };
}

test_method_list(equal_assert_tests, "Testing the equal assert", assert_test_struct<std::string>, equal_assert_inputs)
{
   std::stringstream out;
   std::stringstream error;
   my_logger log(out, error);
   my_assert a(log);

   assert_equal(input.message_, input.expected_, a.equal(__FILE__, __LINE__, "message", input.input1_, input.input2_));
   assert_equal("Total for passed is correct", input.passed_, a.passed());
   assert_equal("Total for failed is correct", input.failed_, a.failed());
   assert_equal("Total for total is correct", input.total_, a.total());
}

test_method_list(not_equal_assert_tests, "Testing the not equal assert", assert_test_struct<std::string>, not_equal_assert_inputs)
{
   std::stringstream out;
   std::stringstream error;
   my_logger log(out, error);
   my_assert a(log);

   assert_equal(input.message_, input.expected_, a.not_equal(__FILE__, __LINE__, "message", input.input1_, input.input2_));
   assert_equal("Total for passed is correct", input.passed_, a.passed());
   assert_equal("Total for failed is correct", input.failed_, a.failed());
   assert_equal("Total for total is correct", input.total_, a.total());
}

test_method(assert_tests, "Testing the assert methods")
{
   test_section("Testing the generic_assert method")
   {
      std::stringstream out;
      std::stringstream error;
      my_logger log(out, error);
      my_assert a(log);

      int current_line = __LINE__;
      std::string message("Test generic assert failed");
      std::string assert_type("My type");
      std::string expected("expected");
      std::string actual("actual");
      std::stringstream ss;
      ss << "FAIL " << expected_file_name << " "
                    << current_line << " "
                    << assert_type << ": "
                    << message << ". Expected: " 
                    << expected << ". Actual: "
                    << actual << "." << std::endl;

      assert_is_false("Calling the generic assert with failed comparison", a.generic(__FILE__,
                                                                                     current_line,
                                                                                     assert_type,
                                                                                     message,
                                                                                     expected,
                                                                                     actual,
                                                                                     [](const std::string& expected, const std::string& actual) { return expected == actual; }));
      assert_string_empty("Output is still empty", out.str());
      assert_equal("Error string is correct", ss.str(), error.str());
      assert_uint64_t_equal("Passed count is still 0", 0, a.passed());
      assert_uint64_t_equal("Failed count is now 1", 1, a.failed());
      assert_uint64_t_equal("Total count is now 1", 1, a.total());
   }
   test_section("Testing the generic_assert with just 'actual' method")
   {
      std::stringstream out;
      std::stringstream error;
      my_logger log(out, error, level::verbose);
      my_assert a(log);

      int current_line = __LINE__;
      std::string message("Test generic assert successful");
      std::string assert_type("Another type");
      std::string actual("actual");
      std::stringstream ss;
      ss << "PASS " << expected_file_name << " "
                    << current_line << " "
                    << assert_type << ": "
                    << message << "." << std::endl;

      assert_is_true("Calling the generic assert with just 'actual' specified", a.generic(__FILE__,
                                                                                          current_line,
                                                                                          assert_type,
                                                                                          message,
                                                                                          actual,
                                                                                          [](const std::string& actual) { return !actual.empty(); }));
      assert_equal("Output string is correct", ss.str(), out.str());
      assert_string_empty("Error string is empty", error.str());
      assert_uint64_t_equal("Passed count is now 1", 1, a.passed());
      assert_uint64_t_equal("Failed count is still 0", 0, a.failed());
      assert_uint64_t_equal("Total count is now 1", 1, a.total());
   }
   test_section("Testing the generic_assert with just 'actual' method that fails")
   {
      std::stringstream out;
      std::stringstream error;
      my_logger log(out, error, level::verbose);
      my_assert a(log);

      int current_line = __LINE__;
      std::string message("Test generic assert failed");
      std::string assert_type("Another type");
      std::string actual("actual");
      std::stringstream ss;
      ss << "FAIL " << expected_file_name << " "
                    << current_line << " "
                    << assert_type << ": "
                    << message << ". Actual: "
                    << actual << "." << std::endl;

      assert_is_false("Calling the generic assert with just 'actual' specified", a.generic(__FILE__,
                                                                                           current_line,
                                                                                           assert_type,
                                                                                           message,
                                                                                           actual,
                                                                                           [](const std::string& actual) { return actual.empty(); }));
      assert_string_empty("Output string is empty", out.str());
      assert_equal("Error string is correct", ss.str(), error.str());
      assert_uint64_t_equal("Passed count is now 1", 0, a.passed());
      assert_uint64_t_equal("Failed count is still 0", 1, a.failed());
      assert_uint64_t_equal("Total count is now 1", 1, a.total());
   }
   test_section("Testing the pass method without verbose option")
   {
      std::stringstream out;
      std::stringstream error;
      my_logger log(out, error);
      my_assert a(log);

      int current_line = __LINE__;
      std::string message("Test passed");
      assert_is_true("Calling pass method will return true", a.pass(__FILE__, current_line, message));
      assert_string_empty("Output string is correct", out.str());
      assert_string_empty("Error string is empty", error.str());
      assert_uint64_t_equal("Passed count is now 1", 1, a.passed());
      assert_uint64_t_equal("Failed count is still 0", 0, a.failed());
      assert_uint64_t_equal("Total count is now 1", 1, a.total());
   }
   test_section("Testing the pass method with verbose option")
   {
      std::stringstream out;
      std::stringstream error;
      my_logger log(out, error, level::verbose);
      my_assert a(log);

      int current_line = __LINE__;
      std::string message("Test passed");
      std::stringstream ss;
      ss << "PASS " << expected_file_name << " " << current_line << " Assert passed logged with message: " << message << "." << std::endl;

      assert_is_true("Calling pass method will return true", a.pass(__FILE__, current_line, message));
      assert_equal("Output string is correct", ss.str(), out.str());
      assert_string_empty("Error string is empty", error.str());
      assert_uint64_t_equal("Passed count is now 1", 1, a.passed());
      assert_uint64_t_equal("Failed count is still 0", 0, a.failed());
      assert_uint64_t_equal("Total count is now 1", 1, a.total());
   }
   test_section("Testing the fail method")
   {
      std::stringstream out;
      std::stringstream error;
      my_logger log(out, error);
      my_assert a(log);

      int current_line = __LINE__;
      std::string message("Test failed");
      std::stringstream ss;
      ss << "FAIL " << expected_file_name << " " << current_line << " Assert failed logged with message: " << message << "." << std::endl;

      assert_is_false("Calling fail method will return false", a.fail(__FILE__, current_line, message));
      assert_string_empty("Output is still empty", out.str());
      assert_equal("Error string is correct", ss.str(), error.str());
      assert_uint64_t_equal("Passed count is still 0", 0, a.passed());
      assert_uint64_t_equal("Failed count is now 1", 1, a.failed());
      assert_uint64_t_equal("Total count is now 1", 1, a.total());
   }
   test_section("Testing the assert vector with different size")
   {
      std::stringstream out;
      std::stringstream error;
      my_logger log(out, error, level::verbose);
      my_assert a(log);

      int current_line = __LINE__;
      std::string message("Test assert vector");
      std::vector<int> vector1 = { 1, 2 };
      std::vector<int> vector2 = { 1, 2, 3 };

      std::stringstream ss;
      ss << "FAIL " << expected_file_name << " " << current_line << " Vector assert: " << message << ": Size of the vectors are equal. Expected: " << vector1.size() << ". Actual: " << vector2.size() << "." << std::endl;

      assert_is_false("Comparing 2 vectors with different size should fail", a.vector_equal(__FILE__, current_line, message, vector1, vector2));
      assert_string_empty("Output is still empty", out.str());
      assert_equal("Error string is correct", ss.str(), error.str());
      assert_uint64_t_equal("Passed count is still 0", 0, a.passed());
      assert_uint64_t_equal("Failed count is now 1", 1, a.failed());
      assert_uint64_t_equal("Total count is now 1", 1, a.total());
   }
   test_section("Testing the assert vector with different content")
   {
      std::stringstream out;
      std::stringstream error;
      my_logger log(out, error, level::verbose);
      my_assert a(log);

      int current_line = __LINE__;
      std::string message("Test assert vector");
      std::vector<int> vector1 = { 1, 2, 3 };
      std::vector<int> vector2 = { 3, 1, 2 };

      std::stringstream successful;
      successful << "PASS " << expected_file_name << " " << current_line << " Vector assert: " << message << ": Size of the vectors are equal." << std::endl;

      std::stringstream failed;
      for (size_t i = 0; i < vector1.size(); ++i)
      {
         failed << "FAIL " << expected_file_name << " " << current_line
            << " Vector assert: " << message << ": Value of vector at index " << i << " should be equal. Expected: " << vector1[i] << ". Actual: " << vector2[i] << "." << std::endl;
      }

      assert_is_false("Comparing 2 vectors with different content should fail", a.vector_equal(__FILE__, current_line, message, vector1, vector2));
      assert_equal("Output string is correct", successful.str(), out.str());
      assert_equal("Error string is correct", failed.str(), error.str());
      assert_uint64_t_equal("Passed count is still 0", 1, a.passed());
      assert_uint64_t_equal("Failed count is now 1", 3, a.failed());
      assert_uint64_t_equal("Total count is now 1", 4, a.total());
   }
   test_section("Testing the assert vector with the same size and content")
   {
      std::stringstream out;
      std::stringstream error;
      my_logger log(out, error, level::verbose);
      my_assert a(log);

      int current_line = __LINE__;
      std::string message("Test assert vector");
      std::vector<int> vector1 = { 1, 2, 3 };
      std::vector<int> vector2 = { 1, 2, 3 };

      std::stringstream ss;
      ss << "PASS " << expected_file_name << " " << current_line << " Vector assert: " << message << ": Size of the vectors are equal." << std::endl;

      for (size_t i = 0; i < vector1.size(); ++i)
      {
         ss << "PASS " << expected_file_name << " " << current_line
            << " Vector assert: " << message << ": Value of vector at index " << i << " should be equal." << std::endl;
      }

      assert_is_true("Comparing 2 vectors with same size and content should be successful", a.vector_equal(__FILE__, current_line, message, vector1, vector2));
      assert_equal("Output string is correct", ss.str(), out.str());
      assert_string_empty("Error string is empty", error.str());
      assert_uint64_t_equal("Passed count is still 0", 4, a.passed());
      assert_uint64_t_equal("Failed count is now 1", 0, a.failed());
      assert_uint64_t_equal("Total count is now 1", 4, a.total());
   }
}
