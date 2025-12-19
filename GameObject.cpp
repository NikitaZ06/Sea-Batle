#include "GameObject.hpp"

// Конструктор GameObject
GameObject::GameObject(const std::string& objectName)
    : name(objectName) {
    // Инициализируем имя объекта
    // std::cout << "Создан GameObject: " << name << std::endl;
}

// Геттер для получения имени
std::string GameObject::getName() const {
    return name;
}

// Базовая реализация отображения
void GameObject::display() const {
    std::cout << "Имя: " << name << std::endl;
}

// Базовая реализация получения типа
std::string GameObject::getType() const {
    return "GameObject (базовый тип)";
}

// Базовая реализация выполнения действия
void GameObject::performAction() const {
    std::cout << name << " выполняет базовое действие" << std::endl;
}

void GameObject::NoVirtualdisplay() const {
   // std::cout << "Не виртуальный метод: " << name << std::endl;
    this->display();
}