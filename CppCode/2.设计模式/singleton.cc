#include <iostream>

// class Singleton // 饿汉单例
// {
// public:
//     static Singleton &GetInstance()
//     {
//         return _eton;
//     }

//     int GetData()
//     {
//         return _data;
//     }

// private:
//     Singleton() : _data(114) {};
//     ~Singleton() {};
//     Singleton(const Singleton &) = delete;
//     static Singleton _eton;

//     int _data;
// };

// Singleton Singleton::_eton;

class Singleton
{
public:
    static Singleton& GetInstance()
    {
        static Singleton _eton;
        return _eton;
    }

    int GetData()
    {
        return _data;
    }

private:
    Singleton() : _data(114) {

    };
    ~Singleton() {};
    Singleton(const Singleton &) = delete;

    int _data;
};

int main()
{
    std::cout << Singleton::GetInstance().GetData() << std::endl;
    return 0;
}