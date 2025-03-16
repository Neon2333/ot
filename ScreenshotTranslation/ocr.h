#ifndef OCR_H
#define OCR_H
#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <vector>
#include "config.hpp"
#include <memory>
// #include <filesystem>

using std::vector;
using std::string;
using std::unique_ptr;
//namespace fs=std::filesystem;



class Ocr:public QObject
{
    Q_OBJECT

public:
    Ocr();
    Ocr(vector<QString> commands);
    void startOcr();
    void quitOcr();
    QProcess* process();

    void useConfig(config::Language language);
    void setCommands(vector<QString> commands);

    ~Ocr();

private:
    QStringList m_command;
    QProcess* m_ocrProcess=nullptr;
    QProcess* m_quitProcess=nullptr;


signals:
    void resReady(QString res);
    void resError(QString err);
    void quitOcrFinished();

};

#endif // OCR_H
