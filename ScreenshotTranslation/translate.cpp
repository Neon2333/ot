#include "translate.h"
#include <cstring>
#include <curl/curl.h>
#include "openssl/md5.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/pointer.h"
#include "rapidjson/stream.h"
#include <random>
#include <QRegularExpression>
#include "common.hpp"

#define MIN 0
#define MAX 4294967295

using std::string;
using namespace config;
using namespace rapidjson;

Translate::Translate() {}
Translate::~Translate(){}

void Translate::doTranslate(const string& text, Language languageTranslateFrom, Language languageTranslateTo)
{
    if(text.empty())
    {
        emit setStatus("请输入文本或使用alt+1/2/3/4截图");
        return;
    }
    if(text.find(" ")==string::npos && languageTranslateFrom==Language::en)//英文单词
    // if(false)
    {
        qout<<"word...api.dictionaryapi.dev";
        string urlWordTrans = config::cfg.wordURL + text;
        qout<<urlWordTrans;

        try
        {
            CURL* curl = nullptr;
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();
            if(!curl)
            {
                logError("curl_easy_init() failed");
            }
            curl_easy_setopt(curl, CURLOPT_URL, urlWordTrans.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            string repBody;
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &repBody);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
            const char* CAPath = config::cfg.caPath.toUtf8().data();
            curl_easy_setopt(curl, CURLOPT_CAINFO, CAPath);

            CURLcode rep;
            rep = curl_easy_perform(curl);

            if(rep != CURLE_OK)
            {
                emit setStatus("请求失败");
                logError("curl_easy_perform() failed:" + string(curl_easy_strerror(rep)));
                return;
            }

            long repCode;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &repCode);
            if(repCode!=200)
            {
                emit setStatus(QString::fromStdString(string("状态码: ") + std::to_string(repCode)));
                logInfo("response code:" + std::to_string(repCode));
                return;
            }

            //清理curl资源
            curl_easy_cleanup(curl);
            curl_global_cleanup();


            //解析body到word
            qout<<repBody;
            Document doc;
            doc.Parse(repBody.c_str(), repBody.size());

            emit translateFinished(QString::fromStdString(repBody));
            emit setStatus("翻译完成");
        }
        catch(std::runtime_error& e)
        {
            spdlog::error("【" + nowStr() + "】	" + "parse json error:" + e.what());
        }
        catch (const std::exception& e)
        {
            spdlog::error("【" + nowStr() + "】	" + e.what());
        }
    }
    else    //非英文单词
    {
        qout<<"sentence...api.fanyi.baidu.com";

#pragma region md5加密sign
        std::mt19937 gen((unsigned int)time(NULL));
        std::uniform_int_distribution<unsigned int> distrib(MIN, MAX);

        string salt = std::to_string(distrib(gen));
        string sign = config::cfg.fanyiID+text+salt+config::cfg.fanyiKey;
        unsigned char digest[MD5_DIGEST_LENGTH];
        MD5_CTX context;
        MD5_Init(&context); // 初始化 MD5 上下文
        MD5_Update(&context, sign.c_str(), sign.size()); // 更新数据
        MD5_Final(digest, &context);

        char signMD5[33]; // 存储转换后的十六进制字符串
        for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
        {
            sprintf(&signMD5[i * 2], "%02x", (unsigned int)digest[i]); // 将每个字节转换为两位的十六进制字符
        }// 计算最终的 MD5 值
#pragma endregion md5加密sign

        //最终请求url
        string urlSentenTrans = config::cfg.fanyiURL
                       +"api/trans/vip/translate?appid=" + config::cfg.fanyiID
                       +"&q=" + replaceSpace(text).toStdString()
                       +"&from=" + (languageTranslateFrom==Language::zh?"zh" : (languageTranslateFrom==Language::en?"en":"jp"))
                       +"&to=" +(languageTranslateTo==Language::zh?"zh" : (languageTranslateTo==Language::en?"en":"jp"))
                       +"&salt=" + salt
                       +"&sign=" + std::string(signMD5);
        qout<<urlSentenTrans;

        try
        {
            CURL* curl = nullptr;
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();
            if(!curl)
            {
                logError("curl_easy_init() failed");
            }
            curl_easy_setopt(curl, CURLOPT_URL, urlSentenTrans.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            string repBody;
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &repBody);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

            //验证证书
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
            const char* CAPath = config::cfg.caPath.toUtf8().data();
            curl_easy_setopt(curl, CURLOPT_CAINFO, CAPath);

            CURLcode rep;
            rep = curl_easy_perform(curl);

            //检查请求是否成功
            if(rep != CURLE_OK)
            {
                emit setStatus("请求失败");
                logError("curl_easy_perform() failed:" + string(curl_easy_strerror(rep)));
                return;
            }

            //状态码，非200
            long repCode;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &repCode);
            if(repCode!=200)
            {
                emit setStatus(QString::fromStdString(string("状态码: ") + std::to_string(repCode)));
                logInfo("response code:" + std::to_string(repCode));
                return;
            }

            //清理curl资源
            curl_easy_cleanup(curl);
            curl_global_cleanup();

            //解析body，拿dst字段
            qout<<repBody;
            Document doc;
            doc.Parse(repBody.c_str(), repBody.size());
            if(!doc.HasMember("trans_result"))
            {
                emit setStatus("json格式不对");
                logInfo("json格式不对");
                return;
            }
            const Value& transReltArr = doc["trans_result"];

            if(!transReltArr.IsArray() || transReltArr.Size()<=0)
            {
                emit setStatus("json格式不对");
                logInfo("json格式不对");
                return;
            }
            const Value& obj = transReltArr[0];

            if(!obj.HasMember("dst"))
            {
                emit setStatus("json格式不对");
                logInfo("json格式不对");
                return;
            }
            const Value& rsltTmp = obj["dst"];

            if(!rsltTmp.IsString())
            {
                emit setStatus("json格式不对");
                logInfo("json格式不对");
                return;
            }
            emit translateFinished(QString::fromStdString(rsltTmp.GetString()));
            emit setStatus("翻译完成");
        }
        catch(std::runtime_error& e)
        {
            spdlog::error("【" + nowStr() + "】	" + "parse json error:" + e.what());
        }
        catch (const std::exception& e)
        {
            spdlog::error("【" + nowStr() + "】	" + e.what());
        }
    }
}

