#include "translate.h"
#include <cstring>
#include "httplib.h"
#include "openssl/md5.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/pointer.h"
#include "rapidjson/stream.h"
#include <random>
#include <QDebug>
#include <QRegularExpression>

#define MIN 0
#define MAX 4294967295

using std::string;
using namespace config;
using namespace rapidjson;

Translate::Translate() {}
Translate::~Translate(){}

QString Translate::doTranslate(const string &text, Language languageTranslateFrom, Language languageTranslateTo)
{
    //md5加密sign
    std::mt19937 gen((unsigned int)time(NULL));
    std::uniform_int_distribution<unsigned int> distrib(MIN, MAX);

    string salt = std::to_string(distrib(gen));
    string sign = fanyiID+text+salt+fanyiKey;
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_CTX context;
    MD5_Init(&context); // 初始化 MD5 上下文
    MD5_Update(&context, sign.c_str(), sign.size()); // 更新数据
    MD5_Final(digest, &context);
    char signMD5[33]; // 存储转换后的十六进制字符串
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        sprintf(&signMD5[i * 2], "%02x", (unsigned int)digest[i]); // 将每个字节转换为两位的十六进制字符
    }// 计算最终的 MD5 值

    //最终请求url
    string rqURL = "/api/trans/vip/translate?appid=" + fanyiID
                   + "&q=" + text
                   +"&from=" + (languageTranslateFrom==Language::zh?"zh" : (languageTranslateFrom==Language::en?"en":"jp"))
                   +"&to=" +(languageTranslateTo==Language::zh?"zh" : (languageTranslateTo==Language::en?"en":"jp"))
                   +"&salt=" + salt
                   +"&sign=" + std::string(signMD5);

    httplib::Client cli(fanyiURL);
    // cli.enable_server_certificate_verification(true);

    //http请求
    auto rep = cli.Get(rqURL.c_str());
    QString rslt;
    if(rep && rep->status == 200)
    {
        std::string repBody = rep->body;
        try
        {
            Document doc;
            doc.Parse(repBody.c_str(), repBody.size());
            qDebug()<<"5";
            if(doc.HasMember("trans_result"))
            {
                const Value& transReltArr = doc["trans_result"];
                if(transReltArr.IsArray()&&transReltArr.Size()>0)
                {
                    const Value& obj = transReltArr[0];
                    if(obj.HasMember("dst"))
                    {
                        const Value& rsltTmp = obj["dst"];
                        if(rsltTmp.IsString())
                        {
                            rslt = QString::fromStdString(rsltTmp.GetString());
                            qDebug()<<"rslt="<<rslt;
                            emit translateFinished(rslt);
                        }
                        else
                        {
                            qDebug()<<"dst not string...";
                            rslt = "dst not string...";
                        }
                    }
                    else
                    {
                        qDebug()<<"dst not exists...";
                        rslt = "dst not exists...";
                    }
                }
                else
                {
                    qDebug()<<"trans_result empty...";
                    rslt = "trans_result empty...";
                }
            }
            else
            {
                qDebug()<<"trans_result not exists...";
                rslt = "trans_result not exists...";
            }

        }
        catch(std::runtime_error& e)
        {
            qDebug()<<"parse json error:    "<<e.what();
            std::cerr<<"parse json error:    "<<e.what();
        }
    }
    else
    {
        rslt = QString("ERROR: GET response code: %1").arg(rep->status);
        emit translateFinished(rslt);

    }
    return rslt;
}

