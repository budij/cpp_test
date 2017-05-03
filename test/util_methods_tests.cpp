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
