#ifndef AI_H
#define AI_H

#include "GameBoard.h"
#include <vector>

class AI {
private:
    GameBoard& ownBoard;      // ссылка на собственное поле AI
    GameBoard& enemyBoard;    // ссылка на поле игрока (куда AI стреляет)

    // Динамический двумерный массив для запоминания ходов
    bool** shotMemory;        // Указатель на массив указателей
    int boardSize;            // Размер поля

public:
    // Конструктор принимает две ссылки на поля
    AI(GameBoard& aiBoard, GameBoard& playerBoard);

    // ДЕСТРУКТОР для очистки динамической памяти
    ~AI();

    // Основные методы 
    void makeMove();           // Сделать ход
    bool setupShips();         // Расставить корабли

    // Проверка проигрыша AI
    bool hasLost() const {
        return ownBoard.isGameOver();  // Проверяем собственное поле
    }

private:
    // Вспомогательные методы для работы с динамической памятью
    void initializeShotMemory();  // Инициализация памяти для запоминания ходов
    void cleanupShotMemory();     // Очистка памяти
};

#endif