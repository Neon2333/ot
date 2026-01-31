#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <array>
#include <QString>
#include <string>
#include <QCoreApplication>
#include <QSettings>

using std::array;
using std::string;

enum class Language{none,zh,en,jp};

struct Config
{
const QString ocrLanguageModelCh="models/config_chinese.txt";
const QString ocrLanguageModelJp="models/config_japan.txt";
const QString ocrLanguageModelEn="models/config_en.txt";
const string fanyiURL="https://api.fanyi.baidu.com/";
const string sentenceYuyinURL="https://fanyi.baidu.com/";
const string wordURL="https://api.dictionaryapi.dev/api/v2/entries/en/";

//main.cpp初始化
inline static QString executableDir;
inline static QString umiocrConfig;
inline static QString configPath;
inline static QString ocrexePath;
inline static QString saveShotPath;
inline static QString logPath;
inline static QString pronPath;
inline static QString caPath;
inline static bool popOcrWindow;
inline static array<QString,12> ocrCommands;

//读配置
inline static string fanyiID;
inline static string fanyiKey;
inline static Language initOcrLanguage;
inline static Language initTransLanguage;

static void make()
{
    // umiocrConfig=QCoreApplication::applicationDirPath() + "/thirdParty/UmiOCR-data/.settings";
    // qDebug()<<umiocrConfig;
    // configPath=QCoreApplication::applicationDirPath() + "/config.ini";
    // ocrexePath=QCoreApplication::applicationDirPath() + "/thirdParty/Umi-OCR.exe";
    // saveShotPath=QCoreApplication::applicationDirPath() + "/saveShots/shot.png";
    umiocrConfig = executableDir + "thirdParty/UmiOCR-data/.settings";
    configPath = executableDir + "config.ini";
    ocrexePath = executableDir + "thirdParty/Umi-OCR.exe";
    saveShotPath = executableDir + "saveShots/shot.png";
    logPath = executableDir + "log.txt";
    pronPath = executableDir + "pronouncation";
    caPath = executableDir + "thirdParty/lib/curl/cacert.pem";
    popOcrWindow=false;
    ocrCommands = {"--help"};

    QSettings setting(configPath, QSettings::IniFormat);

    QVariant idTmp = setting.value("Global/transID");
    fanyiID = idTmp.toString().toStdString();

    QVariant keyTmp = setting.value("Global/transKey");
    fanyiKey = keyTmp.toString().toStdString();

    QVariant ocrLanguage = setting.value("Global/ocrLanguage");
    switch(ocrLanguage.toInt())
    {
    case 1:
        initOcrLanguage=Language::zh;
        break;
    case 2:
        initOcrLanguage=Language::en;
        break;
    case 3:
        initOcrLanguage=::Language::jp;
        break;
    default:
        initOcrLanguage=::Language::none;
        break;
    }

    QVariant transLanguage = setting.value("Global/transLanguage");
    switch(transLanguage.toInt())
    {
    case 1:
        initTransLanguage=Language::zh;
        break;
    case 2:
        initTransLanguage=Language::en;
        break;
    case 3:
        initTransLanguage=Language::jp;
        break;
    default:
        initTransLanguage=Language::none;
        break;
    }
}
};


namespace config
{
    static inline Config cfg;
};

#endif // CONFIG_HPP
