#pragma once
#include "Player.hpp"
#include "AI.hpp"
#include "GameBoard.hpp"
#include <vector>
#include <memory>
class GameSession {
private:
    // УМНЫЕ УКАЗАТЕЛИ 
    std::unique_ptr<GameBoard> aiBoard;           
    std::unique_ptr<Player> humanPlayer;            
    std::unique_ptr<AI> computerAI;                

    bool isPlayerTurn;            // Чей ход (true - игрок, false - AI)

    // ДИНАМИЧЕСКИЙ МАССИВ ДЛЯ ИСТОРИИ ХОДОВ
    std::unique_ptr<std::string[]> moveHistory;   // unique_ptr для массива
    std::unique_ptr<std::string[]> aiHistory;
    int moveCount;                // Текущее количество ходов
    int maxMoves;                 // Максимальное количество ходов

public:
    friend void showStats(const GameSession& session);//функция для статискики 
    //Константы
    static const int SIZE_BOARD = 10;
    static const int KOLVO_CELLS = 100;
    // Конструктор принимает имя игрока
    GameSession(const std::string& playerName);

    // ДЕСТРУКТОР для очистки всей динамической памяти
  //  ~GameSession();

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