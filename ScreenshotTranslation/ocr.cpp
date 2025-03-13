#include "ocr.h"
#include <QSettings>
#include <QDebug>
#include "config.hpp"


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
    qDebug()<<"m_ocrProcess="<<config::ocrexePath;
    m_ocrProcess->setArguments(m_command);
    qDebug()<<"args="<<m_command;


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
        m_ocrProcess->waitForFinished(10000);
        qDebug()<<"ocr start";
    }
}

QProcess *Ocr::process()
{
    return m_ocrProcess;
}

void Ocr::useConfig(QString language)
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
    setting.setValue("ScreenshotOCR/ocr.language", language);
    qDebug()<<"language00="<<language;
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
    qDebug()<<"m_ocrProcess="<<config::ocrexePath;
    m_ocrProcess->setArguments(m_command);
    qDebug()<<"args="<<m_command;

    //m_ocrProcess重新new了之前的connect失效
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

Ocr::~Ocr()
{
    if(m_ocrProcess!=nullptr)   m_ocrProcess->deleteLater();
    m_ocrProcess=nullptr;
}
