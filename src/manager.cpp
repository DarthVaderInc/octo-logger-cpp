/**
 * @file manager.cpp
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "octo-logger-cpp/manager.hpp"

namespace octo::logger
{
std::shared_ptr<Manager> Manager::manager_;
std::mutex Manager::manager_init_mutex_;

Manager::Manager() : config_(std::make_shared<ManagerConfig>()), default_log_level_(Log::LogLevel::INFO)
{
}

Manager& Manager::instance()
{
    if (manager_)
    {
        return *manager_;
    }
    {
        std::lock_guard<std::mutex> lock(manager_init_mutex_);
        if (!manager_)
        {
            manager_ = std::shared_ptr<Manager>(new Manager());
            manager_->global_logger_ = std::make_shared<Logger>("GLOBAL");
        }
    }
    return *manager_;
}

void Manager::reset_manager()
{
    std::lock_guard<std::mutex> lock(manager_init_mutex_);
    if (manager_)
    {
        manager_.reset();
    }
}

Manager::~Manager()
{
    terminate();
}

void Manager::create_channel(std::string_view name)
{
    channels_.try_emplace(std::string(name), Channel(name, default_log_level_));
}

const Channel& Manager::channel(const std::string& name) const
{
    if (channels_.find(name) != channels_.cend())
    {
        return channels_.at(name);
    }
    throw std::runtime_error("No channel for given name [" + name + "]");
}

Channel& Manager::editable_channel(const std::string& name)
{
    if (channels_.find(name) != channels_.cend())
    {
        return channels_.at(name);
    }
    throw std::runtime_error("No channel for given name [" + name + "]");
}

void Manager::configure(const ManagerConfigPtr& config, bool clear_old_sinks)
{
    if (clear_old_sinks)
    {
        clear_sinks();
    }
    config_ = config;

    // Change the default level if requested by config
    if (config_->has_option(ManagerConfig::LoggerOption::DEFAULT_CHANNEL_LEVEL))
    {
        int default_level;
        if (config_->option(ManagerConfig::LoggerOption::DEFAULT_CHANNEL_LEVEL, default_level))
        {
            default_log_level_ = static_cast<Log::LogLevel>(default_level);
        }
    }

    // Create all the sinks
    for (auto& sink_config : config_->sinks())
    {
        SinkPtr sink = SinkFactory::instance().create_sink(sink_config);
        if (sink)
        {
            sinks_.push_back(sink);
        }
    }
    for (auto& sink : config_->custom_sinks())
    {
        sinks_.push_back(sink);
    }

    for (auto& channel : channels_)
    {
        channel.second.set_log_level(default_log_level_);
    }
}

void Manager::terminate()
{
    clear_sinks();
    clear_channels();
}

void Manager::dump(const Log& log, const std::string& channel_name, Logger::ContextInfo const& context_info)
{
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    for (auto& sink : sinks_)
    {
        sink->dump(log, channel(channel_name), context_info);
    }
}

void Manager::clear_sinks()
{
    sinks_.clear();
}

void Manager::clear_channels()
{
    channels_.clear();
}

const Logger& Manager::global_logger() const
{
    return *global_logger_;
}

Log::LogLevel Manager::get_log_level() const
{
    return default_log_level_;
}

void Manager::set_log_level(Log::LogLevel log_level)
{
    if (log_level == default_log_level_)
    {
        return;
    }
    std::lock_guard<std::mutex> lock(sinks_mutex_);
    default_log_level_ = log_level;
    for (auto& channel : channels_)
    {
        channel.second.set_log_level(default_log_level_);
    }
}
} // namespace octo::logger
