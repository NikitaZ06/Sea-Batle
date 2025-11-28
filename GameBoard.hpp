#pragma once
#include "Cell.hpp"
#include <vector>
#include <memory>
//#include "GameSession.hpp"
class GameBoard
{
private:
   // Cell** grid;                 // ДИНАМИЧЕСКИЙ ДВУМЕРНЫЙ МАССИВ - указатель на указатели
    std::unique_ptr<std::unique_ptr<Cell[]>[]> grid;
public:
    const int SIZE_BOARD = 10;
    // Конструктор и деструктор
    GameBoard();
    // Конструктор копирования
    GameBoard(const GameBoard& other) {
        // Создаем новую сетку такого же размера
        grid = std::make_unique<std::unique_ptr<Cell[]>[]>(SIZE_BOARD);

        for (int i = 0; i < SIZE_BOARD; i++) {
            grid[i] = std::make_unique<Cell[]>(SIZE_BOARD);
            for (int j = 0; j < SIZE_BOARD; j++) {
                grid[i][j] = other.grid[i][j];  // Копируем каждую ячейку
            }
        }
    }   
    //~GameBoard(); 

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