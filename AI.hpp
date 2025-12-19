#pragma once
#include "GameObject.hpp"
#include "GameBoard.hpp"
#include <memory>
#include <vector>

// Класс AI - представляет компьютерного противника
// Наследует от GameObject и реализует ИИ-логику
class AI : public GameObject {
private:
    // Собственное игровое поле
    std::unique_ptr<GameBoard> ownBoard;

    // Указатель на поле игрока (противника)
    GameBoard* enemyBoard;

    int difficultyLevel;  // Уровень сложности (1-легкий, 2-средний, 3-сложный)

    // Память для запоминания ходов (динамический двумерный массив)
    std::vector<std::vector<bool>> shotMemory;

    // Последние успешные попадания для "умного" поиска
    std::vector<std::pair<int, int>> recentHits;

public:
    // Конструктор - создает AI с указанным именем и уровнем сложности
    AI(const std::string& aiName = "Компьютер", int difficulty = 1);

    // Деструктор
    ~AI() override {  //  Автоматически виртуальный
       // std::cout << "~AI" << std::endl;
      //  delete enemyBoard;  // Освобождаем память
    }

    // ПЕРЕОПРЕДЕЛЕНИЕ виртуальных методов GameObject:
    void display() const override;
    std::string getType() const override;
    void performAction() const override;

    // Перегруженная версия - с уровнем сложности
    void performAction(int difficultyBoost) const;
    // ИГРОВЫЕ МЕТОДЫ AI:

    // AI делает ход (сам выбирает координаты)
    bool makeMove();

    // Обработка выстрела игрока по полю AI
    bool receiveShot(int x, int y);

    // Проверка, проиграл ли AI
    bool hasLost() const;

    // Расстановка кораблей на поле AI
    bool setupShips();

    // SETTER-методы:

    // Установка поля противника (игрока)
    void setEnemyBoard(GameBoard& enemyBoardRef) {
        enemyBoard = &enemyBoardRef;
    }

    // GETTER-методы:

    // Получение собственного поля (для связи с Player)
    GameBoard& getOwnBoard() {
        return *ownBoard;
    }

    // Получение уровня сложности
    int getDifficulty() const {
        return difficultyLevel;
    }
    void update() override;
    GameObject* clone() const override;
private:
    // ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ (инкапсулированная логика):

    // Инициализация памяти для ходов
    void initializeShotMemory();

    // Поиск случайной свободной клетки
    std::pair<int, int> findRandomCell() const;

    // "Умный" поиск клетки (на основе предыдущих попаданий)
    std::pair<int, int> findSmartCell();

    // Проверка, делал ли AI уже выстрел в указанную клетку
    bool hasShotAt(int x, int y) const;
};