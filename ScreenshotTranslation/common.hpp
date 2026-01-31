#ifndef COMMON_HPP
#define COMMON_HPP
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <QDebug>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <array>
#include <regex>

using std::string;
using std::array;

#define qout qDebug()<<"["<<__FILE__<<":"<<__LINE__<<"]"



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
    //设置日志缓冲刷新时间
    spdlog::flush_every(std::chrono::seconds(10));
}

//获取当前时间YYYY-MM-DD HH:mm:SS
inline string nowStr()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    std::tm* localTime;
    localTime = localtime(&now_c);
    ss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

inline void logInfo(string info)
{
    spdlog::info("[" + nowStr() + "]    " + info);
}

inline void logError(string error)
{
    spdlog::error("[" + nowStr() + "]    " + error);
}

//字符转义
inline static std::vector<array<string,2>> escapeChars
{
    {"%20", " "},
    {"%28", "("},
    {"%29", ")"},
    {"%2C", ","},
    {"%3A", ":"},
    {"%3C", "<"},
    {"%3D", "="},
    {"%3E", ">"},
    // {"%27", "'"},
    {"%40", "@"},

    {"\xCB\x88", "'"},
    {"\x69\xCC\x90", "iː"},
    {"\xC9\xAA", "ɪ"},
    {"\xCA\x8A", "ʊ"},
    {"\x75\xCC\x90", "uː"},
    {"\x65", "e"},
    {"\xC9\x99", "ə"},
    {"\xC9\x9C\xCC\x90", "ɜː"},
    {"\xC9\x94\xCC\x90", "ɔː"},
    {"\xC3\xA6", "æ"},
    {"\xCA\x8C", "ʌ"},
    {"\xC9\x91\xCC\x90", "ɑː"},
    {"\xC9\x92", "ɒ"},
    {"\xC9\x9B", "ɛ"},
    {"\x65\xCA\x8A", "eɪ"},
    {"\x61\xCA\x8A", "aɪ"},
    {"\xC9\x94\xCA\x8A", "ɔɪ"},
    {"\xCA\x83", "ʃ"},
    {"\xCA\x92", "ʒ"},
    {"\xCE\xB8", "θ"},
    {"\xC3\xB0", "ð"},
    {"\xC5\x8B", "ŋ"}
};

inline void escape(string& str)
{
    for (int i = 0; i < escapeChars.size(); i++)
    {
        str = std::regex_replace(str, std::regex(escapeChars[i][0]), escapeChars[i][1]);
    }
}

inline void escape(QString& str)
{
    std::string strTmp = str.toStdString();
    for (int i = 0; i < escapeChars.size(); i++)
    {
        strTmp = std::regex_replace(strTmp, std::regex(escapeChars[i][0]), escapeChars[i][1]);
    }
    str = QString::fromStdString(strTmp);
}

inline QString replaceSpace(const string& str)
{
    std::string result;
    for (char c : str)
    {
        if (c == ' ')
        {
            result += "%20";
        }
        else
        {
            result += c;
        }
    }
    return QString::fromStdString(result);
}

inline QString replaceSpace(const QString& str)
{
    return replaceSpace(str.toStdString());
}

inline size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

#endif // COMMON_HPP
