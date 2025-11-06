#pragma once
#include "GameBoard.hpp"
#include <string>

class Player {
private:
    std::string name;
    GameBoard* ownBoard;      // УКАЗАТЕЛЬ на собственное поле (ДИНАМИЧЕСКИЙ ОБЪЕКТ)
    GameBoard& enemyBoard;    // ССЫЛКА на поле противника
    int score;

    // ДИНАМИЧЕСКИЙ МАССИВ ДЛЯ СТАТИСТИКИ ПОПАДАНИЙ ПО СТОЛБЦАМ
    int* hitStatistics;

public:
    // Конструктор принимает ссылку на поле противника
    Player(const std::string& playerName, GameBoard& enemyBoardRef);

    // ДЕСТРУКТОР для очистки динамической памяти
    ~Player();

    // Основные методы игрока
    bool makeMove(int x, int y);
    bool receiveShot(int x, int y);
    bool hasLost() const;
    std::string getName() const;
    void displayBoards() const;

    // Геттеры для доступа к полям
    GameBoard& getOwnBoard() { return *ownBoard; }      // Разыменование указателя
    GameBoard& getEnemyBoard() { return enemyBoard; }   // Возврат ссылки


};