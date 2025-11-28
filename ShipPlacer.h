#pragma once
#include "GameBoard.h"
#include <vector>

class ShipPlacer {
private:
    GameBoard& board;  // —сылка на игровое поле дл€ расстановки

public:
    //  онструктор принимает ссылку на поле
    ShipPlacer(GameBoard& gameBoard);

    // ќсновной метод автоматической расстановки кораблей
    bool AutoPlaceShips();

private:
    // ¬спомогательные методы
    bool placeSingleShip(int size);
    bool tryPlaceShip(int size, int maxAttempts = 50);
};

