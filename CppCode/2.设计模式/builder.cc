#include <iostream>
#include <string>
#include <memory>

// 抽象产品类：Phone
class Phone
{
public:
    Phone() {}
    void setBoard(const std::string &board)
    {
        _board = board;
    }
    void setDisplay(const std::string &display)
    {
        _display = display;
    }
    void show()
    {
        std::cout << "OS: " << _os << " | Board: " << _board << " | Display: " << _display << std::endl;
    }
    virtual void setOs() = 0;

protected:
    std::string _board;
    std::string _display;
    std::string _os;
};

// 具体产品类：iPhone
class iPhone : public Phone
{
public:
    void setOs() override
    {
        _os = "iOS 17";
    }
};

// 抽象Builder类
class Builder
{
public:
    virtual void buildBoard(const std::string &board) = 0;
    virtual void buildDisplay(const std::string &display) = 0;
    virtual void buildOs() = 0;
    virtual std::shared_ptr<Phone> build() = 0;
};

// 具体产品的Builder类：iPhoneBuilder
class iPhoneBuilder : public Builder
{
public:
    iPhoneBuilder() : _phone(std::make_shared<iPhone>()) {}

    void buildBoard(const std::string &board) override
    {
        _phone->setBoard(board);
    }

    void buildDisplay(const std::string &display) override
    {
        _phone->setDisplay(display);
    }

    void buildOs() override
    {
        _phone->setOs();
    }

    std::shared_ptr<Phone> build() override
    {
        return _phone;
    }

private:
    std::shared_ptr<Phone> _phone;
};

// Director类：用于控制构建流程
class Director
{
public:
    Director(Builder *bd) : _builder(bd) {}

    void construct(const std::string &board, const std::string &display)
    {
        _builder->buildBoard(board);
        _builder->buildDisplay(display);
        _builder->buildOs();
    }

private:
    Builder *_builder;
};

// 主函数
int main()
{
    // 创建iPhone构建器
    Builder *builder = new iPhoneBuilder();
    
    // 通过Director构建iPhone
    std::unique_ptr<Director> director(new Director(builder));
    director->construct("华硕", "三星");

    // 获取构建好的iPhone
    std::shared_ptr<Phone> iphone = builder->build();
    iphone->show();

    delete builder; // 释放内存
    return 0;
}
