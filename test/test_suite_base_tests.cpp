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

namespace
{
   std::stringstream out;
   std::stringstream err;

   class mock_test_suite_singleton
   {
   public:
      static test_suite_base<mock_test_suite_singleton, my_logger>& get()
      {
         static my_logger log(out, err);
         static test_suite_base<mock_test_suite_singleton, my_logger> test_suite(log);
         return test_suite;
      }
   };

   class mock_unit_test : public unit_test_base<mock_test_suite_singleton, my_logger>
   {
   public:
      mock_unit_test(const std::string& test_name) noexcept : unit_test_base(test_name, "description") { }
      ~mock_unit_test() noexcept = default;

   private:
      void run_tests(assert_base<my_logger>& assert)
      {
         assert.pass(__FILE__, __LINE__, "Some message");
         assert.fail(__FILE__, __LINE__, "Fail message");
      };
   };
}

using my_test_suite = test_suite_base<mock_test_suite_singleton, my_logger>;

test_method(test_suite_base_constructor_test, "Testing the constructor of test_suite_base class")
{
   test_section("Testing test suite constructor")
   {
      std::stringstream out;
      std::stringstream error;
      my_logger log(out, error);
      my_test_suite test_suite(log);
      my_logger& obtained_logger = test_suite.test_logger();
      assert_equal("The logger has been initialised to the same object", (void*)&log, (void*)&obtained_logger);
      assert_uint64_t_equal("The total number of passed test has been initialised to 0", 0, test_suite.passed());
      assert_uint64_t_equal("The total number of failed test has been initialised to 0", 0, test_suite.failed());
      assert_uint64_t_equal("The total number of total test has been initialised to 0", 0, test_suite.total());
   }
}

test_method(test_suite_register_and_run_test, "Testing the register and run test methods")
{
   test_section("Testing test suite constructor")
   {
      my_test_suite& test_suite = mock_test_suite_singleton::get();

      std::string test_name("test_name");
      mock_unit_test test(test_name);

      std::string title("title");
      std::stringstream expected;
      expected << title << std::endl;
      expected << "--------------------------------------------------------------" << std::endl;
      expected << "TEST  2     Passed 1     Failed 1     test_name(0s)" << std::endl;
      expected << "--------------------------------------------------------------" << std::endl;
      expected << "TOTAL 2     PASSED 1     FAILED 1       " << title << std::endl;

      assert_is_false("Running the test that will result in failed tests will fail", test_suite.run(title));
      assert_equal("Output of the message is correct", expected.str(), out.str());
      assert_equal("Total passed is correct", test.passed(), test_suite.passed());
      assert_equal("Total failed is correct", test.failed(), test_suite.failed());
      assert_equal("Total tests is correct", test.total(), test_suite.total());
   }
}
