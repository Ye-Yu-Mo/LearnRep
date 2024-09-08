#pragma once
#include "level.hpp"
#include "format.hpp"
#include "util.hpp"
#include <fstream>

const std::string logdir = "./log/";

enum WriteStyle
{
    Screen = 114, // 屏幕打印
    OneFile,      // 全部打印到一个文件
    ClassFile     // 按等级分类打印
};

void WriteToOneFile(const std::string &str, const std::string &logname = "logALL")
{
    Xulog::Util::File::createDirectory(logdir);
    std::string fullLogName = logdir + logname;
    std::ofstream out(fullLogName, std::ios_base::app);
    if (!out.is_open())
    {
        std::cout << fullLogName << ":文件打开失败" << std::endl;
    }
    out.write(str.c_str(), str.size());
    out.close();
}

void WriteToClassFile(const std::string &str, Xulog::LogLevel::value val)
{
    std::string logname = "log";
    logname += Xulog::LogLevel::toString(val);
    WriteToOneFile(str, logname);
}

void WriteLog(Xulog::LogMsg msg, Xulog::Formatter fmt, WriteStyle style = Screen)
{
    std::string str = fmt.Format(msg);
    switch (style)
    {
    case Screen:
        std::cout << str;
        break;
    case OneFile:
        WriteToOneFile(str);
        break;
    case ClassFile:
        WriteToClassFile(str, msg._level);
        break;
    }
}