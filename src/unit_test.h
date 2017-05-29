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

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <iomanip>
#include <iostream>

#define test_main(title)                                 main_test_function(title)
#define test_method(name, description)                   unit_test_method(name, description)
#define test_method_list(name, description, type, list)  unit_test_method_list(name, description, type, list)
#define test_section(message)

///////////////////////////////////////////////////////////////////////////////////
// assert macros
#define assert_equal(message, expected, actual)          assert.equal(__FILE__, __LINE__, message, expected, actual)
#define assert_not_equal(message, expected, actual)      assert.not_equal(__FILE__, __LINE__, message, expected, actual)
#define assert_not_null(message, actual)                 assert.generic(__FILE__, __LINE__, "Not null", message, (void*)nullptr, (void*)(actual), [](void* expected, void* value) { return expected != value; })
#define assert_uint64_t_equal(message, expected, actual) assert_equal(message, uint64_t(expected), uint64_t(actual))
#define assert_uint32_t_equal(message, expected, actual) assert_equal(message, uint32_t(expected), uint32_t(actual))
#define assert_enum_equal(message, expected, actual)     assert_uint32_t_equal(message, expected, actual)
#define assert_size_t_equal(message, expected, actual)   assert_equal(message, size_t(expected), size_t(actual))
#define assert_is_true(message, actual)                  assert.generic(__FILE__, __LINE__, "Is true", message, true, (actual), [](bool expected, bool value) { return expected == value; })
#define assert_is_false(message, actual)                 assert.generic(__FILE__, __LINE__, "Is false", message, false, (actual), [](bool expected, bool value) { return expected == value; })
#define assert_pass(message)                             assert.pass(__FILE__, __LINE__, message)
#define assert_fail(message)                             assert.fail(__FILE__, __LINE__, message)
#define assert_string_empty(message, actual)             assert_equal(message, std::string(), actual)
#define assert_vector_equal(message, expected, actual)   assert.vector_equal(__FILE__, __LINE__, message, expected, actual)
#define assert_vector_empty(message, actual)             assert_size_t_equal(message, 0, actual.size())
#define assert_ptr_equal(message, expected, actual)      assert_equal(message, ((void*)expected), ((void*)actual))
#define assert_ptr_null(message, actual)                 assert_ptr_equal(message, nullptr, actual)
#define assert_ptr_not_equal(message, expected, actual)  assert_not_equal(message, ((void*)expected), ((void*)actual))
#define assert_ptr_not_null(message, actual)             assert_not_equal(message, nullptr, actual)

///////////////////////////////////////////////////////////////////////////////////
// useful macros
#define array_size(test_array, struct_type)              (sizeof(test_array) / sizeof(struct_type))


namespace aes
{
   namespace test
   {
      namespace utils
      {
         template <typename T>
         char** convert_to_char_array(const T& input) noexcept;
         template<typename _TIterator, typename _TPredicate, typename _TFunction>
         _TFunction for_each_if(_TIterator first,
                                _TIterator last,
                                _TPredicate predicate,
                                _TFunction operation);
         size_t find_min_pos(size_t pos1, size_t pos2, size_t end_pos = std::string::npos);
         size_t find_max_pos(size_t pos1, size_t pos2, size_t end_pos = std::string::npos);
         int unit_test_main(int argc, char** argv, const char* title);
      }

      namespace log
      {
         enum level
         {
            error = 0,
            warning = 1,
            information = 3,
            verbose = 7,
            trace = 15,
            debug = 31
         };

         template <typename _TOut, typename _TError>
         class logger_base
         {
         public:
            logger_base(_TOut& out_writer, _TError& error_writer) noexcept;
            logger_base(_TOut& out_writer, _TError& error_writer, level level) noexcept;
            logger_base(const logger_base&) = default;
            ~logger_base() noexcept = default;

         public:
            logger_base& operator=(const logger_base&) = default;

