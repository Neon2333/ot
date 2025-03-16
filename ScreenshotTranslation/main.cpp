#include "mainwindow.h"
#include <QApplication>

namespace config
{
QString executableDir;
QString umiocrConfig;
QString ocrexePath;
QString saveShotPath;
bool popOcrWindow;
array<QString,12> ocrCommands;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    config::executableDir = QCoreApplication::applicationDirPath();
    config::umiocrConfig=QCoreApplication::applicationDirPath() + "/thirdParty/UmiOCR-data/.settings";
    config::ocrexePath=QCoreApplication::applicationDirPath() + "/thirdParty/Umi-OCR.exe";
    config::saveShotPath=QCoreApplication::applicationDirPath() + "/saveShots/shot.png";
    config::popOcrWindow=false;
    config::ocrCommands = {"--help"};

    MainWindow w;
    w.show();
    return a.exec();
}
