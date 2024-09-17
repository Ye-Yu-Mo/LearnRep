#pragma once

#include <iostream>
#include <memory>
#include <jsoncpp/json/json.h>

// 定制协议
namespace Protocol
{
    const std::string ProtSep = " ";
    const std::string LineBreakSep = "\n";

    // 对报文进行打包
    // "len\nx op y\n" 这是一个完整报文，以'\n'为分界
    std::string Encode(const std::string &message)
    {
        std::string len = std::to_string(message.size());
        std::string package = len + LineBreakSep + message + LineBreakSep;
        return package;
    }

    // 对报文解包，判断报文的完整性，正确处理有边界的报文
    bool Decode(std::string &package, std::string *message)
    {
        auto pos = package.find(LineBreakSep);
        if (pos == std::string::npos)
            return false;
        std::string lens = package.substr(0, pos);
        int messagelen = std::stoi(lens);
        int total = lens.size() + messagelen + 2 * LineBreakSep.size();
        if (package.size() < total)
            return false;
        *message = package.substr(pos + LineBreakSep.size(), messagelen);
        package.erase(0, total);
        return true;
    }

    class Request
    {
    public:
        Request()
            : _data_x(0), _data_y(0), _oper(0)
        {
        }
        Request(int x, int y, char op)
            : _data_x(x), _data_y(y), _oper(op)
        {
        }
        void Inc()
        {
            _data_x++;
            _data_y++;
        }
        // 结构化数据->字符串
        // 利用条件编译控制自行定义和Json定义
        bool Serialize(std::string *out)
        {
#ifdef SelfDefine
            *out = std::to_string(_data_x) + ProtSep + _oper + ProtSep + std::to_string(_data_y);
            return true;
#else
            Json::Value root;
            root["datax"] = _data_x;
            root["datay"] = _data_y;
            root["oper"] = _oper;
            Json::FastWriter writer;
            *out = writer.write(root);
            return true;
#endif
        }
        // 字符串->结构化数据
        bool Deserialize(std::string &in)
        {
#ifdef SelfDefine
            auto left = in.find(ProtSep);
            if (left == std::string::npos)
                return false;
            auto right = in.rfind(ProtSep);
            if (right == std::string::npos)
                return false;

            _data_x = std::stoi(in.substr(0, left));
            _data_y = std::stoi(in.substr(right + ProtSep.size()));
            std::string oper = in.substr(left + ProtSep.size(), right - (left + ProtSep.size()));
            if (oper.size() != 1)
                retrun false;
            _oper = oper[0];
            return true;
#else
            Json::Value root;
            Json::Reader reader;
            bool res = reader.parse(in, root);
            if (res)
            {
                _data_x = root["datax"].asInt();
                _data_y = root["datay"].asInt();
                _oper = root["oper"].asInt();
            }
            return res;
#endif
        }
        int GetX() { return _data_x; }
        int GetY() { return _data_y; }
        char GetOper() { return _oper; }

    private:
        // 运算格式 _data_x _oper _data_y
        int _data_x;
        int _data_y;
        char _oper;
    };

    class Response
    {
    public:
        Response()
            : _result(0), _code(0)
        {
        }
        Response(int result, int code)
            : _result(result), _code(code)
        {
        }
        bool Serialize(std::string *out)
        {
#ifdef SelfDefine
            *out = std::to_string(_result) + ProtSep + std::to_string(_code);
            return true;
#else
            Json::Value root;
            root["result"] = _result;
            root["code"] = _code;
            Json::FastWriter writer;
            *out = writer.write(root);
            return true;
#endif
        }
        bool Deserialize(std::string &in)
        {
#ifdef SelfDefine
            auto pos = in.find(ProtSep);
            if (pos == std::string::npos)
                return false;
            _result = std::stoi(in.substr(0, pos));
            _code = std::stoi(in.substr(pos + ProtSep.size()));
            return true;
#else
            Json::Value root;
            Json::Reader reader;
            bool res = reader.parse(in, root);
            if (res)
            {
                _result = root["result"].asInt();
                _code = root["code"].asInt();
            }
            return res;
#endif
        }
        void SetResult(int res) { _result = res; }
        void SetCode(int code) { _code = code; }
        int GetResult() { return _result; }
        int GetCode() { return _code; }

    private:
        // 返回格式 "len\nresult code\n"
        int _result; // 运算结果
        int _code;   // 结果码
    };
    // 简单工厂模式
    class Factory
    {
    public:
        std::shared_ptr<Request> BuildRequest()
        {
            return std::make_shared<Request>();
        }
        std::shared_ptr<Request> BuildRequest(int x, int y, char op)
        {
            return std::make_shared<Request>(x, y, op);
        }
        std::shared_ptr<Response> BuildResponse()
        {
            return std::make_shared<Response>();
        }
        std::shared_ptr<Response> BuildResponse(int result, int code)
        {
            return std::make_shared<Response>(result, code);
        }
    };
}