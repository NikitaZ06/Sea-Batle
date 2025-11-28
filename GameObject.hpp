
#pragma once
#include <string>

class GameObject {
protected:
    std::string name;

public:
    // Базовый конструктор
    GameObject(const std::string& objectName);

    // Простые методы
    std::string getName() const;
    virtual void display() const;  // Виртуальный для переопределения
};


