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
  private:
    const SinkConfig config_;

  protected:
    const SinkConfig& config() const;
    const std::string origin_;

    enum class LineFormat {
      PLAINTEXT_LONG = 0,
      PLAINTEXT_SHORT = 1,
      JSON = 2,
      DEFAULT = PLAINTEXT_LONG
    };

    std::string formatted_log_plaintext_long(Log const& log, Channel const& channel, Logger::ContextInfo const& context_info, bool disable_context_info) const;
    std::string formatted_log_plaintext_short(Log const& log, Channel const& channel) const;
    std::string formatted_log_JSON(Log const& log, Channel const& channel, Logger::ContextInfo const& context_info) const;

    inline std::string formatted_log(Log const& log, Channel const& channel, Logger::ContextInfo const& context_info, bool disable_context_info, LineFormat format = LineFormat::DEFAULT) const {
      switch(format)  {
        case LineFormat::PLAINTEXT_LONG:
          return formatted_log_plaintext_long(log, channel,  context_info, disable_context_info);
        case LineFormat::PLAINTEXT_SHORT:
          return formatted_log_plaintext_short(log, channel);
        case LineFormat::JSON:
          return formatted_log_JSON(log, channel,  context_info);
      }
    }

    [[nodiscard]] virtual std::string formatted_context_info(Log const& log,
                                                             Channel const& channel,
                                                             Logger::ContextInfo const& context_info) const;

  public:
    explicit Sink(const SinkConfig& config, std::string const & origin);
    virtual ~Sink() = default;

    virtual void dump(const Log& log, const Channel& channel, Logger::ContextInfo const& context_info) = 0;
    const std::string& sink_name() const;
};
typedef std::shared_ptr<Sink> SinkPtr;
} // namespace octo::logger

#if OCTO_UNITTEST
#include <nlohmann/json.hpp>
namespace octo::logger::unittests
{
  void init_context_info(nlohmann::json& dst, Log const& log, Channel const& channel, Logger::ContextInfo const& context_info);
  nlohmann::json init_context_info(Log const& log, Channel const& channel, Logger::ContextInfo const& context_info);
} // namespace octo::logger::unittests
#endif

#endif
