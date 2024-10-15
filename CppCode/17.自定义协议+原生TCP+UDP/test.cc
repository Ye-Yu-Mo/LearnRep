#include "codec.hpp"
int main()
{
    LogMsg logmsg(LogLevel::value::DEBUG, __LINE__, __FILE__, "root", "debug log...");
    Json::StreamWriterBuilder writer;
    std::string jsonString = Json::writeString(writer, logmsg.toJson());
    std::cout << "序列化为Json格式：\n"
              << jsonString << std::endl;
    Json::CharReaderBuilder reader;
    Json::Value root;
    std::istringstream(jsonString) >> root;
    LogMsg a_msg = LogMsg::fromJson(root);

    if (logmsg._file == a_msg._file)
        std::cout << "文件名序列化和反序列化成功！" << std::endl;
    else
        std::cout << "文件名序列化和反序列化失败！" << std::endl;

    if (logmsg._level == a_msg._level)
        std::cout << "日志等级序列化和反序列化成功！" << std::endl;
    else
        std::cout << "日志等级序列化和反序列化失败！" << std::endl;

    if (logmsg._line == a_msg._line)
        std::cout << "行号序列化和反序列化成功！" << std::endl;
    else
        std::cout << "行号序列化和反序列化失败！" << std::endl;

    if (logmsg._logger == a_msg._logger)
        std::cout << "日志器名称序列化和反序列化成功！" << std::endl;
    else
        std::cout << "日志器名称序列化和反序列化失败！" << std::endl;

    if (logmsg._payload == a_msg._payload)
        std::cout << "日志内容序列化和反序列化成功！" << std::endl;
    else
        std::cout << "日志内容序列化和反序列化失败！" << std::endl;

    if (logmsg._tid == a_msg._tid)
        std::cout << "线程id序列化和反序列化成功！" << std::endl;
    else
    {
        std::cout << "线程id序列化和反序列化失败！" << std::endl;
    }

    if (logmsg._ctime == a_msg._ctime)
        std::cout << "时间戳序列化和反序列化成功！" << std::endl;
    else
        std::cout << "时间戳序列化和反序列化失败！" << std::endl;

    return 0;
}