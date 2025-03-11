#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <array>
#include <QString>

using std::array;
namespace config
{
const QString umiocrConfig="E:\\WorkSpace\\Coding\\ScreenshotTranslation\\ScreenshotTranslation\\thirdParty\\UmiOCR-data\\.settings";
const QString ocrexePath = "E:\\WorkSpace\\Coding\\ScreenshotTranslation\\ScreenshotTranslation\\thirdParty\\Umi-OCR.exe";
const QString saveShotPath = "E:\\WorkSpace\\Coding\\ScreenshotTranslation\\ScreenshotTranslation\\saveShots\\shot.png";
inline static array<QString,12> ocrCommands{"--help"};
}

#endif // CONFIG_HPP
