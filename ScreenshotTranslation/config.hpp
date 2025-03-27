#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <array>
#include <QString>
#include <string>
#include <QCoreApplication>
#include <QSettings>

using std::array;
using std::string;


namespace config
{
//写死
enum class Language{none,zh,en,jp};

const QString ocrLanguageModelCh="models/config_chinese.txt";
const QString ocrLanguageModelJp="models/config_japan.txt";
const QString ocrLanguageModelEn="models/config_en.txt";
const string fanyiURL="http://api.fanyi.baidu.com";

//main.cpp初始化
extern QString executableDir;
extern QString umiocrConfig;
extern QString configPath;
extern QString ocrexePath;
extern QString saveShotPath;
extern bool popOcrWindow;
extern array<QString,12> ocrCommands;

//读配置
extern string fanyiID;
extern string fanyiKey;
extern Language initOcrLanguage;
extern Language initTransLanguage;

inline static void readConfig(QString path)
{
    QSettings setting(config::configPath, QSettings::IniFormat);

    QVariant id = setting.value("Global/transID");
    config::fanyiID = id.toString().toStdString();

    QVariant key = setting.value("Global/transKey");
    config::fanyiKey = id.toString().toStdString();

    QVariant ocrLanguage = setting.value("Global/ocrLanguage");
    switch(ocrLanguage.toInt())
    {
    case 1:
        config::initOcrLanguage=config::Language::zh;
        break;
    case 2:
        config::initOcrLanguage=config::Language::en;
        break;
    case 3:
        config::initOcrLanguage=config::Language::jp;
        break;
    default:
        config::initOcrLanguage=config::Language::none;
        break;
    }

    QVariant transLanguage = setting.value("Global/transLanguage");
    switch(transLanguage.toInt())
    {
    case 1:
        config::initTransLanguage=config::Language::zh;
        break;
    case 2:
        config::initTransLanguage=config::Language::en;
        break;
    case 3:
        config::initTransLanguage=config::Language::jp;
        break;
    default:
        config::initTransLanguage=config::Language::none;
        break;
    }
}

};



#endif // CONFIG_HPP
