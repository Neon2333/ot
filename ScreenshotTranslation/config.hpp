#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <array>
#include <QString>

using std::array;
namespace config
{
const QString ocrLanguageCh="models/config_chinese.txt";
const QString ocrLanguageJpn="models/config_japan.txt";
const QString ocrLanguageEn="models/config_en.txt";

inline static array<QString,12> ocrCommands{"--help"};

const QString umiocrConfig="E:\\WorkSpace\\Coding\\ScreenshotTranslation\\ScreenshotTranslation\\thirdParty\\UmiOCR-data\\.settings";
const QString ocrexePath = "E:\\WorkSpace\\Coding\\ScreenshotTranslation\\ScreenshotTranslation\\thirdParty\\Umi-OCR.exe";
const QString saveShotPath = "E:\\WorkSpace\\Coding\\ScreenshotTranslation\\ScreenshotTranslation\\saveShots\\shot.png";
const bool popOcrWindow=false;

}

#endif // CONFIG_HPP
