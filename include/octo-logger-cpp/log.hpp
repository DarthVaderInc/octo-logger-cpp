/**
 * @file log.h
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef LOG_HPP_
#define LOG_HPP_

#include "octo-logger-cpp/logger-test-definitions.hpp"
#include <fmt/format.h>
#include <fmt/printf.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>

namespace octo::logger
{
class Logger;
class Log
{
  public:
    enum class LogLevel : std::uint8_t
    {
        TRACE = 1,
        DEBUG = 2,
        INFO = 4,
        NOTICE = 8,
        WARNING = 16,
        ERROR = 32,
        QUIET = 255,
    };

  private:
    std::unique_ptr<std::ostringstream> stream_;
    LogLevel log_level_;
    const Logger& logger_;
    std::chrono::time_point<std::chrono::system_clock> time_created_;
    std::string extra_identifier_;

  private:
    Log(const LogLevel& log_level, std::string_view extra_identifier, const Logger& logger);

  public:
    virtual ~Log();

    static std::string level_to_string(const LogLevel& level);
    static LogLevel string_to_level(const std::string& level_str);

    const std::chrono::time_point<std::chrono::system_clock>& time_created() const;
    const std::ostringstream* stream() const;
    const LogLevel& log_level() const;
    const std::string& extra_identifier() const;
    template <class T>
    Log& operator<<(const T& value)
    {
        if (stream_)
        {
            *stream_ << value;
        }
        return *this;
    }

    template <typename... Args>
    void formatted(char const* fmt, Args... args)
    {
        if (stream_)
        {
            *stream_ << fmt::format(fmt, args...);
        }
    }

    template <typename... Args>
    void formattedf(char const* fmt, Args... args)
    {
        if (stream_)
        {
            *stream_ << fmt::sprintf(fmt, args...);
        }
    }

    friend class Logger;

    TESTS_MOCK_CLASS(Log)
};
} // namespace octo::logger

#endif
