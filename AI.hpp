#pragma once
#include "GameBoard.hpp"
#include <vector>
#include <memory>

class AI {
private:
    GameBoard& ownBoard;      // ссылка на собственное поле AI
    GameBoard& enemyBoard;    // ссылка на поле игрока (куда AI стреляет)

    // Динамический двумерный массив для запоминания ходов
   // bool** shotMemory;        // Указатель на массив указателей
    std::unique_ptr<std::unique_ptr<bool[]>[]> shotMemory;
    
public:
    // Конструктор принимает две ссылки на поля
    AI(GameBoard& aiBoard, GameBoard& playerBoard);

    // ДЕСТРУКТОР для очистки динамической памяти
   // ~AI();

    // Основные методы 
    bool makeMove();           // Сделать ход
    bool setupShips();         // Расставить корабли

    // Проверка проигрыша AI
    bool hasLost() const {
        return ownBoard.isGameOver();  // Проверяем собственное поле
    }

private:
    // Вспомогательные методы для работы с динамической памятью
    void initializeShotMemory();  // Инициализация памяти для запоминания ходов
   
};
