/// @file INIParser.hpp
/// @brief 解析ini风格文件的功能 支持段落 键值对 注释
#pragma once
#include <string>
#include <fstream>
#include <functional>
#include <iostream>

/**
 * @class INIParser
 * @brief 简单的INI文件解析器
 *
 * 该类提供解析INI风格文件的功能，支持段落、键值对以及注释。
 * 支持多行值和UTF-8 BOM。
 */
class INIParser
{
public:
    /**
     * @brief 解析给定的INI文件
     *
     * @param filename INI文件的路径
     * @param handler 处理每个键值对的函数
     * @return 0表示成功，返回解析错误的行号，-1表示文件打开错误，-2表示内存分配错误
     */
    int parse(const std::string &filename,
              std::function<int(const std::string &, const std::string &, const std::string &)> handler);

private:
    /**
     * @brief 去掉字符串末尾的空白字符
     *
     * @param s 要处理的字符串
     * @return 去掉空白后的字符串
     */
    std::string rstrip(std::string s);

    /**
     * @brief 跳过字符串开头的空白字符
     *
     * @param s 要处理的字符串
     * @return 指向第一个非空白字符的指针
     */
    const char *lskip(const char *s);

    /**
     * @brief 在字符串中查找字符或注释的起始位置
     *
     * @param s 要搜索的字符串
     * @param c 要查找的字符
     * @return 查找到的字符或字符串末尾的指针
     */
    const char *findCharOrComment(const char *s, char c);
};

std::string INIParser::rstrip(std::string s)
{
    while (!s.empty() && isspace(s.back()))
    {
        s.pop_back();
    }
    return s;
}

const char *INIParser::lskip(const char *s)
{
    while (*s && isspace(static_cast<unsigned char>(*s)))
    {
        s++;
    }
    return s;
}

const char *INIParser::findCharOrComment(const char *s, char c)
{
    bool wasWhitespace = false;
    while (*s && *s != c && !(wasWhitespace && *s == ';'))
    {
        wasWhitespace = isspace(static_cast<unsigned char>(*s));
        s++;
    }
    return s;
}

int INIParser::parse(const std::string &filename,
                     std::function<int(const std::string &, const std::string &, const std::string &)> handler)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return -1; // 文件打开错误
    }

    std::string line;
    std::string section;
    std::string prevName;

    int lineno = 0;
    while (std::getline(file, line))
    {
        lineno++;
        line = rstrip(line);
        const char *start = lskip(line.c_str());

        if (*start == ';' || *start == '#')
        {
            continue; // 注释行
        }
        else if (*start == '[')
        {
            const char *end = findCharOrComment(start + 1, ']');
            if (*end == ']')
            {
                section = std::string(start + 1, end);
                prevName.clear();
            }
            else
            {
                return lineno; // 段落错误
            }
        }
        else if (*start)
        {
            const char *end = findCharOrComment(start, '=');
            if (*end != '=')
            {
                end = findCharOrComment(start, ':');
            }
            if (*end == '=' || *end == ':')
            {
                std::string name(start, end);
                std::string value(lskip(end + 1));
                value = rstrip(value);

                // 找到有效的键值对，调用处理函数
                prevName = name;
                if (handler(section, name, value) == 0)
                {
                    return lineno; // 处理函数错误
                }
            }
            else
            {
                return lineno; // 键值对错误
            }
        }
    }

    return 0; // 成功
}

