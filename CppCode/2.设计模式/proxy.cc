#include <iostream>
#include <string>

class RentHouse
{
public:
    virtual void rentHouse() = 0;
};

class Landlord : public RentHouse
{
public:
    void rentHouse() override
    {
        std::cout << "租房子了" << std::endl;
    }
};

class Intermediary : public RentHouse
{
public:
    void rentHouse() override
    {
        std::cout << "发布公告" << std::endl;
        std::cout << "看房" << std::endl;
        _landlord.rentHouse();
        std::cout << "售后" << std::endl;
    }

private:
    Landlord _landlord;
};

int main()
{
    Intermediary intermediary;
    intermediary.rentHouse();
    return 0;
}