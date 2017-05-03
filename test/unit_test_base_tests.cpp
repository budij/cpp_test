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
   template <typename _TUnitTest>
   class mock_test_suite
   {
   public:
      mock_test_suite()
         : out_()
         , error_()
         , logger_(out_, error_)
         , unit_test_(nullptr)
      {
      }
      my_logger& test_logger() noexcept
      {
         return logger_;
      }
      void register_test(_TUnitTest *test)
      {
         unit_test_ = test;
      }
      _TUnitTest* unit_test() const noexcept
      {
         return unit_test_;
      }

   private:
      std::stringstream out_;
      std::stringstream error_;
      my_logger logger_;
      _TUnitTest *unit_test_;
   };

   class mock_test_suite_singleton
   {
   public:
      using my_unit_test = unit_test_base<mock_test_suite_singleton, my_logger>;

      static mock_test_suite<my_unit_test>& get()
      {
         static mock_test_suite<my_unit_test> test_suite;
         return test_suite;
      }
   };

   class unit_test_child : public mock_test_suite_singleton::my_unit_test
   {
   public:
      unit_test_child(const std::string& name, const std::string& description)
         : unit_test_base(name, description)
         , run_test_called_(false)
      {
      }

   public:
      bool is_run_test_called() const noexcept
      {
         return run_test_called_;
      }

   private:
      virtual void run_tests(my_assert& assert)
      {
         run_test_called_ = true;
      };

   private:
      bool run_test_called_;
   };
}

test_method(unit_test_constructor_tests, "Testing the constructor of unit_test_base class")
{
   test_section("Testing the constructor through the chile class")
   {
      std::string name("name");
      std::string description("description");
      unit_test_child test(name, description);

      assert_equal("Name of the test has been assigned successfully", name, test.name());
      assert_equal("Description of the test has been assigned successfully", description, test.description());
      assert_uint64_t_equal("Passed test is still 0", 0, test.passed());
      assert_uint64_t_equal("Failed test is still 0", 0, test.failed());
      assert_uint64_t_equal("Total test is still 0", 0, test.total());
      assert_equal("Current test has been registered", (void*)&test, (void*)mock_test_suite_singleton::get().unit_test());
   }
}

test_method(unit_test_run_test, "Testing the run_test method")
{
   test_section("Testing the run_test method")
   {
      std::string name("name");
      std::string description("description");
      unit_test_child test(name, description);

      assert_is_true("Running the mock test will always be successful", test.run_test());
      assert_is_true("The child run_test has been called", test.is_run_test_called());
   }
}