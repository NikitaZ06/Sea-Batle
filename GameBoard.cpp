#include "GameBoard.h"
#include <iostream>
#include <iomanip>
#include "Player.h"
using namespace std;

// конструктор - создает динамический двумерный массив
GameBoard::GameBoard() : grid(nullptr) {
    //выделение памяти для двумерного массива

    // Создаем массив указателей на строки
    grid = new Cell * [SIZE];

    //  Для каждой строки создаем массив ячеек
    for (int i = 0; i < SIZE; i++) {
        grid[i] = new Cell[SIZE];
    }

   // cout << "Конструктор: выделено памяти для " << SIZE << "x" << SIZE << " ячеек" << endl;
}

// освобождаем динамическую память
GameBoard::~GameBoard() {
    if (grid != nullptr) {
        // Удаляем каждую строку (массивы ячеек)
        for (int i = 0; i < SIZE; i++) {
            delete[] grid[i];  // Освобождаем память строки
            grid[i] = nullptr; // Обнуляем указатель
        }
        // Удаляем массив указателей
        delete[] grid;
        grid = nullptr;
    }
    cout << "Деструктор: память освобождена" << endl;
}

// Размещение корабля на поле
bool GameBoard::placeShip(int x, int y, int size, bool horizontal) {
    // Проверяем можно ли разместить корабль
    if (!canPlaceShip(x, y, size, horizontal)) {
        return false; // Размещение невозможно
    }

    // Размещаем корабль - устанавливаем флаги hasShip в true
    if (horizontal) {
        // Горизонтальное размещение - по оси Y
        for (int i = 0; i < size; i++) {
            grid[x][y + i].setHasShip(true);
        }
    }
    else {
        // Вертикальное размещение - по оси X
        for (int i = 0; i < size; i++) {
            grid[x + i][y].setHasShip(true);
        }
    }
    return true; // Размещение успешно
}

// Обработка выстрела по полю
bool GameBoard::receiveShot(int x, int y)
{
    // Проверка валидности координат
    if (!isValidPosition(x, y)) return false;

    // Проверяем, есть ли корабль в ячейке
    if (grid[x][y].getHasShip()) {
        grid[x][y].setState(CellState::HIT); // Попадание
        return true;
    }
    else {
        grid[x][y].setState(CellState::MISS); // Промах
        return false;
    }
}

// Проверка окончания игры (все корабли потоплены)
bool GameBoard::isGameOver() const
{
    // Проходим по всем ячейкам поля
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            // Если нашли корабль, который не подбит - игра не окончена
            if (grid[i][j].getHasShip() &&
                grid[i][j].getState() != CellState::HIT) {
                return false;
            }
        }
    }
    return true; // Все корабли потоплены
}

// Возвращает размер поля
int GameBoard::getSize() const
{
    return SIZE;
}

// Возвращает ячейку по координатам
Cell GameBoard::getCell(int x, int y) const
{
    return grid[x][y];
}

// Проверка валидности координат
bool GameBoard::isValidPosition(int x, int y) const
{
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}

// Проверка возможности размещения корабля
bool GameBoard::canPlaceShip(int x, int y, int size, bool horizontal) const
{
    // Проверка выхода за границы поля
    if (horizontal) {
        if (y + size > SIZE) {
            return false; // Корабль выходит за правую границу
        }
    }
    else {
        if (x + size > SIZE) {
            return false; // Корабль выходит за нижнюю границу
        }
    }

    // Проверка что все целевые клетки свободны
    for (int i = 0; i < size; i++) {
        int checkX = horizontal ? x : x + i;
        int checkY = horizontal ? y + i : y;

        // Если клетка невалидна или уже занята кораблем
        if (!isValidPosition(checkX, checkY) ||
            grid[checkX][checkY].getHasShip()) {
            return false;
        }
    }

    // Проверка зоны вокруг корабля (должна быть пустой)
    if (!isAreaClear(x, y, size, horizontal)) {
        return false;
    }

    return true; // Все проверки пройдены
}

// Проверка зоны вокруг корабля (корабль + 1 клетка вокруг)
bool GameBoard::isAreaClear(int x, int y, int size, bool horizontal) const
{
    // Определяем зону проверки (корабль + 1 клетка вокруг)
    int startX = max(0, x - 1);
    int endX = min(SIZE - 1, horizontal ? x + 1 : x + size);
    int startY = max(0, y - 1);
    int endY = min(SIZE - 1, horizontal ? y + size : y + 1);

    // Проверяем всю зону
    for (int i = startX; i <= endX; i++) {
        for (int j = startY; j <= endY; j++) {
            // Если в этой клетке есть корабль - зона не чистая
            if (grid[i][j].getHasShip()) {
                return false;
            }
        }
    }

    return true; // Вся зона чистая
}

// Отображение игрового поля
void GameBoard::Display(bool showShips) const {
    // Вывод буквенных координат (A-J)
    cout << "  ";
    for (int i = 0; i < SIZE; i++) {
        cout << " " << static_cast<char>('A' + i);
    }
    cout << endl;

    // Вывод поля с числовыми координатами
    for (int i = 0; i < SIZE; i++) {
        cout << setw(2) << (i + 1); // Номер строки
        for (int j = 0; j < SIZE; j++) {
            CellState state = grid[i][j].getState();
            char symbol = '~';

            // Выбор символа в зависимости от состояния ячейки
            switch (state) {
            case CellState::EMPTY: symbol = '~'; break;
            case CellState::SHIP: symbol = showShips ? 'S' : '~'; break; // Корабли показываем только если showShips=true
            case CellState::HIT: symbol = 'X'; break;
            case CellState::MISS: symbol = 'O'; break;
            }
            cout << " " << symbol;
        }
        cout << endl;
    }
}





