#include "ShipPlacer.hpp"
#include <algorithm>
#include "GameSession.hpp"
#include <cstdlib>
#include <ctime>

// Конструктор - инициализирует ссылку на поле и генератор случайных чисел
ShipPlacer::ShipPlacer(GameBoard& gameBoard)
    : board(gameBoard) {  // Инициализация ссылки в списке инициализации
    srand(time(0));  // Инициализация генератора случайных чисел
}

// Основной метод автоматической расстановки всех кораблей
bool ShipPlacer::AutoPlaceShips() {
    // СОЗДАНИЕ ДИНАМИЧЕСКОГО МАССИВА ДЛЯ ХРАНЕНИЯ РАЗМЕРОВ КОРАБЛЕЙ
    // Классические правила: 1x4, 2x3, 3x2, 4x1
    std::vector<int> shipSizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
    bool success = true;  // Флаг успешной расстановки

    // Размещаем каждый корабль из динамического массива
    for (int i = 0; i < 10; i++) {
        if (!placeSingleShip(shipSizes[i])) {
            success = false;  // Если не удалось разместить корабль
            break;
        }
    }

    return success;
}

// Размещение одного корабля заданного размера
bool ShipPlacer::placeSingleShip(int size) {
    return tryPlaceShip(size, GameSession::KOLVO_CELLS);  // 100 попыток на размещение
}

// Попытка размещения корабля с случайными координатами
bool ShipPlacer::tryPlaceShip(int size, int maxAttempts) {
    // Пытаемся разместить корабль в случайной позиции
    for (int attempt = 0; attempt < maxAttempts; attempt++) {
        // Генерируем случайные координаты и направление
        int x = rand() % GameSession::SIZE_BOARD;
        int y = rand() % GameSession::SIZE_BOARD;
        bool horizontal = rand() % 2 == 0;  // Случайное направление

        // Пытаемся разместить корабль
        if (board.placeShip(x, y, size, horizontal)) {
            return true;  // Успешно разместили
        }
    }
    return false;  // Не удалось разместить после всех попыток
}