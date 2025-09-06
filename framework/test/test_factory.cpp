#include <iostream>

#include "factory.hpp"

using namespace ilrd;

class IShape
{
public:
    virtual void Draw() = 0;
    virtual ~IShape() = default;
};

class Circle : public IShape
{
public:
    void Draw() override
    {
        std::cout<<"Drawing a Circle"<< std::endl;
    }
};

class Square : public IShape
{
public:
    void Draw() override
    {
        std::cout<<"Drawing a Square"<<std::endl;
    }
};

class Rectangle : public IShape
{
public:
    void Draw() override
    {
        std::cout<<"Drawing a Rectangle"<<std::endl;
    }
};

std::unique_ptr<IShape> CreateCircle()
{
    return std::make_unique<Circle>();
}

std::unique_ptr<IShape> CreateSquare()
{
    return std::make_unique<Square>();
}

enum ShapesEnum
{
    CIRCLE,
    SQUARE,
    RECTANGLE
};

int main()
{
    Factory<ShapesEnum, IShape> factory;
    factory.Register(CIRCLE, CreateCircle);
    factory.Register(SQUARE, CreateSquare);

    std::unique_ptr<IShape> circle = factory.Create(CIRCLE);
    std::unique_ptr<IShape> square = factory.Create(SQUARE);

    try
    {
        std::unique_ptr<IShape> rectangle = factory.Create(RECTANGLE);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    circle->Draw();
    square->Draw();

    return 0;
}