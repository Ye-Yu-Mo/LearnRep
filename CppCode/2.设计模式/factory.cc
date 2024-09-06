#include <iostream>
#include <memory>

class Fruit
{
public:
    virtual void name() = 0;
};

class Apple : public Fruit
{
public:
    void name() override
    {
        std::cout << "苹果" << std::endl;
    }
};

class Banana : public Fruit
{
public:
    void name() override
    {
        std::cout << "香蕉" << std::endl;
    }
};

class Animal
{
public:
    virtual void name() = 0;
};

class Cat : public Animal
{
public:
    void name() override
    {
        std::cout << "小猫" << std::endl;
    }
};

class Dog : public Animal
{
public:
    void name() override
    {
        std::cout << "小狗" << std::endl;
    }
};

class Factory
{
public:
    virtual std::shared_ptr<Fruit> CreateFruit(const std::string &name) = 0;
    virtual std::shared_ptr<Animal> CreateAnimal(const std::string &name) = 0;
};

class AnimalFactory : public Factory
{
public:
    std::shared_ptr<Fruit> CreateFruit(const std::string &name) override
    {
        return nullptr;
    }
    std::shared_ptr<Animal> CreateAnimal(const std::string &name) override
    {
        if (name == "小猫")
        {
            return std::make_shared<Cat>();
        }
        else
        {
            return std::make_shared<Dog>();
        }
    }
};

class FruitFactory : public Factory
{
public:
    std::shared_ptr<Fruit> CreateFruit(const std::string &name) override
    {
        if (name == "苹果")
        {
            return std::make_shared<Apple>();
        }
        else
        {
            return std::make_shared<Banana>();
        }
    }
    std::shared_ptr<Animal> CreateAnimal(const std::string &name) override
    {
        return nullptr;
    }
};

// class FruitFactory
// {
// public:
//     static std::shared_ptr<Fruit> create(const std::string &name)
//     {
//         if (name == "苹果")
//         {
//             return std::make_shared<Apple>();
//         }
//         else
//         {
//             return std::make_shared<Banana>();
//         }

//     }
// };

// class FruitFactory
// {
// public:
//     virtual std::shared_ptr<Fruit> create() = 0;
// };

// class AppleFactory : public FruitFactory
// {
//     std::shared_ptr<Fruit> create() override
//     {
//         return std::make_shared<Apple>();
//     }
// };

// class BananaFactory : public FruitFactory
// {
//     std::shared_ptr<Fruit> create() override
//     {
//         return std::make_shared<Banana>();
//     }
// };

class FactoryProducer
{
public:
    static std::shared_ptr<Factory> create(const std::string &name)
    {
        if (name == "动物工厂")
            return std::make_shared<AnimalFactory>();
        else
            return std::make_shared<FruitFactory>();
    }
};

int main()
{
    // std::shared_ptr<Fruit> fruit1 = FruitFactory::create("苹果");
    // std::shared_ptr<Fruit> fruit2 = FruitFactory::create("香蕉");
    // std::shared_ptr<FruitFactory> ff1(new AppleFactory());
    // std::shared_ptr<Fruit> fruit1 = ff1->create();
    // std::shared_ptr<FruitFactory> ff2(new BananaFactory());
    // std::shared_ptr<Fruit> fruit2 = ff2->create();
    // fruit1->name();
    // fruit2->name();

    // 创建动物工厂
    std::shared_ptr<Factory> animalFactory = FactoryProducer::create("动物工厂");
    std::shared_ptr<Animal> cat = animalFactory->CreateAnimal("小猫");
    std::shared_ptr<Animal> dog = animalFactory->CreateAnimal("小狗");

    cat->name(); // 输出“小猫”
    dog->name(); // 输出“小狗”

    // 创建水果工厂
    std::shared_ptr<Factory> fruitFactory = FactoryProducer::create("水果工厂");
    std::shared_ptr<Fruit> apple = fruitFactory->CreateFruit("苹果");
    std::shared_ptr<Fruit> banana = fruitFactory->CreateFruit("香蕉");

    apple->name();  // 输出“苹果”
    banana->name(); // 输出“香蕉”

    return 0;
}