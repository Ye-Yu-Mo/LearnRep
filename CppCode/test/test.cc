#include <iostream>
#include <regex>
#include <string>

std::string convertToRegex(const std::string& format) {
    std::string regex = format;

    // 替换格式符号为对应的正则表达式
    regex = std::regex_replace(regex, std::regex("%d\\{?\\w?:?\\w?\\}?"), R"((\d{2}-\d{2}-\d{2}))"); // 日期
    regex = std::regex_replace(regex, std::regex("%T"), R"((\d{2}:\d{2}:\d{2}))"); // 时间
    regex = std::regex_replace(regex, std::regex("%t"), R"((\d+))"); // 线程ID
    regex = std::regex_replace(regex, std::regex("%p"), R"((\w+))"); // 日志级别
    regex = std::regex_replace(regex, std::regex("%c"), R"([\w]+)"); // 日志器名称
    regex = std::regex_replace(regex, std::regex("%f"), R"([\w\.]+)"); // 文件名
    regex = std::regex_replace(regex, std::regex("%l"), R"((\d+))"); // 行号
    regex = std::regex_replace(regex, std::regex("%m"), R"((.*))"); // 日志消息

    // 处理方括号
    regex = std::regex_replace(regex, std::regex(R"(\[)"), R"(\[)"); // 处理开方括号
    regex = std::regex_replace(regex, std::regex(R"(\])"), R"(\])"); // 处理闭方括号

    return regex;
}

int main() {
    std::string format = "[%d{%y-%m-%d|%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n";
    std::string regex_str = convertToRegex(format);

    // 定义日志信息
    std::string log = "[24-10-14|14:49:02][140516360988480][root][test.cc:17][FATAL]   测试结束";
    
    // 使用正则表达式进行匹配
    std::regex log_regex(regex_str);
    std::smatch match;

    // 使用 std::regex_match 检查整个日志字符串的匹配
    if (std::regex_match(log, match, log_regex)) {
        std::cout << "匹配成功!" << std::endl;
        // 提取匹配内容
        std::cout << "日期: " << match[1] << std::endl;          // 日期
        std::cout << "时间: " << match[2] << std::endl;          // 时间
        std::cout << "线程ID: " << match[3] << std::endl;       // 线程ID
        std::cout << "日志器名称: " << match[4] << std::endl;  // 日志器名称
        std::cout << "文件名: " << match[5] << std::endl;      // 文件名
        std::cout << "行号: " << match[6] << std::endl;        // 行号
        std::cout << "日志级别: " << match[7] << std::endl;    // 日志级别
        std::cout << "日志消息: " << match[8] << std::endl;     // 日志消息
    } else {
        std::cout << "匹配失败!" << std::endl;
    }

    return 0;
}
