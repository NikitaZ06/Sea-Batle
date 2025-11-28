#include "GameObject.hpp"
// GameObject.cpp
#include <iostream>

GameObject::GameObject(const std::string& objectName)
    : name(objectName) {
    std::cout << "Создан: " << name << std::endl;
}

std::string GameObject::getName() const {
    return name;
}

void GameObject::display() const {
    std::cout << "Объект: " << name << std::endl;
}
