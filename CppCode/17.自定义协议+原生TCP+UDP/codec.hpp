#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <cstring>
#include <jsoncpp/json/json.h>

class LogLevel
{
public:
    /**
     * @enum value
     * @brief 日志等级的枚举值
     *
     * 定义日志的不同等级，从UNKNOW到OFF。
     */
    enum class value
    {
        UNKNOW = 0, /**< 未知日志等级 */
        DEBUG,      /**< 调试信息 */
        INFO,       /**< 普通信息 */
        WARN,       /**< 警告信息 */
        ERROR,      /**< 错误信息 */
        FATAL,      /**< 严重错误 */
        OFF         /**< 日志关闭 */
    };
    /**
     * @brief 将日志等级转换为对应的字符串
     *
     * @param level 日志等级的枚举值
     * @return 对应的日志等级字符串
     *
     * 该函数根据传入的日志等级枚举值，返回对应的字符串表示。
     */
    static const char *toString(LogLevel::value level)
    {
        switch (level)
        {
        case LogLevel::value::DEBUG:
            return "DEBUG";
            break;
        case LogLevel::value::INFO:
            return "INFO";
            break;
        case LogLevel::value::WARN:
            return "WARN";
            break;
        case LogLevel::value::ERROR:
            return "ERROR";
            break;
        case LogLevel::value::FATAL:
            return "FATAL";
            break;
        case LogLevel::value::OFF:
            return "OFF";
            break;
        default:
            return "UNKNOW";
        }
    }
    /// @brief 从字符串转换成日志等级
    /// @param level 日志等级字符串
    /// @return 日志等级
    static LogLevel::value fromString(const std::string &level)
    {
        if (level == "DEBUG")
            return LogLevel::value::DEBUG;
        else if (level == "INFO")
            return LogLevel::value::INFO;
        else if (level == "WARN")
            return LogLevel::value::WARN;
        else if (level == "ERROR")
            return LogLevel::value::ERROR;
        else if (level == "FATAL")
            return LogLevel::value::FATAL;
        else if (level == "OFF")
            return LogLevel::value::OFF;
        else
            return LogLevel::value::UNKNOW;
    }
};
struct LogMsg
{
    time_t _ctime;          ///< 日志产生的时间戳
    size_t _line;           ///< 行号
    std::thread::id _tid;   ///< 线程ID
    LogLevel::value _level; ///< 日志等级
    std::string _file;      ///< 源文件名称
    std::string _logger;    ///< 日志器
    std::string _payload;   ///< 有效载荷数据
    LogMsg() {}

    /**
     * @brief LogMsg 构造函数
     *
     * @param level 日志等级
     * @param line 源代码行号
     * @param file 源文件名称
     * @param logger 日志器名称
     * @param msg 日志主体消息
     *
     * 构造日志消息对象，并初始化所有相关字段。
     */
    LogMsg(LogLevel::value level,
           size_t line,
           const std::string file,
           const std::string logger,
           const std::string msg) : _ctime((size_t)time(nullptr)),
                                    _line(line),
                                    _tid(std::this_thread::get_id()),
                                    _level(level),
                                    _file(file),
                                    _logger(logger),
                                    _payload(msg)
    {
    }

    Json::Value toJson() const
    {
        Json::Value json;
        json["ctime"] = static_cast<Json::Int64>(_ctime);
        json["line"] = _line;
        json["tid"] = std::to_string(*(unsigned long int *)(&_tid));
        json["level"] = LogLevel::toString(_level);
        json["file"] = _file;
        json["logger"] = _logger;
        json["payload"] = _payload;
        return json;
    }

    static LogMsg fromJson(const Json::Value &json)
    {
        LogMsg msg;
        msg._ctime = json["ctime"].asInt64();
        msg._line = json["line"].asUInt();
        msg._tid = std::thread::id(std::stoul(json["tid"].asString()));
        msg._level = LogLevel::fromString(json["level"].asString());
        msg._file = json["file"].asString();
        msg._logger = json["logger"].asString();
        msg._payload = json["payload"].asString();
        return msg;
    }
};