#include "mainwindow.h"
#include <QApplication>
#include "config.hpp"
#include "common.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    config::cfg.executableDir = QCoreApplication::applicationDirPath();
    config::cfg.make();

    initLog(config::cfg.logPath.toStdString());
    spdlog::info("【" + nowStr() + "】	"+ "ot run");

    MainWindow w;
    w.show();
    return a.exec();
}
