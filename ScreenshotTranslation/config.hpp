#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <array>
#include <QString>

using std::array;
namespace config
{
const QString ocrexePath = "E:\\WorkSpace\\Coding\\ScreenshotTranslation\\ScreenshotTranslation\\thirdParty\\Umi-OCR.exe";
const QString saveShotPath = "./saveShot";
inline static array<QString,12> ocrCommands{"--help"};
}

#endif // CONFIG_HPP
