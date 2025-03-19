#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <array>
#include <QString>
#include <string>
#include <QCoreApplication>

using std::array;
using std::string;


namespace config
{
enum class Language{none,zh,en,jp};
extern array<QString,12> ocrCommands;

const QString ocrLanguageModelCh="models/config_chinese.txt";
const QString ocrLanguageModelJp="models/config_japan.txt";
const QString ocrLanguageModelEn="models/config_en.txt";

extern QString executableDir;
extern QString umiocrConfig;
extern QString ocrexePath;
extern QString saveShotPath;
extern bool popOcrWindow;

const string fanyiURL="http://api.fanyi.baidu.com";
const string fanyiID="ID";//读配置文件
const string fanyiKey="key";

};

#endif // CONFIG_HPP
