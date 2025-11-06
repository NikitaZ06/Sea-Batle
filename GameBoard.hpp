#pragma once
#include "Cell.hpp"
#include <vector>

class GameBoard
{
private:
    Cell** grid;                 // ДИНАМИЧЕСКИЙ ДВУМЕРНЫЙ МАССИВ - указатель на указатели

public:
    // Конструктор и деструктор
    GameBoard();
    ~GameBoard();

    // Основные методы
    void Display(bool showShips = false) const; // Вывод поля
    bool placeShip(int x, int y, int size, bool horizontal); // Размещение корабля
    bool receiveShot(int x, int y);             // Обработка выстрела
    bool isGameOver() const;                    // Проверка конца игры

    // Геттеры
    int getSize() const;
    Cell getCell(int x, int y) const;
    bool isValidPosition(int x, int y) const;

    // методы для размещения кораблей
    bool canPlaceShip(int x, int y, int size, bool horizontal) const;
    bool isAreaClear(int x, int y, int size, bool horizontal) const;
};