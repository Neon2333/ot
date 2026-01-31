#include "pronounce.h"
#include <curl/curl.h>
#include "common.hpp"
#include <filesystem>
#include <string>
#include <fstream>
#include "word.hpp"

using std::string;
using std::ofstream;
namespace fs=std::filesystem;


size_t Pronounce::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

Pronounce::Pronounce(QWidget *parent)
    : QObject(parent)
{
    connect(&m_mediaPlayer, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error) {
        if (error == QMediaPlayer::Error::NoError)
            return;
        qout << "Media player error occurred:" << error;
    });
}

Pronounce::~Pronounce()
{
}

void Pronounce::playPronounationFile(QString audioFileName)
{
    m_audioOutput.setVolume(1.0);
    m_mediaPlayer.setAudioOutput(&m_audioOutput);
    QString soundFile = QString(config::cfg.pronPath + "/" + audioFileName);
    if(!fs::exists(soundFile.toStdString()))
    {
        emit setStatus("音频文件不存在");
        logError("sound file not exists:" + soundFile.toStdString());
        return;
    }
    m_mediaPlayer.setSource(QUrl::fromLocalFile(soundFile));
    m_mediaPlayer.play();
    logInfo("sound file have been played:" + soundFile.toStdString());
}

bool Pronounce::getSoundFile(Language lan, QString text)
{
    // QString encodedText = QUrl::toPercentEncoding(text);
    if(text.isEmpty())
    {
        emit setStatus("请输入文本或使用alt+1/2/3/4截图");
        return false;
    }

    CURL* curl = nullptr;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(!curl)
    {
        logError("curl_easy_init failed");
        return false;
    }

    bool isWord = false;
    string urlPron;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "User-Agent:  Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.212 Safari/537.36");
    headers = curl_slist_append(headers, "Accept: audio/mpeg, audio/*");
    headers = curl_slist_append(headers, "Host: fanyi.baidu.com");
    headers = curl_slist_append(headers, "Connection: keep-alive");
    if(!text.contains(" ") && lan==Language::en)
    {
        //单词
        urlPron = config::cfg.wordURL + text.toStdString();
        headers = curl_slist_append(headers, "Referer: https://api.dictionaryapi.dev/");
        isWord = true;
        qout<<urlPron;
    }
    else
    {
        //语句
        QString lanTmp=(lan==Language::zh ? "zh" : (lan==Language::en ? "en" : "jp"));
        urlPron = config::cfg.sentenceYuyinURL + QString("gettts?lan=%1&text=%2&spd=3&source=web")
                                                           .arg(lanTmp)
                                                           .arg(replaceSpace(text)).toStdString();
        headers = curl_slist_append(headers, "Referer: https://fanyi.baidu.com/");
    }
    qout<<urlPron;

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_URL, urlPron.c_str());
    // curl_easy_setopt(curl, CURLOPT_URL, "api.dictionaryapi.dev/");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    string repBody;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &repBody);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);   //超时时间30s


    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    //验证证书
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
    const char* CAPath = config::cfg.caPath.toUtf8().data();
    curl_easy_setopt(curl, CURLOPT_CAINFO, CAPath);

    //执行 GET 请求
    CURLcode rep;
    rep = curl_easy_perform(curl);

    //检查请求是否成功
    if(rep != CURLE_OK)
    {
        emit setStatus("请求失败");
        logError("curl_easy_perform() failed:" + string(curl_easy_strerror(rep)));
        return false;
    }

    //状态码，非200
    long repCode;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &repCode);
    if(repCode!=200)
    {
        emit setStatus(QString::fromStdString(string("状态码: ") + std::to_string(repCode)));
        logInfo("response code:" + std::to_string(repCode));
        return false;
    }

    //获取Content-Type
    char* contentType;
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType);
    if(string(contentType).find("audio") == string::npos)
    {
        emit setStatus("json格式不对");
        logInfo("Content-Type is not audio");
        return false;
    }

    //清理curl资源
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return true;



    //body处理，保存语音
    if(isWord)
    {
        //从json找audio字段获取audio url，从url下载mp3


    }
    else
    {
        //保存语音文件
        string audioName = text.toStdString() + ".mp3";
        ofstream soundFile(config::cfg.pronPath.toStdString() + "/" + audioName, ofstream::binary);
        if(!soundFile)
        {
            emit setStatus("音频文件下载失败");
            logError("save sound file error: " + text.toStdString() + ".mp3");
            return false;
        }

        soundFile.write(repBody.c_str(), repBody.size());
        soundFile.close();
        emit setStatus("音频文件下载完成");
        emit pronouncationAudioDownloaded(QString::fromUtf8(audioName));
        logInfo("save sound file succeed: " + text.toStdString() + ".mp3");
        return true;
    }
}

size_t Pronounce::parseResponse(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t total_size = size * nmemb;
    // 将返回的数据写入文件或内存等
    qout << std::string(static_cast<char*>(contents), total_size);
    return total_size;
}

