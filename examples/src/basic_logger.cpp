/**
 * @file basic_logger.cpp
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "octo-logger-cpp/manager.hpp"
#include "octo-logger-cpp/logger.hpp"

int main(int argc, char** argv)
{
    auto config = std::make_shared<octo::logger::ManagerConfig>();
    config->set_option(octo::logger::ManagerConfig::LoggerOption::DEFAULT_CHANNEL_LEVEL,
                       static_cast<int>(octo::logger::Log::LogLevel::TRACE));
    octo::logger::SinkConfig console_sink("Console", octo::logger::SinkConfig::SinkType::CONSOLE_SINK);
    config->add_sink(console_sink);
    octo::logger::Manager::instance().configure(config);

    octo::logger::Logger logger("Test");
    logger.trace() << "HI1";
    logger.debug() << "HI2";
    logger.info() << "HI3";
    logger.notice() << "HI4";
    logger.warning() << "HI5";
    logger.error() << "HI6";

    logger.trace("ID1") << "HI1 WITH ID";
    logger.debug("ID2") << "HI2 WITH ID";
    logger.info("ID3") << "HI3 WITH ID";
    logger.notice("ID4") << "HI4 WITH ID";
    logger.warning("ID5") << "HI5 WITH ID";
    logger.error("ID6") << "HI6 WITH ID";

    logger.log(octo::logger::Log::LogLevel::ERROR, "BLA") << "BLA LOG";

    octo::logger::Manager::instance().global_logger().info() << "WTF IS GLOBAL";

    octo::logger::Manager::instance().terminate();
}