#ifndef OCR_H
#define OCR_H
#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <vector>
// #include <filesystem>

using std::vector;
using std::string;
//namespace fs=std::filesystem;



class Ocr:public QObject
{
    Q_OBJECT

public:
    Ocr();
    Ocr(vector<QString> commands);
    void startOcr();
    QProcess* process();

    void useConfig(QString language);
    void setCommands(vector<QString> commands);

    ~Ocr();

private:
    QStringList m_command;
    QProcess* m_ocrProcess=nullptr;



signals:
    void resReady(QString res);

};

#endif // OCR_H
