#ifndef GAMESESSION_H
#define GAMESESSION_H

#include "Player.h"
#include "AI.h"
#include "GameBoard.h"
#include <vector>

class GameSession {
private:
    // ДИНАМИЧЕСКИЕ ОБЪЕКТЫ - используем указатели
    GameBoard* aiBoard;           // УКАЗАТЕЛЬ на поле AI
    Player* humanPlayer;          // УКАЗАТЕЛЬ на игрока  
    AI* computerAI;               // УКАЗАТЕЛЬ на AI

    bool isPlayerTurn;            // Чей ход (true - игрок, false - AI)

    // ДИНАМИЧЕСКИЙ МАССИВ ДЛЯ ИСТОРИИ ХОДОВ
    std::string* moveHistory;     // Указатель на массив строк
    int moveCount;                // Текущее количество ходов
    int maxMoves;                 // Максимальное количество ходов

public:
    // Конструктор принимает имя игрока
    GameSession(const std::string& playerName);

    // ДЕСТРУКТОР для очистки всей динамической памяти
    ~GameSession();

    // Основные методы игровой сессии
    void startGame();             // Запуск игры
    bool isGameOver();            // Проверка окончания игры
    void syncPlayerEnemyBoard();  // Синхронизация полей
    void displayGameState();      // Отображение состояния игры

    // Новые методы для демонстрации работы с динамической памятью
    void DynamicObjects();        // Демонстрация динамических объектов
    void YkazObject(); // Демонстрация указателей и ссылок

private:
    // Вспомогательные методы
    void setupGame();             // Настройка игры
    void playerTurn();            // Ход игрока
    void aiTurn();                // Ход компьютера
    void addMoveToHistory(const std::string& move);  // Добавление хода в историю
    void displayMoveHistory() const;                 // Отображение истории ходов
};

#endif
