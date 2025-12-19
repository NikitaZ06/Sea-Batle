#pragma once
#include "GameObject.hpp"
#include "GameBoard.hpp"
#include <memory>

// Класс Player - представляет игрока-человека
// Наследует от GameObject и добавляет игровую логику
class Player : public GameObject {
private:
    // Уникальный указатель на собственное игровое поле
    // Player ВЛАДЕЕТ своим полем (управляет его временем жизни)
    std::unique_ptr<GameBoard> ownBoard;

    // Указатель на поле противника (AI)
    // Player НЕ владеет полем противника, только ссылается на него
    GameBoard* enemyBoard;

    int score;          // Счет игрока
    static int playerCount;  // Статическое поле - счетчик всех игроков

public:
    // Конструктор 1: создает игрока без указания противника
    // Противник будет установлен позже через setEnemyBoard()
    Player(const std::string& playerName);

    // Конструктор 2: создает игрока с сразу указанным противником
    Player(const std::string& playerName, GameBoard& enemyBoardRef);

    // Деструктор
    ~Player() override {  //  Автоматически виртуальный
        //std::cout << "~Player" << std::endl;
        
    }
    //  ПЕРЕГРУЗКА оператора присваивания для объектов базового класса
    Player& operator=(const GameObject& other) override;

    //  Обычный оператор присваивания для объектов того же класса
    Player& operator=(const Player& other);

    // ПЕРЕОПРЕДЕЛЕНИЕ виртуальных методов GameObject:
    void display() const override;
    std::string getType() const override;
    void performAction() const override;
    // Перегруженная версия 1 - с параметром
    void performAction(const std::string& target) const;
    // ИГРОВЫЕ МЕТОДЫ (специфичные для Player):

    // Игрок делает ход по указанным координатам
    bool makeMove(int x, int y);

    // Обработка выстрела противника по полю игрока
    bool receiveShot(int x, int y);

    // Проверка, проиграл ли игрок (все корабли потоплены)
    bool hasLost() const;

    // Отображение обоих игровых полей (своего и противника)
    void displayBoards() const;

    // SETTER-методы для установки связей:

    // Установка поля противника
    void setEnemyBoard(GameBoard& enemyBoardRef) {
        enemyBoard = &enemyBoardRef;
    }

    // GETTER-методы:

    // Получение собственного поля (для связи с AI)
    GameBoard& getOwnBoard() {
        return *ownBoard;
    }

    // Получение указателя на поле противника
    GameBoard* getEnemyBoard() {
        return enemyBoard;
    }

    // Статический метод - возвращает количество созданных игроков
    // Не требует создания объекта для вызова
    static int getPlayerCount();

    void update() override;
    GameObject* clone() const override;
};