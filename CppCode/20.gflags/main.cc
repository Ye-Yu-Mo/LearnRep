#include <gflags/gflags.h>
#include <iostream>
DEFINE_string(ip, "127.0.0.1", "服务器ip地址, 127.0.0.1");
DEFINE_int32(port, 8080, "服务器监听端口号");
DEFINE_bool(debug_enable, true, "启用调试模式");

int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);

    std::cout << FLAGS_ip << std::endl;
    std::cout << FLAGS_port << std::endl;
    std::cout << FLAGS_debug_enable << std::endl;
    return 0;
}