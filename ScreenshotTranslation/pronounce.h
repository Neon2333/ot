#ifndef PRONOUNCE_H
#define PRONOUNCE_H

#include <QWidget>
#include <QObject>
#include "config.hpp"
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>

using std::string;

class Pronounce : public QObject
{
    Q_OBJECT

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

    QMediaPlayer m_mediaPlayer;
    QAudioOutput m_audioOutput;


public:
    explicit Pronounce(QWidget *parent = nullptr);
    ~Pronounce();

    bool getSoundFile(Language lan, QString text);
    static size_t parseResponse(void* contents, size_t size, size_t nmemb, void* userp);
    void playPronounationFile(QString text);

signals:
    void setStatus(QString);
    void pronouncationAudioDownloaded(QString audioName);
};

#endif // PRONOUNCE_H