         public:
            template <typename T>
            void log_error(const T& message) const noexcept;
            template <typename T>
            void log_warning(const T& message) const noexcept;
            template <typename T>
            void log_information(const T& message) const noexcept;
            template <typename T>
            void log_verbose(const T& message) const noexcept;
            template <typename T>
            void log_trace(const T& message) const noexcept;
            template <typename T>
            void log_debug(const T& message) const noexcept;

         public:
            bool should_log_error() const noexcept;
            bool should_log_warning() const noexcept;
            bool should_log_information() const noexcept;
            bool should_log_verbose() const noexcept;
            bool should_log_trace() const noexcept;
            bool should_log_debug() const noexcept;
            template <level l>
            bool is_level() const noexcept;
            level log_level() const noexcept;
            void log_level(level new_level) noexcept;

         private:
            template <typename T, typename _TShouldLog, typename _TStream>
            void log(const T& message, _TShouldLog should_log_func, _TStream& stream) const noexcept;

         private:
            _TOut& out_;
            _TError&  error_;
            level level_;
         };
      }

      template <typename _TLogger>
      class assert_base
      {
      public:
         assert_base(_TLogger& logger) noexcept;
         assert_base(const assert_base&) = default;
         ~assert_base() noexcept = default;

      public:
         assert_base& operator=(const assert_base&) = default;

      public:
         template <typename T>
         bool equal(const std::string& file, int line, const std::string& message, const T& expected, const T& actual) noexcept;
         template <typename T>
         bool not_equal(const std::string& file, int line, const std::string& message, const T& expected, const T& actual) noexcept;
         template <typename T, typename _TPredicate>
         bool generic(const std::string& file, int line, const std::string& assert_type, const std::string& message, const T& expected, const T& actual, _TPredicate pred) noexcept;
         template <typename T, typename _TPredicate>
         bool generic(const std::string& file, int line, const std::string& assert_type, const std::string& message, const T& actual, _TPredicate pred) noexcept;
         template <typename T>
         bool generic(const std::string& file, int line, const std::string& assert_type, const std::string& message, const T& expected, const T& actual, bool result) noexcept;
         template <typename T>
         bool generic(const std::string& file, int line, const std::string& assert_type, const std::string& message, const T& actual, bool result) noexcept;
         bool pass(const std::string& file, int line, const std::string& message) noexcept;
         bool fail(const std::string& file, int line, const std::string& message) noexcept;
         template <typename T>
         bool vector_equal(const std::string& file, int line, const std::string& message, const std::vector<T>& expected, const std::vector<T>& actual) noexcept;

      public:
         uint64_t passed() const noexcept;
         uint64_t failed() const noexcept;
         uint64_t total() const noexcept;

      private:
         std::string file_name(const std::string file_path) noexcept;
         void log_result(const std::string& file, int line, bool result, const std::string& message) noexcept;
         void log_fail(const std::string& file, int line, const std::string& message) noexcept;
         void log_success(const std::string& file, int line, const std::string& message) noexcept;

      private:
         _TLogger& logger_;
         uint64_t passed_;
         uint64_t failed_;
      };

      template <typename _TSuiteSingleton, typename _TLogger>
      class unit_test_base
      {
      public:
         unit_test_base(const std::string& name, const std::string description) noexcept;
         unit_test_base(const unit_test_base&) = default;
         virtual ~unit_test_base() noexcept = default;

      public:
         unit_test_base& operator=(const unit_test_base&) = default;

      public:
         bool run_test();

      public:
         const std::string& name() const noexcept;
         const std::string& description() const noexcept;
         uint64_t passed() const noexcept;
         uint64_t failed() const noexcept;
         uint64_t total() const noexcept;

      private:
         virtual void run_tests(assert_base<_TLogger>& assert) = 0;

      private:
         assert_base<_TLogger> assert_;
         std::string name_;
         std::string description_;
      };

