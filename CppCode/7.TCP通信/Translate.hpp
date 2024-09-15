#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>

const std::string unknow = "unknow";
const std::string mydict = "./resource/dict.txt";
const std::string sep = " ";

class Translate
{
public:
    Translate(std::string dict_path = mydict)
        : _dict_path(dict_path)
    {
        LoadDict();
        Prase();
    }
    void LoadDict()
    {
        std::ifstream in(_dict_path);
        std::string line;
        while (std::getline(in, line))
        {
            lines.push_back(line);
        }
        in.close();
        std::cout << "词典加载完成,路径为:" << _dict_path.c_str() << std::endl;
    }
    void Prase()
    {
        for (auto &line : lines)
        {
            auto pos = line.find(sep);
            if (pos == std::string::npos)
                continue;
            else
            {
                std::string word = line.substr(0, pos);
                std::string trans = line.substr(pos + sep.size());
                _dict.insert(std::make_pair(word, trans));
            }
        }
        std::cout << "词典解析完成\n";
    }
    void debug()
    {
        for (auto &elem : _dict)
        {
            std::cout << elem.first << " : " << elem.second << std::endl;
        }
    }
    std::string Excute(const std::string &word)
    {
        auto iter = _dict.find(word);
        if (iter == _dict.end())
            return unknow;
        else
            return _dict[word];
    }
    ~Translate()
    {
    }

private:
    std::string _dict_path;
    std::unordered_map<std::string, std::string> _dict;
    std::vector<std::string> lines;
};