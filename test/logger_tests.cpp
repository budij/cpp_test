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
#include <algorithm>

using namespace aes::test;
using namespace aes::test::log;

using my_logger = logger_base<std::stringstream, std::stringstream>;

test_method(logger_constructor_tests, "Testing the constructors for logger class")
{
   test_section("Testing logger constructor with default level")
   {
      std::stringstream out;
      std::stringstream error;
      my_logger log(out, error);
      assert_equal("Log level is informational", level::information, log.log_level());
      assert_is_true("Should log error is true", log.should_log_error());
      assert_is_true("Should log warning is true", log.should_log_warning());
      assert_is_true("Should log information is true", log.should_log_information());
      assert_is_false("Should log verbose is false", log.should_log_verbose());
      assert_is_false("Should log trace is false", log.should_log_trace());
      assert_is_false("Should log debug is false", log.should_log_debug());
   }
   test_section("Testing logger constructor with custom level")
   {
      level log_level(level::debug);
      std::stringstream out;
      std::stringstream error;
      my_logger log(out, error, log_level);
      assert_equal("Log level is informational", log_level, log.log_level());
      assert_is_true("Should log error is true", log.should_log_error());
      assert_is_true("Should log warning is true", log.should_log_warning());
      assert_is_true("Should log information is true", log.should_log_information());
      assert_is_true("Should log verbose is true", log.should_log_verbose());
      assert_is_true("Should log trace is true", log.should_log_trace());
      assert_is_true("Should log debug is true", log.should_log_debug());
   }
}

namespace
{
   struct log_level_test_struct
   {
      level level_;
      bool is_error_;
      bool is_warning_;
      bool is_information_;
      bool is_verbose_;
      bool is_trace_;
      bool is_debug_;
   };

   std::vector<log_level_test_struct> log_level_test_inputs =
   {
        { level::error, true, false, false, false, false, false }
      , { level::warning, true, true, false, false, false, false }
      , { level::information, true, true, true, false, false, false }
      , { level::verbose, true, true, true, true, false, false }
      , { level::trace, true, true, true, true, true, false }
      , { level::debug, true, true, true, true, true, true }
   };
}

test_method_list(logger_log_level_tests, "Testing the log level getter and setter", log_level_test_struct, log_level_test_inputs)
{
   std::stringstream out;
   std::stringstream error;
   my_logger log(out, error);
   log.log_level(input.level_);

   assert_equal("Log level is correct", input.level_, log.log_level());
   assert_equal("Should log error is correct", input.is_error_, log.should_log_error());
   assert_equal("Should log warning is correct", input.is_warning_, log.should_log_warning());
   assert_equal("Should log information is correct", input.is_information_, log.should_log_information());
   assert_equal("Should log verbose is correct", input.is_verbose_, log.should_log_verbose());
   assert_equal("Should log trace is correct", input.is_trace_, log.should_log_trace());
   assert_equal("Should log debug is correct", input.is_debug_, log.should_log_debug());

   assert_equal("Is level -> error is correct", input.is_error_, log.is_level<level::error>());
   assert_equal("Is level -> warning is correct", input.is_warning_, log.is_level<level::warning>());
   assert_equal("Is level -> information is correct", input.is_information_, log.is_level<level::information>());
   assert_equal("Is level -> verbose is correct", input.is_verbose_, log.is_level<level::verbose>());
   assert_equal("Is level -> trace is correct", input.is_trace_, log.is_level<level::trace>());
   assert_equal("Is level -> debug is correct", input.is_debug_, log.is_level<level::debug>());
}

template <typename _TLogFunc, typename _TShouldLogFunc, typename _TLogStream, typename _TNoLogStream>
void logging_test(test_assert& assert,
                  _TLogFunc log_func,
                  _TShouldLogFunc should_log_func,
                  _TLogStream log_stream_func,
                  _TNoLogStream no_log_stream_func)
{
   std::for_each(log_level_test_inputs.cbegin(),
                 log_level_test_inputs.cend(),
                 [&assert, &log_func, &should_log_func, &log_stream_func, &no_log_stream_func](const log_level_test_struct& input)
   {
      std::string message("This is my message");
      std::stringstream out;
      std::stringstream error;
      my_logger log(out, error);
      log.log_level(input.level_);

      log_func(log, message);
      if (should_log_func(log))
      {
         assert_equal("Message has been logged", message + "\n", log_stream_func(out, error));
      }
      else
      {
         assert_string_empty("Message has not been logged", log_stream_func(out, error));
      }

      assert_string_empty("Other stream is still empty", no_log_stream_func(out, error));
   });
}

test_method(logger_log_tests, "Test all the log methods")
{
   logging_test(assert,
                [](my_logger& log, const std::string& message) { log.log_error(message); },
                [](my_logger& log) { return log.should_log_error(); },
                [](std::stringstream& out, std::stringstream& error) { return error.str(); },
                [](std::stringstream& out, std::stringstream& error) { return out.str(); });
   logging_test(assert,
                [](my_logger& log, const std::string& message) { log.log_warning(message); },
                [](my_logger& log) { return log.should_log_warning(); },
                [](std::stringstream& out, std::stringstream& error) { return out.str(); },
                [](std::stringstream& out, std::stringstream& error) { return error.str(); });
   logging_test(assert,
                [](my_logger& log, const std::string& message) { log.log_information(message); },
                [](my_logger& log) { return log.should_log_information(); },
                [](std::stringstream& out, std::stringstream& error) { return out.str(); },
                [](std::stringstream& out, std::stringstream& error) { return error.str(); });
   logging_test(assert,
                [](my_logger& log, const std::string& message) { log.log_verbose(message); },
                [](my_logger& log) { return log.should_log_verbose(); },
                [](std::stringstream& out, std::stringstream& error) { return out.str(); },
                [](std::stringstream& out, std::stringstream& error) { return error.str(); });
   logging_test(assert,
                [](my_logger& log, const std::string& message) { log.log_trace(message); },
                [](my_logger& log) { return log.should_log_trace(); },
                [](std::stringstream& out, std::stringstream& error) { return out.str(); },
                [](std::stringstream& out, std::stringstream& error) { return error.str(); });
   logging_test(assert,
                [](my_logger& log, const std::string& message) { log.log_debug(message); },
                [](my_logger& log) { return log.should_log_debug(); },
                [](std::stringstream& out, std::stringstream& error) { return out.str(); },
                [](std::stringstream& out, std::stringstream& error) { return error.str(); });
}
