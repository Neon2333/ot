#include "ocr.h"
#include <QSettings>
#include <QDebug>

Ocr::Ocr() {}

Ocr::Ocr(vector<QString> commands)
{
    if(m_command.size()!=0)    m_command.clear();
    // m_command << "umi-ocr";//注意不要传这个命令到setArguments，否则报错
    for(int i=0;i<commands.size();i++)
    {
        m_command << commands.at(i);
    }

    m_ocrProcess = new QProcess;
    m_ocrProcess->setProgram(config::ocrexePath);
    m_ocrProcess->setArguments(m_command);


    // connect(m_ocrProcess, &QProcess::readyReadStandardOutput, [this](){
    //     QByteArray output = m_ocrProcess->readAllStandardOutput();
    //     QString result = QString::fromLocal8Bit(output);
    //     if(result.isEmpty())
    //     {
    //         emit resReady("OCR 识别失败");
    //     }
    //     else
    //     {
    //         emit resReady(result);
    //         qDebug()<<"result="<<result;
    //     }
    // });

    connect(m_ocrProcess, &QProcess::finished, [=](int code){
        if (code != 0)
        {
            QByteArray error = m_ocrProcess->readAllStandardError();
            emit resStatus(QString("error: OCR failed...code=%1").arg(code));
        }

        QByteArray output = m_ocrProcess->readAllStandardOutput();
        QString result = QString::fromLocal8Bit(output);
        if(result.isEmpty())
        {
            emit resStatus("error: contents of OCR is empty...");
        }
        else
        {
            emit resReady(result);
        }
    });
}

void Ocr::startOcr()
{
    if(m_ocrProcess!=nullptr)
    {
        m_ocrProcess->start();
        if(!m_ocrProcess->waitForFinished(10000))
        {
            emit resStatus(QString("error: ocr out of time..."));//超时
        }
    }
}

void Ocr::quitOcr()
{
    if(m_quitProcess!=nullptr)   m_quitProcess->deleteLater();
    m_quitProcess = new QProcess;
    connect(m_quitProcess, &QProcess::finished, this, &Ocr::quitOcrFinished);
    m_quitProcess->setProgram(config::ocrexePath);
    qDebug()<<config::ocrexePath;
    QStringList quitOcr;
    // restartOcr << "--quit" << "--show" << "--hide";
    quitOcr << "--quit";
    m_quitProcess->setArguments(quitOcr);
    m_quitProcess->start();
    // m_ocrProcess->waitForFinished(10000);

    // if(!m_ocrProcess->waitForFinished(10000))
    // {
    //     emit resError("error: restart out of time(10s)...");
    // }
}

QProcess *Ocr::process()
{
    return m_ocrProcess;
}

void Ocr::useConfig(config::Language language)
{
    QSettings setting(config::umiocrConfig, QSettings::IniFormat);
    //不弹出ocr窗口
    QVariant isPopOcrWindow = setting.value("ScreenshotOCR/action.popMainWindow");
    if(isPopOcrWindow.toBool())
    {
        setting.setValue("ScreenshotOCR/action.popMainWindow", config::popOcrWindow);
    }
    //设定识别语言
    QVariant ocrLanguage = setting.value("ScreenshotOCR/ocr.language");
    switch(language)
    {
    case config::Language::zh:
        setting.setValue("ScreenshotOCR/ocr.language", config::ocrLanguageModelCh);
        break;
    case config::Language::en:
        setting.setValue("ScreenshotOCR/ocr.language", config::ocrLanguageModelEn);
        break;
    case config::Language::jp:
        setting.setValue("ScreenshotOCR/ocr.language", config::ocrLanguageModelJp);
        break;
    default:
        break;
    }
}

void Ocr::setCommands(vector<QString> commands)
{
    if(m_command.size()!=0)    m_command.clear();
    for(int i=0;i<commands.size();i++)
    {
        m_command << commands.at(i);
    }

    if(m_ocrProcess!=nullptr)   m_ocrProcess->deleteLater();
    m_ocrProcess = new QProcess;
    m_ocrProcess->setProgram(config::ocrexePath);
    m_ocrProcess->setArguments(m_command);

    connect(m_ocrProcess, &QProcess::finished, [=](int code){
        if (code != 0)
        {
            QByteArray error = m_ocrProcess->readAllStandardError();
            emit resStatus(QString("OCR failed...，code=%1").arg(code));
        }

        QByteArray output = m_ocrProcess->readAllStandardOutput();
        QString result = QString::fromLocal8Bit(output);
        if(result.isEmpty())
        {
            emit resStatus("OCR failed...");
        }
        else
        {
            emit resReady(result);
        }
    });
}

Ocr::~Ocr()
{
    if(!m_ocrProcess)   m_ocrProcess->deleteLater();
    m_ocrProcess=nullptr;

    if(!m_quitProcess)  m_quitProcess->deleteLater();
    m_quitProcess=nullptr;
}
