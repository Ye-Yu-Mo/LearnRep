#include "INIParser.hpp"
#include "../logs/Xulog.h"
#include <unordered_map>
#include <string>
namespace XuServer
{
    class Config
    {
    public:
        using config_map = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;
        Config(const std::string &filename)
        {
            INIParser parser;
            auto callBack = std::bind(&Config::handleKeyValue, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            int res = parser.parse(filename, callBack);
            if (res != 0)
            {
                ERROR("配置读取失败！");
            }
        }
        std::string get(const std::string &section, const std::string &name)
        {
            auto sec_it = _config_data.find(section);
            if (sec_it != _config_data.end())
            {
                auto name_it = sec_it->second.find(name);
                if (name_it != sec_it->second.end())
                    return name_it->second;
            }
            return "";
        }
        int handleKeyValue(const std::string &section, const std::string &name, const std::string &value)
        {
            _config_data[section][name] = value;
            return 1;
        }

    private:
        config_map _config_data;
    };
}

int main()
{
    XuServer::Config config("./config.ini");
    // 获取并打印配置信息
    std::cout << "Server Port: " << config.get("Server", "port") << std::endl;
    std::cout << "StdoutSink Color: " << config.get("StdoutSink", "color") << std::endl;
    std::cout << "FileSink Path: " << config.get("FileSink", "path") << std::endl;
    std::cout << "RollBySize Path: " << config.get("RollBySize", "path") << std::endl;
    std::cout << "RollByTime Type: " << config.get("RollByTime", "type") << std::endl;
    std::cout << "DatabaseSink Path: " << config.get("DataBaseSink", "path") << std::endl;

    return 0;
}