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

    // Геттеры 
    CellState getState() const;
    bool getHasShip() const;

    // Сеттеры (позволяют изменять приватные поля)
    void setState(CellState newState);
    void setHasShip(bool hasShip);
};