      template <typename _TSuiteSingleton, typename _TLogger>
      class test_suite_base
      {
      public:
         test_suite_base(_TLogger& logger) noexcept;
         test_suite_base(const test_suite_base&) = delete;
         ~test_suite_base() noexcept = default;

      public:
         test_suite_base& operator=(const test_suite_base&) = delete;

      public:
         bool register_test(unit_test_base<_TSuiteSingleton, _TLogger>* test) noexcept;
         bool run(const std::string& title);

      public:
         _TLogger& test_logger() const noexcept;
         uint64_t passed() const noexcept;
         uint64_t failed() const noexcept;
         uint64_t total() const noexcept;

      private:
         _TLogger& logger_;
         uint64_t passed_;
         uint64_t failed_;
         std::multimap<const std::string, unit_test_base<_TSuiteSingleton, _TLogger>*> map_;
      };

      class test_suite_singleton
      {
      public:
         static test_suite_base<test_suite_singleton, aes::test::log::logger_base<std::ostream, std::ostream>>& get() noexcept;
      };
   }
}

using logger = aes::test::log::logger_base<std::ostream, std::ostream>;
using test_assert = aes::test::assert_base<logger>;
using unit_test = aes::test::unit_test_base<aes::test::test_suite_singleton, logger>;
using test_suite = aes::test::test_suite_base<aes::test::test_suite_singleton, logger>;

#define unit_test_method(name, description)                                   \
class unit_test_##name : public unit_test                                     \
{                                                                             \
   public:                                                                    \
      unit_test_##name() : unit_test("  " #name " ", description) {}          \
   private:                                                                   \
      virtual void run_tests(test_assert& assert);                            \
};                                                                            \
static unit_test_##name unit_test_obj_##name;                                 \
void unit_test_##name::run_tests(test_assert& assert)

#define unit_test_method_list(name, description, list_type, list)             \
class unit_test_##name : public unit_test                                     \
{                                                                             \
   public:                                                                    \
      unit_test_##name() : unit_test("  " #name " ", description) {}          \
   private:                                                                   \
      virtual void run_tests(test_assert& assert);                            \
      void run_tests(test_assert& assert, list_type& input);                  \
};                                                                            \
static unit_test_##name unit_test_obj_##name;                                 \
void unit_test_##name::run_tests(test_assert& assert)                         \
{                                                                             \
   std::for_each(list.begin(), list.end(), [&](list_type& input)  \
   {                                                                          \
      run_tests(assert, input);                                               \
   });                                                                        \
}                                                                             \
void unit_test_##name::run_tests(test_assert& assert, list_type& input)

#define main_test_function(title)                                                           \
   int main(int argc, char** argv)                                                          \
   {                                                                                        \
      return aes::test::utils::unit_test_main(argc, argv, title); \
   }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// methods in aes::test::utils namespace

template <typename T>
inline char** aes::test::utils::convert_to_char_array(const T& input) noexcept
{
   char** result = {};

   if (input.size() > 0)
   {
      result = const_cast<char**>(&input[0]);
   }

   return result;
}

template<typename _TIterator, typename _TPredicate, typename _TFunction>
inline _TFunction aes::test::utils::for_each_if(_TIterator first,
                                                _TIterator last,
                                                _TPredicate predicate,
                                                _TFunction operation)
{
   for (int index = 0; first != last; ++first, ++index)
   {
      if (predicate(*first, index))
      {
         operation(*first, index);
      }
   }

   return operation;
}

inline size_t aes::test::utils::find_min_pos(size_t pos1, size_t pos2, size_t end_pos)
{
   return pos1 == end_pos ? pos2 : pos2 == end_pos ? pos1 : std::min(pos1, pos2);
}

inline size_t aes::test::utils::find_max_pos(size_t pos1, size_t pos2, size_t end_pos)
{
   return pos1 == end_pos ? pos2 : pos2 == end_pos ? pos1 : std::max(pos1, pos2);
}


