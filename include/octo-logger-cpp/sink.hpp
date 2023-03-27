/**
 * @file sink.hpp
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef SINK_HPP_
#define SINK_HPP_

#include "octo-logger-cpp/channel.hpp"
#include "octo-logger-cpp/log.hpp"
#include "octo-logger-cpp/logger.hpp"
#include "octo-logger-cpp/sink-config.hpp"
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

namespace octo::logger
{
class Sink
{
  public:
    enum class LineFormat {
      PLAINTEXT_LONG = 0,    // Used in console
      PLAINTEXT_SHORT = 1,   // Used in syslog
#ifdef OCTO_ENABLE_JSON_FORMATTING
      JSON = 2,
#endif
      DEFAULT = PLAINTEXT_LONG
    };

  private:
    const SinkConfig config_;

  protected:
    const SinkConfig& config() const;
    const std::string origin_;
    const LineFormat line_format_;

    std::string formatted_log_plaintext_long(Log const& log, Channel const& channel, Logger::ContextInfo const& context_info, bool disable_context_info) const;
    std::string formatted_log_plaintext_short(Log const& log, Channel const& channel) const;
#ifdef OCTO_ENABLE_JSON_FORMATTING
    std::string formatted_log_JSON(Log const& log, Channel const& channel, Logger::ContextInfo const& context_info) const;
#endif

    inline std::string formatted_log(Log const& log, Channel const& channel, Logger::ContextInfo const& context_info, bool disable_context_info) const {
      switch(line_format_)  {
        case LineFormat::PLAINTEXT_LONG:
          return formatted_log_plaintext_long(log, channel,  context_info, disable_context_info);
        case LineFormat::PLAINTEXT_SHORT:
          return formatted_log_plaintext_short(log, channel);
#ifdef OCTO_ENABLE_JSON_FORMATTING
        case LineFormat::JSON:
          return formatted_log_JSON(log, channel,  context_info);
#endif
      }
    }

    inline static LineFormat extract_format_with_default(const SinkConfig& config, LineFormat default_format) {
      return LineFormat(config.option_default(SinkConfig::SinkOption::LINE_FORMAT, int(default_format)));
    }

    [[nodiscard]] virtual std::string formatted_context_info(Log const& log,
                                                             Channel const& channel,
                                                             Logger::ContextInfo const& context_info) const;

  public:
    explicit Sink(const SinkConfig& config, std::string const & origin, LineFormat format);
    virtual ~Sink() = default;

    virtual void dump(const Log& log, const Channel& channel, Logger::ContextInfo const& context_info) = 0;
    const std::string& sink_name() const;
};
typedef std::shared_ptr<Sink> SinkPtr;
} // namespace octo::logger

#if defined(OCTO_ENABLE_JSON_FORMATTING) && defined(OCTO_UNITTEST)
#include <nlohmann/json.hpp>
namespace octo::logger::unittests
{
  void init_context_info(nlohmann::json& dst, Log const& log, Channel const& channel, Logger::ContextInfo const& context_info);
  nlohmann::json init_context_info(Log const& log, Channel const& channel, Logger::ContextInfo const& context_info);
} // namespace octo::logger::unittests
#endif

#endif
