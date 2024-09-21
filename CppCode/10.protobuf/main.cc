#include "contacts.pb.h"
#include <string>

int main()
{
    // 创建对象
    contacts::contact cont;
    // 初始化值
    cont.set_id(0001);
    cont.set_adders("翻斗大街翻斗花园2号楼1001室");
    cont.set_name("胡图图");
    cont.set_telnumber("13600000001");
    // 序列化
    std::string str = cont.SerializeAsString();
    // 数据传输
    // ......

    // 反序列化
    contacts::contact recv;
    bool res = recv.ParseFromString(str);
    if (res == false)
    {
        std::cout << "反序列化失败" << std::endl;
        return -1;
    }
    // 使用
    std::cout << "ID: " << recv.id() << std::endl;
    std::cout << "Name: " << recv.name() << std::endl;
    std::cout << "Adders: " << recv.adders() << std::endl;
    std::cout << "TelNumber: " << recv.telnumber() << std::endl;
    return 0;
}