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
#include <sstream>

using namespace aes::test;
using namespace aes::test::log;
using namespace aes::test::utils;

test_method(convert_char_array_tests, "Testing the convert_to_char_array method")
{
   std::vector<const char*> cli_inputs = { "-d", "-v" };
   char** output = convert_to_char_array(cli_inputs);

   for (size_t i = 0; i < cli_inputs.size(); ++i)
   {
      assert_equal("Array item is correct", (char*)cli_inputs[i], output[i]);
   }
}

test_method(foreach_if_tests, "Testing the foreach_if method")
{
   std::vector<std::string> items;
   for (int i = 0; i < 10; i++)
   {
      items.push_back("");
   }

   for_each_if(items.begin(),
               items.end(),
               [](std::string&, int index) { return index % 2 == 0; },
               [](std::string& item, int) { item = "true"; });

   for (size_t i = 0; i < items.size(); i++)
   {
      assert_equal("Value is correct", i % 2 == 0, !items[i].empty());
   }
}

namespace
{
   struct min_max_test_struct
   {
      size_t pos1_;
      size_t pos2_;
      size_t expected_min_;
      size_t expected_max_;
   };
   std::vector<min_max_test_struct> min_max_test_inputs
   {
      { std::string::npos, std::string::npos, std::string::npos, std::string::npos },
      { size_t(20), std::string::npos, size_t(20), size_t(20) },
      { std::string::npos, size_t(20), size_t(20), size_t(20) },
      { size_t(20), size_t(21), size_t(20), size_t(21) },
   };
}

test_method_list(find_min_max_pos_tests, "Testing the pos min and max functions", min_max_test_struct, min_max_test_inputs)
{
   assert_equal("Minimum position is correct", input.expected_min_, find_min_pos(input.pos1_, input.pos2_));
   assert_equal("Maximum position is correct", input.expected_max_, find_max_pos(input.pos1_, input.pos2_));
}

namespace
{
   using logger_mock = aes::test::log::logger_base<std::stringstream, std::stringstream>;

   class test_suite_singleton_mock
   {
   public:
      static test_suite_base<test_suite_singleton_mock, logger_mock>& get() noexcept;
   };

   using test_suite_mock = aes::test::test_suite_base<test_suite_singleton_mock, logger_mock>;

   std::stringstream out;
   std::stringstream err;

   inline test_suite_mock& test_suite_singleton_mock::get() noexcept
   {
      static logger_mock log(out, err);
      static test_suite_mock suite(log);
      return suite;
   }

   struct main_test_struct
   {
      int argc_;
      std::vector<char*> argv_;
      std::string title_;
      int expected_;
      std::string expected_output_;
      std::string expected_error_;
      level expected_level_;
   };

   std::vector<main_test_struct> main_test_inputs =
   {
      { 0, std::vector<char*>(),                           "Testing without any arguments",  0, "Testing without any arguments\n--------------------------------------------------------------\n--------------------------------------------------------------\nTOTAL 0     PASSED 0     FAILED 0       Testing without any arguments\n", "",                                        level::information },
      { 2, std::vector<char*> { "ApplicationName", "-v" }, "Testing without any arguments",  0, "Testing without any arguments\n--------------------------------------------------------------\n--------------------------------------------------------------\nTOTAL 0     PASSED 0     FAILED 0       Testing without any arguments\n", "",                                        level::verbose     },
      { 2, std::vector<char*> { "ApplicationName", "/v" }, "Testing without any arguments",  0, "Testing without any arguments\n--------------------------------------------------------------\n--------------------------------------------------------------\nTOTAL 0     PASSED 0     FAILED 0       Testing without any arguments\n", "",                                        level::verbose     },
      { 2, std::vector<char*> { "ApplicationName", "-V" }, "Testing without any arguments",  0, "Testing without any arguments\n--------------------------------------------------------------\n--------------------------------------------------------------\nTOTAL 0     PASSED 0     FAILED 0       Testing without any arguments\n", "",                                        level::verbose     },
      { 2, std::vector<char*> { "ApplicationName", "/V" }, "Testing without any arguments",  0, "Testing without any arguments\n--------------------------------------------------------------\n--------------------------------------------------------------\nTOTAL 0     PASSED 0     FAILED 0       Testing without any arguments\n", "",                                        level::verbose     },
      { 2, std::vector<char*> { "ApplicationName", "-a" }, "Testing without any arguments", -1, "",                                                                                                                                                                                                                                       "Error: invalid or unknown argument -a\n", level::information },
      { 2, std::vector<char*> { "ApplicationName", "aa" }, "Testing without any arguments", -1, "",                                                                                                                                                                                                                                       "Error: invalid or unknown argument aa\n", level::information },
   };
}

test_method_list(unit_test_main_tests, "Testing the unit test main function", main_test_struct, main_test_inputs)
{
   out.str("");
   err.str("");
   test_suite_singleton_mock::get().test_logger().log_level(level::information);

   assert_equal("Result should be 0", input.expected_, aes::test::utils::unit_test_main<test_suite_singleton_mock>(input.argc_, convert_to_char_array(input.argv_), input.title_.c_str()));
   assert_equal("Output is correct", input.expected_output_, out.str());
   assert_equal("Error is correct", input.expected_error_, err.str());
   assert_enum_equal("Log level is correct", input.expected_level_, test_suite_singleton_mock::get().test_logger().log_level());
}
