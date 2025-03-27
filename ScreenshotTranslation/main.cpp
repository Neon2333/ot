#include "mainwindow.h"
#include <QApplication>
#include "config.hpp"


// #include "spdlog/spdlog.h"
// #include "spdlog/sinks/rotating_file_sink.h"

// void initLog()
// {
//     std::string logFileName = "logs/logfile.log";
//     size_t max_file_size = 10 * 1024 * 1024; // 10MB
//     size_t max_files = 10;
//     //定义并初始化滚动日志记录器对象logger
//     auto file_logger = spdlog::rotating_logger_mt("file_logger", logFileName, max_file_size, max_files);
//     //设定显示的日志级别为info及以上
//     file_logger->set_level(spdlog::level::info);
//     file_logger->set_formatter(std::make_unique<spdlog::pattern_formatter>("%v"));
//     //设定默认日志记录器为file_logger
//     spdlog::set_default_logger(file_logger);
// }



namespace config
{
    QString executableDir;
    QString umiocrConfig;
    QString configPath;
    QString ocrexePath;
    QString saveShotPath;
    bool popOcrWindow;
    array<QString,12> ocrCommands;

    string fanyiID;
    string fanyiKey;
    Language initOcrLanguage;
    Language initTransLanguage;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    config::executableDir = QCoreApplication::applicationDirPath();
    config::umiocrConfig=QCoreApplication::applicationDirPath() + "/thirdParty/UmiOCR-data/.settings";
    config::configPath=QCoreApplication::applicationDirPath() + "/config.ini";
    config::ocrexePath=QCoreApplication::applicationDirPath() + "/thirdParty/Umi-OCR.exe";
    config::saveShotPath=QCoreApplication::applicationDirPath() + "/saveShots/shot.png";
    config::popOcrWindow=false;
    config::ocrCommands = {"--help"};

    config::readConfig(config::configPath);

    MainWindow w;
    w.show();
    return a.exec();


    // initLog();

    // // 使用全局函数进行日志记录
    // spdlog::info("This is an info message.");
    // spdlog::error("This is an error message with arg: {}", 1);
    // spdlog::warn("Easy padding in numbers like {:08d}", 12);

}
