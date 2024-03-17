#include <cstdlib>
#include <iostream>
#include <memory>
#include <string_view>
#include <vector>

class Animal {
protected:
    std::string name;
    int legs;
public:
    Animal(std::string_view name_, int legs_)
      : name(name_), legs(legs_)
    {}
    virtual std::string speak() const = 0;
    virtual ~Animal() = default;
};

class Cat : public Animal {
public:
    Cat(std::string_view name_, int legs_)
        : Animal(name_, legs_) {}

    std::string speak() const override {
        return name + " says: Meow";
    }
};

class Dog : public Animal {
public:
    Dog(std::string_view name_, int legs_)
        : Animal(name_, legs_) {}

    std::string speak() const override {
        return name + " says: Bark";
    }
};

constexpr int kCount = 12;

auto main() -> int {
    std::vector<std::unique_ptr<Animal>> v(kCount);
    for (auto& p : v)
        if (rand() % 2)
            p = std::make_unique<Cat>("Garfield", 4);
        else
            p = std::make_unique<Dog>("Beethowen", 4);

    for (auto&& p : v)
        std::cout << p->speak() << std::endl;
    return EXIT_SUCCESS;
}