///////////////////////////////////////////////////////////////////////////////////
// logger_base implementation

template <typename _TOut, typename _TError>
inline aes::test::log::logger_base<_TOut, _TError>::logger_base(_TOut& out_writer, _TError& error_writer) noexcept
   : logger_base(out_writer, error_writer, level::information)
{
}

template <typename _TOut, typename _TError>
inline aes::test::log::logger_base<_TOut, _TError>::logger_base(_TOut& out_writer, _TError& error_writer, level level) noexcept
   : out_(out_writer)
   , error_(error_writer)
   , level_(level)
{
}

template <typename _TOut, typename _TError>
template <typename T>
inline void aes::test::log::logger_base<_TOut, _TError>::log_error(const T& message) const noexcept
{
   log(message, [this]() { return should_log_error(); }, error_);
}

template <typename _TOut, typename _TError>
template <typename T>
inline void aes::test::log::logger_base<_TOut, _TError>::log_warning(const T& message) const noexcept
{
   log(message, [this]() { return should_log_warning(); }, out_);
}

template <typename _TOut, typename _TError>
template <typename T>
inline void aes::test::log::logger_base<_TOut, _TError>::log_information(const T& message) const noexcept
{
   log(message, [this]() { return should_log_information(); }, out_);
}

template <typename _TOut, typename _TError>
template <typename T>
inline void aes::test::log::logger_base<_TOut, _TError>::log_verbose(const T& message) const noexcept
{
   log(message, [this]() { return should_log_verbose(); }, out_);
}

template <typename _TOut, typename _TError>
template <typename T>
inline void aes::test::log::logger_base<_TOut, _TError>::log_trace(const T& message) const noexcept
{
   log(message, [this]() { return should_log_trace(); }, out_);
}

template <typename _TOut, typename _TError>
template <typename T>
inline void aes::test::log::logger_base<_TOut, _TError>::log_debug(const T& message) const noexcept
{
   log(message, [this]() { return should_log_debug(); }, out_);
}

template <typename _TOut, typename _TError>
inline bool aes::test::log::logger_base<_TOut, _TError>::should_log_error() const noexcept
{
   return is_level<error>();
}

template <typename _TOut, typename _TError>
inline bool aes::test::log::logger_base<_TOut, _TError>::should_log_warning() const noexcept
{
   return is_level<warning>();
}

template <typename _TOut, typename _TError>
inline bool aes::test::log::logger_base<_TOut, _TError>::should_log_information() const noexcept
{
   return is_level<information>();
}

template <typename _TOut, typename _TError>
inline bool aes::test::log::logger_base<_TOut, _TError>::should_log_verbose() const noexcept
{
   return is_level<verbose>();
}

template <typename _TOut, typename _TError>
inline bool aes::test::log::logger_base<_TOut, _TError>::should_log_trace() const noexcept
{
   return is_level<trace>();
}

template <typename _TOut, typename _TError>
inline bool aes::test::log::logger_base<_TOut, _TError>::should_log_debug() const noexcept
{
   return is_level<debug>();
}

template <typename _TOut, typename _TError>
template <aes::test::log::level l>
inline bool aes::test::log::logger_base<_TOut, _TError>::is_level() const noexcept
{
   return (level_ & l) == l;
}

template <typename _TOut, typename _TError>
inline aes::test::log::level aes::test::log::logger_base<_TOut, _TError>::log_level() const noexcept
{
   return level_;
}

template <typename _TOut, typename _TError>
inline void aes::test::log::logger_base<_TOut, _TError>::log_level(level new_level) noexcept
{
   level_ = new_level;
}

