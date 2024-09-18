#pragma once

#include <iostream>
#include <string>
#include <vector>

const std::string HttpSep = "\r\n";

class HttpRequest
{
public:
    HttpRequest()
        : _req_blank(HttpSep)
    {
    }
    bool GetLine(std::string &str, std::string *line)
    {
        auto pos = str.find(HttpSep);
        if (pos == std::string::npos)
            return false;
        *line = str.substr(0, pos);
        str.erase(0, pos + HttpSep.size());
        return true;
    }
    void Deserialize(std::string &request)
    {
        std::string line;
        bool ok = GetLine(request, &line);
        if (!ok)
            return;
        _req_line = line;
        while (true)
        {
            bool ok = GetLine(request, &line);
            if (ok && line.empty())
            {
                _req_content = request;
                break;
            }
            else if (ok && !line.empty())
            {
                _req_header.push_back(line);
            }
            else
            {
                break;
            }
        }
    }
    void DebugHttp()
    {
        std::cout << "_req_line" << _req_line << std::endl;
        for (auto &line : _req_header)
        {
            std::cout << "----> " << line << std::endl;
        }
        std::cout << "_req_blank: " << _req_blank << std::endl;
        std::cout << "_req_content: " << _req_content << std::endl;
    }
    ~HttpRequest()
    {
    }

private:
    std::string _req_line;
    std::vector<std::string> _req_header;
    std::string _req_blank;
    std::string _req_content;
};