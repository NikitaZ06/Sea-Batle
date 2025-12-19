#pragma once
enum class CellState {
    EMPTY,      // Пустая
    SHIP,       // Корабль
    HIT,        // Попадание
    MISS        // Промах
};

class Cell
{
private:
    CellState state;    // Текущее состояние ячейки
    bool hasShip;       // Флаг - есть ли в ячейке корабль

public:
    // Конструктор по умолчанию
    Cell();
    //Перегрузка операторов
    bool operator!() const {
        return getHasShip() && getState() != CellState::HIT;
    }
    explicit operator bool() const {
        return state != CellState::HIT && state != CellState::MISS;
    }
    // Проверка, стреляли ли уже в эту клетку
    bool wasShotBefore() const {
        return state == CellState::HIT || state == CellState::MISS;
    }
    // Геттеры 
    CellState getState() const;
    bool getHasShip() const;

    // Сеттеры (позволяют изменять приватные поля)
    void setState(CellState newState);
    void setHasShip(bool hasShip);
};