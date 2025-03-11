#include "ocr.h"
#include <QSettings>
#include <QDebug>

Ocr::Ocr() {}

Ocr::Ocr(vector<QString> commands)
{
    m_command.clear();
    m_command << "umi-ocr";
    for(int i=0;i<commands.size();i++)
    {
        m_command << commands.at(i);
    }

    m_ocrProcess = new QProcess;
    m_ocrProcess->setProgram(config::ocrexePath);
    qDebug()<<"m_ocrProcess="<<config::ocrexePath;
    m_ocrProcess->setArguments(m_command);
    qDebug()<<"args="<<m_command;


    // QObject::connect(m_ocrProcess, &QProcess::readyReadStandardOutput, [this](){
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
            emit resReady(QString("OCR 识别失败，code=%1").arg(code));
        }

        QByteArray output = m_ocrProcess->readAllStandardOutput();
        QString result = QString::fromLocal8Bit(output);
        if(result.isEmpty())
        {
            emit resReady("OCR 识别失败");
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
        m_ocrProcess->waitForFinished(5000);
        qDebug()<<"ocr start";
    }
}

QProcess *Ocr::process()
{
    return m_ocrProcess;
}

void Ocr::hideOcrWindow()
{
    QSettings setting(config::umiocrConfig, QSettings::IniFormat);
    QVariant isPopOcrWindow = setting.value("ScreenshotOCR/action.popMainWindow");
    if(isPopOcrWindow.toBool())
    {
        setting.setValue("ScreenshotOCR/action.popMainWindow", false);
    }
}

void Ocr::setCommands(vector<QString> commands)
{
    m_command.clear();
    m_command << "umi-ocr";
    for(int i=0;i<commands.size();i++)
    {
        m_command << commands.at(i);
    }

    if(m_ocrProcess!=nullptr)   m_ocrProcess->deleteLater();
    m_ocrProcess = new QProcess;
    m_ocrProcess->setProgram(config::ocrexePath);
    qDebug()<<"m_ocrProcess="<<config::ocrexePath;
    m_ocrProcess->setArguments(m_command);
    qDebug()<<"args="<<m_command;
}


Ocr::~Ocr()
{
    if(m_ocrProcess!=nullptr)   m_ocrProcess->deleteLater();
    m_ocrProcess=nullptr;
}
