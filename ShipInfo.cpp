#include "ShipInfo.hpp"
#include <iostream>

ShipInfo::ShipInfo(const std::string& name, const std::string& type, int size)
    : GameObject(name), size(size), shipType(type) {
    positions = new std::vector<int>;
    std::cout << "Создан ShipInfo: " << name << std::endl;
}

// Конструктор копирования для глубокого клонирования
ShipInfo::ShipInfo(const ShipInfo& other)
    : GameObject(other.name), size(other.size), shipType(other.shipType) {
    // Глубокое копирование вектора
    positions = new std::vector<int>(*other.positions);
    std::cout << "Скопирован ShipInfo: " << name << " (глубокое копирование)" << std::endl;
}

ShipInfo::~ShipInfo() {
    delete positions;
    std::cout << "Уничтожен ShipInfo: " << name << std::endl;
}

void ShipInfo::update() {
    std::cout << "Корабль " << name << " обновляет данные" << std::endl;
}

void ShipInfo::display() const {
    GameObject::display();
    std::cout << "  Тип корабля: " << shipType << std::endl;
    std::cout << "  Размер: " << size << std::endl;
    std::cout << "  Позиций: " << positions->size() << std::endl;
}

std::string ShipInfo::getType() const {
    return "ShipInfo";
}

void ShipInfo::performAction() const {
    std::cout << name << " (корабль) выполняет морские операции" << std::endl;
}

GameObject* ShipInfo::clone() const {
    // Используем конструктор копирования для глубокого клонирования
    return new ShipInfo(*this);
}

void ShipInfo::addPosition(int pos) {
    positions->push_back(pos);
}

void ShipInfo::showPositions() const {
    std::cout << "Позиции корабля " << name << ": ";
    for (int pos : *positions) {
        std::cout << pos << " ";
    }
    std::cout << std::endl;
}
