
#pragma once
#include <string>
#include <iostream>

// Базовый класс для всех игровых объектов
// Реализует общие свойства и поведение
class GameObject {
protected:
    std::string name;  // Имя объекта (доступно в наследниках)

public:
    // Конструктор - инициализирует имя объекта
    GameObject(const std::string& objectName);

    // Виртуальный деструктор -  для корректного удаления наследников
    virtual ~GameObject() {  }

    // Геттер имени - общий для всех наследников
    std::string getName() const;
    void NoVirtualdisplay() const;
    virtual void update() = 0;
    // ВИРТУАЛЬНЫЕ МЕТОДЫ для переопределения в наследниках:

    // Отображение информации об объекте
    virtual void display() const;

    // Получение типа объекта (для идентификации)
    virtual std::string getType() const;

    // Выполнение действия, характерного для объекта
    virtual void performAction() const;

    virtual GameObject* clone() const = 0;
    //  ВИРТУАЛЬНЫЙ оператор присваивания
    virtual GameObject& operator=(const GameObject& other) {
        if (this != &other) {
            name = other.name;
        }
        return *this;
    }
};


