#pragma once
#include "GameBoard.hpp"
#include <string>
#include <memory>
#include "GameObject.hpp"
class Player : public GameObject {
private:
    std::string name;
    std::unique_ptr<GameBoard> ownBoard;
    GameBoard& enemyBoard;    // ССЫЛКА на поле противника
    int score;

    // ДИНАМИЧЕСКИЙ МАССИВ ДЛЯ СТАТИСТИКИ ПОПАДАНИЙ ПО СТОЛБЦАМ
    int* hitStatistics;
    // СТАТИЧЕСКОЕ ПОЛЕ - общее для всех игроков
    static int KolvoPlayers;

public:
    // Конструктор принимает ссылку на поле противника
    // Конструктор вызывает конструктор базового класса
    Player(const std::string& playerName, GameBoard& enemyBoardRef);

    // Переопределяем метод display
    void display() const ;


    // Основные методы игрока
    bool makeMove(int x, int y);
    bool receiveShot(int x, int y);
    bool hasLost() const;
    std::string getName() const;
    void displayBoards() const;

    // Геттеры для доступа к полям
    GameBoard& getOwnBoard() { return *ownBoard; }      // Разыменование указателя
    GameBoard& getEnemyBoard() { return enemyBoard; }   // Возврат ссылки

    //СТАТИЧЕСКИЙ МЕТОД - работает без создания объекта
    static int getPlayers();
};