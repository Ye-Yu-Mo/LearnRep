#include <iostream>
#include <string>
#include <curl/curl.h>
#include <cstdlib>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <jsoncpp/json/json.h>
#include <sstream>
#include <iomanip>
#include "../logs/Xulog.h"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp)
{
    size_t total_size = size * nmemb;
    userp->append(static_cast<char *>(contents), total_size);
    return total_size;
}

std::string generateSign(const std::string &appid, const std::string &q, const std::string &salt, const std::string &secret_key)
{
    std::string sign = appid + q + salt + secret_key;
    unsigned char md[EVP_MAX_MD_SIZE];
    unsigned int md_len;

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx)
    {
        std::cerr << "Failed to create context for MD5." << std::endl;
        return "";
    }

    if (EVP_DigestInit_ex(ctx, EVP_md5(), nullptr) != 1 ||
        EVP_DigestUpdate(ctx, sign.c_str(), sign.length()) != 1 ||
        EVP_DigestFinal_ex(ctx, md, &md_len) != 1)
    {
        std::cerr << "Error generating MD5 digest." << std::endl;
        EVP_MD_CTX_free(ctx);
        return "";
    }

    EVP_MD_CTX_free(ctx);

    char buf[33] = {0};
    for (unsigned int i = 0; i < md_len; ++i)
    {
        sprintf(buf + i * 2, "%02x", md[i]);
    }
    return std::string(buf);
}

std::string parseJsonResponse(const std::string &response)
{
    Json::CharReaderBuilder reader;
    Json::Value jsonData;
    std::string errs;

    std::istringstream s(response);
    if (Json::parseFromStream(reader, s, &jsonData, &errs))
    {
        std::string from = jsonData["from"].asString();
        std::string to = jsonData["to"].asString();
        std::string translatedText = jsonData["trans_result"][0]["dst"].asString();

        return translatedText;
    }
    else
    {
        std::cerr << "Failed to parse JSON: " << errs << std::endl;
        exit(0);
    }
}

std::string urlEncode(const std::string &value)
{
    std::ostringstream escaped;
    escaped << std::hex << std::setfill('0');
    for (const char c : value)
    {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            escaped << c;
        }
        else
        {
            escaped << '%' << std::setw(2) << static_cast<int>(static_cast<unsigned char>(c));
        }
    }
    return escaped.str();
}

std::string _translate(const std::string &appid, const std::string &secret_key, const std::string &q, const std::string &from, const std::string &to)
{
    char salt[60];
    sprintf(salt, "%d", rand());

    std::string sign = generateSign(appid, q, salt, secret_key);
    std::string q_encoded = urlEncode(q);
    std::string myurl = "http://api.fanyi.baidu.com/api/trans/vip/translate?appid=" + appid + "&q=" + q_encoded +
                        "&from=" + from + "&to=" + to + "&salt=" + salt + "&sign=" + sign;
    CURL *curl = curl_easy_init();
    std::string response_string;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, myurl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    return response_string;
}

enum
{
    EnToZh,
    ZhToEn,
    Exit
};

std::string Translate(std::string text_to_translate, std::string from, std::string to)
{
    std::ifstream key("key");
    if (!key)
    {
        FATAL("密钥打开失败!");
    }
    std::string appid;
    std::getline(key, appid);
    std::string secret_key;
    std::getline(key, secret_key);
    std::string response = _translate(appid, secret_key, text_to_translate, from, to);
    return parseJsonResponse(response);
}