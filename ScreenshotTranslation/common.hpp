#ifndef COMMON_HPP
#define COMMON_HPP
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <QDebug>
#include <ctime>
#include <iomanip>
#include <sstream>

inline void initLog(std::string logfilePath)
{
    std::string logFileName = logfilePath;
    size_t max_file_size = 10 * 1024 * 1024; // 10MB
    size_t max_files = 10;
    //定义并初始化滚动日志记录器对象logger
    auto file_logger = spdlog::rotating_logger_mt("file_logger", logFileName, max_file_size, max_files);
    //设定显示的日志级别为info及以上
    file_logger->set_level(spdlog::level::info);
    file_logger->set_formatter(std::make_unique<spdlog::pattern_formatter>("%v"));
    //设定默认日志记录器为file_logger
    spdlog::set_default_logger(file_logger);
}

//获取当前时间YYYY-MM-DD HH:mm:SS
inline std::string nowStr()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    std::tm* localTime;
    localTime = localtime(&now_c);
    ss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}


#endif // COMMON_HPP