template <typename _TOut, typename _TError>
template <typename T, typename _TShouldLog, typename _TStream>
inline void aes::test::log::logger_base<_TOut, _TError>::log(const T& message,
                                                        _TShouldLog should_log_func,
                                                        _TStream& stream) const noexcept
{
   if (should_log_func())
   {
      stream << message << std::endl;
   }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// assert class implementation

template <typename _TLogger>
inline aes::test::assert_base<_TLogger>::assert_base(_TLogger& logger) noexcept
   : logger_(logger)
   , passed_(0)
   , failed_(0)
{
}

template <typename _TLogger>
template <typename T>
inline bool aes::test::assert_base<_TLogger>::equal(const std::string& file, int line, const std::string& message, const T& expected, const T& actual) noexcept
{
   return generic(file, line, "Equal", message, expected, actual, expected == actual);
}

template <typename _TLogger>
template <typename T>
inline bool aes::test::assert_base<_TLogger>::not_equal(const std::string& file, int line, const std::string& message, const T& expected, const T& actual) noexcept
{
   return generic(file, line, "Not equal", message, expected, actual, expected != actual);
}

template <typename _TLogger>
template <typename T, typename _TPredicate>
inline bool aes::test::assert_base<_TLogger>::generic(const std::string& file,
                                                      int line,
                                                      const std::string& assert_type,
                                                      const std::string& message,
                                                      const T& expected,
                                                      const T& actual,
                                                      _TPredicate pred) noexcept
{
   return generic(file, line, assert_type, message, expected, actual, pred(expected, actual));
}

template <typename _TLogger>
template <typename T, typename _TPredicate>
inline bool aes::test::assert_base<_TLogger>::generic(const std::string& file,
                                                      int line,
                                                      const std::string& assert_type,
                                                      const std::string& message,
                                                      const T& actual,
                                                      _TPredicate pred) noexcept
{
   return generic(file, line, assert_type, message, actual, pred(actual));
}

template <typename _TLogger>
template <typename T>
inline bool aes::test::assert_base<_TLogger>::generic(const std::string& file,
                                                      int line,
                                                      const std::string& assert_type,
                                                      const std::string& message,
                                                      const T& expected,
                                                      const T& actual,
                                                      bool result) noexcept
{
   std::stringstream ss;
   ss << assert_type << ": " << message << ".";
   if (!result)
   {
      ss << " Expected: " << expected << ". Actual: " << actual << ".";
   }

   log_result(file, line, result, ss.str());
   return result;
}

template <typename _TLogger>
template <typename T>
inline bool aes::test::assert_base<_TLogger>::generic(const std::string& file,
                                                      int line,
                                                      const std::string& assert_type,
                                                      const std::string& message,
                                                      const T& actual,
                                                      bool result) noexcept
{
   std::stringstream ss;
   ss << assert_type << ": " << message << ".";
   if (!result)
   {
      ss << " Actual: " << actual << ".";
   }

   log_result(file, line, result, ss.str());
   return result;
}

template <typename _TLogger>
inline bool aes::test::assert_base<_TLogger>::pass(const std::string& file, int line, const std::string& message) noexcept
{
   std::stringstream ss;
   ss << "Assert passed logged with message: " << message << ".";
   log_result(file, line, true, ss.str());
   return true;
}

template <typename _TLogger>
inline bool aes::test::assert_base<_TLogger>::fail(const std::string& file, int line, const std::string& message) noexcept
{
   std::stringstream ss;
   ss << "Assert failed logged with message: " << message << ".";
   log_result(file, line, false, ss.str());
   return false;
}

template <typename _TLogger>
template <typename T>
inline bool aes::test::assert_base<_TLogger>::vector_equal(const std::string& file, int line, const std::string& message, const std::vector<T>& expected, const std::vector<T>& actual) noexcept
{
   bool result = false;
   std::stringstream ss;
   ss << message << ": Size of the vectors are equal";
   if ((result = generic(file, line, "Vector assert", ss.str(), expected.size(), actual.size(), [](size_t expected, size_t actual) { return expected == actual; })))
   {
      for (size_t i = 0; i < expected.size(); i++)
      {
         ss.str("");
         ss << message << ": Value of vector at index " << i << " should be equal";
         result &= generic(file, line, "Vector assert", ss.str(), expected[i], actual[i], [](const T& expected, const T& actual) { return expected == actual; });
      }
   }

   return result;
}

template <typename _TLogger>
inline uint64_t aes::test::assert_base<_TLogger>::passed() const noexcept
{
   return passed_;
}

template <typename _TLogger>
inline uint64_t aes::test::assert_base<_TLogger>::failed() const noexcept
{
   return failed_;
}

template <typename _TLogger>
inline uint64_t aes::test::assert_base<_TLogger>::total() const noexcept
{
   return failed_ + passed_;
}

template <typename _TLogger>
inline std::string aes::test::assert_base<_TLogger>::file_name(const std::string file_path) noexcept
{
   std::string name(file_path);
   size_t index = aes::test::utils::find_max_pos(file_path.find_last_of("\\"), file_path.find_last_of("/"));

   if (index != std::string::npos)
   {
      name = file_path.substr(index + 1);
   }

   return name;
}

template <typename _TLogger>
void aes::test::assert_base<_TLogger>::log_result(const std::string& file, int line, bool result, const std::string& message) noexcept
{
   if (result)
   {
      passed_++;
      log_success(file, line, message);
   }
   else
   {
      failed_++;
      log_fail(file, line, message);
   }
}

template <typename _TLogger>
inline void aes::test::assert_base<_TLogger>::log_fail(const std::string& file, int line, const std::string& message) noexcept
{
   std::stringstream ss;
   ss << "FAIL " << file_name(file) << " " << line << " " << message;
   logger_.log_error(ss.str());
}

template <typename _TLogger>
inline void aes::test::assert_base<_TLogger>::log_success(const std::string& file, int line, const std::string& message) noexcept
{
   std::stringstream ss;
   ss << "PASS " << file_name(file) << " " << line << " " << message;
   logger_.log_verbose(ss.str());
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// unit_test_base class implementation

template <typename _TSuiteSingleton, typename _TLogger>
inline aes::test::unit_test_base<_TSuiteSingleton, _TLogger>::unit_test_base(const std::string& name, const std::string description) noexcept
   : assert_(_TSuiteSingleton::get().test_logger())
   , name_(name)
   , description_(description)
{
   _TSuiteSingleton::get().register_test(this);
}

template <typename _TSuiteSingleton, typename _TLogger>
inline bool aes::test::unit_test_base<_TSuiteSingleton, _TLogger>::run_test()
{
   run_tests(assert_);
   return failed() == 0;
}

template <typename _TSuiteSingleton, typename _TLogger>
inline const std::string& aes::test::unit_test_base<_TSuiteSingleton, _TLogger>::name() const noexcept
{
   return name_;
}

template <typename _TSuiteSingleton, typename _TLogger>
inline const std::string& aes::test::unit_test_base<_TSuiteSingleton, _TLogger>::description() const noexcept
{
   return description_;
}

template <typename _TSuiteSingleton, typename _TLogger>
inline uint64_t aes::test::unit_test_base<_TSuiteSingleton, _TLogger>::passed() const noexcept
{
   return assert_.passed();
}

template <typename _TSuiteSingleton, typename _TLogger>
inline uint64_t aes::test::unit_test_base<_TSuiteSingleton, _TLogger>::failed() const noexcept
{
   return assert_.failed();
}

template <typename _TSuiteSingleton, typename _TLogger>
inline uint64_t aes::test::unit_test_base<_TSuiteSingleton, _TLogger>::total() const noexcept
{
   return assert_.total();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// test_suite_base class implementation

template <typename _TSuiteSingleton, typename _TLogger>
inline aes::test::test_suite_base<_TSuiteSingleton, _TLogger>::test_suite_base(_TLogger& logger) noexcept
   : logger_(logger)
   , passed_(0)
   , failed_(0)
   , map_()
{
}

template <typename _TSuiteSingleton, typename _TLogger>
inline bool aes::test::test_suite_base<_TSuiteSingleton, _TLogger>::register_test(unit_test_base<_TSuiteSingleton, _TLogger>* test) noexcept
{
   bool result = false;

   if (test)
   {
      map_.insert(std::pair<const std::string, unit_test_base<_TSuiteSingleton, _TLogger>*>(test->name(), test));
      result = true;
   }

   return result;
}

template <typename _TSuiteSingleton, typename _TLogger>
inline bool aes::test::test_suite_base<_TSuiteSingleton, _TLogger>::run(const std::string& title)
{
   const int width = 5;
   typename std::multimap<const std::string, unit_test_base<_TSuiteSingleton, _TLogger>*>::iterator it;

   logger_.log_information(title);
   logger_.log_information("--------------------------------------------------------------");
   for (it = map_.begin(); it != map_.end(); ++it)
   {
      time_t start = time(0);
      it->second->run_test();
      time_t end = time(0);

      uint64_t total = it->second->total();
      passed_ += it->second->passed();
      failed_ += it->second->failed();

      std::stringstream ss;
      ss << std::setiosflags(std::ios::left);
      ss << "TEST  " << std::setw(width) << total << " Passed " << std::setw(width) << it->second->passed() << " Failed " << std::setw(width) << it->second->failed() << " " << it->second->name() << "(" << (end - start) << "s)";
      ss << std::resetiosflags(std::ios::left);
      logger_.log_information(ss.str());
   }
   logger_.log_information("--------------------------------------------------------------");

   std::stringstream ss;
   ss << std::setiosflags(std::ios::left);
   ss << "TOTAL " << std::setw(width) << total() << " PASSED " << std::setw(width) << passed() << " FAILED " << std::setw(width) << failed() << "   " << title;
   ss << std::resetiosflags(std::ios::left);
   logger_.log_information(ss.str());
   return failed() == 0;
}

template <typename _TSuiteSingleton, typename _TLogger>
inline _TLogger& aes::test::test_suite_base<_TSuiteSingleton, _TLogger>::test_logger() const noexcept
{
   return logger_;
}

template <typename _TSuiteSingleton, typename _TLogger>
inline uint64_t aes::test::test_suite_base<_TSuiteSingleton, _TLogger>::passed() const noexcept
{
   return passed_;
}

template <typename _TSuiteSingleton, typename _TLogger>
inline uint64_t aes::test::test_suite_base<_TSuiteSingleton, _TLogger>::failed() const noexcept
{
   return failed_;
}

template <typename _TSuiteSingleton, typename _TLogger>
inline uint64_t aes::test::test_suite_base<_TSuiteSingleton, _TLogger>::total() const noexcept
{
   return passed_ + failed_;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// test_suite_singleton class implementation

inline test_suite& aes::test::test_suite_singleton::get() noexcept
{
   static logger log(std::cout, std::cerr);
   static test_suite suite(log);
   return suite;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// unit_test_main function implementation

inline int aes::test::utils::unit_test_main(int argc, char** argv, const char* title)
{
   for (int i = 1; i < argc; ++i)
   {
      char* str = argv[i];
      if (str && (*str == '-' || *str == '/'))
      {
         ++str;
         if (*str == 'v' || *str == 'V')
         {
            aes::test::test_suite_singleton::get().test_logger().log_level(aes::test::log::level::verbose);
         }
         else
         {
            std::stringstream ss;
            ss << "Error: invalid or unknown argument " << argv[i];
            aes::test::test_suite_singleton::get().test_logger().log_error(ss.str());
            return -1;
         }
      }
      else
      {
         std::stringstream ss;
         ss << "Error: invalid or unknown argument " << argv[i];
         aes::test::test_suite_singleton::get().test_logger().log_error(ss.str());
         return -1;
      }
   }

   aes::test::test_suite_singleton::get().run(title);
   return int(aes::test::test_suite_singleton::get().failed());
}
