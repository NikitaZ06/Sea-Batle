#pragma once
#include "Player.hpp"
#include "AI.hpp"
#include <vector>
#include <memory>
#include <string>
#include <type_traits>
#include <iostream>
#include <algorithm>
#include "Statistics.hpp"

class GameSession {
private:
    // УМНЫЕ УКАЗАТЕЛИ на игроков
    std::unique_ptr<Player> player;  // Игрок-человек
    std::unique_ptr<AI> ai;          // Компьютерный противник

    bool isPlayerTurn;               // Чей ход (true - игрок, false - AI)
    bool gameActive;                 // Флаг активности игры

    // ИСТОРИЯ ХОДОВ (вектор вместо динамического массива - проще и безопаснее)
    std::vector<std::string> moveHistory;  // История ходов игрока
    std::vector<std::string> aiHistory;    // История ходов AI

 
    // Контейнер для хранения указателей на GameObject и его наследников
    std::vector<std::shared_ptr<GameObject>> gameObjects;

    // Статистика для игроков
    PlayerStatistics<int> playerStats;
public:
    static const int BOARD_SIZE = 10;
    static const int MAX_CELLS = 100;
    // Дружественная функция для статистики
    friend void showStats(const GameSession& session);

    // Конструктор
    GameSession(const std::string& playerName = "Игрок",
        const std::string& aiName = "Компьютер",
        int aiDifficulty = 1);
    GameSession(const GameSession&) = delete;
    GameSession& operator=(const GameSession&) = delete;

    // Основные методы игровой сессии
    void startGame();                    // Запуск игры
    bool isGameOver() const;             // Проверка окончания игры
    void displayGameState() const;       // Отображение состояния игры

    // Методы для управления ходами (для использования из main)
    bool makePlayerMove(int x, int y);   // Игрок делает ход по координатам
    bool makeAIMove();                   // AI делает ход

    // Геттеры
    bool getIsPlayerTurn() const { return isPlayerTurn; }

    bool isGameActive() const { return gameActive; }
    Player* getPlayer() const { return player.get(); }
    AI* getAI() const { return ai.get(); }


    static int getBoardSize() { return BOARD_SIZE; }
    static int getMaxCells() { return MAX_CELLS; }
    const std::vector<std::string>& getMoveHistory() const { return moveHistory; }
    const std::vector<std::string>& getAiHistory() const { return aiHistory; }



    // Добавить объект в контейнер
    void addGameObject(std::shared_ptr<GameObject> obj) {
        gameObjects.push_back(obj);
    }
    // Показать все объекты в контейнере
    void displayAllGameObjects() const {
        std::cout << "\n=== ВСЕ ИГРОВЫЕ ОБЪЕКТЫ ===" << std::endl;
        for (const auto& obj : gameObjects) {
            obj->display();
            std::cout << "Тип: " << obj->getType() << std::endl;
            std::cout << "------------------------" << std::endl;
        }
    }

    // Найти объект по имени (алгоритм поиска)
    std::shared_ptr<GameObject> findGameObjectByName(const std::string& name) {
        auto it = std::find_if(gameObjects.begin(), gameObjects.end(),
            [&name](const std::shared_ptr<GameObject>& obj) {
                return obj->getName() == name;
            });

        if (it != gameObjects.end()) {
            return *it;
        }
        return nullptr;
    }

    // Сортировать объекты по имени (алгоритм сортировки)
    void sortGameObjectsByName() {
        std::sort(gameObjects.begin(), gameObjects.end(),
            [](const std::shared_ptr<GameObject>& a, const std::shared_ptr<GameObject>& b) {
                return a->getName() < b->getName();
            });
    }

    // Получить объекты определенного типа
    template<typename T>
    std::vector<std::shared_ptr<T>> getObjectsOfType() {
        std::vector<std::shared_ptr<T>> result;
        for (const auto& obj : gameObjects) {
            if (auto derived = std::dynamic_pointer_cast<T>(obj)) {
                result.push_back(derived);
            }
        }
        return result;
    }

    // Обновить статистику
    void updatePlayerStats(const std::string& playerName, int score) {
        playerStats.addScore(playerName, score);
    }

    // Показать отсортированную статистику
    void displaySortedStats() {
        playerStats.sortByScoreDescending();
        playerStats.displayStats();

        // Поиск лучшего игрока
        int bestScore = playerStats.getHighestScore();
        std::cout << "\nЛучший счет: " << bestScore << " очков" << std::endl;
    }

    // Получить статистику для игрока
    int getPlayerStats(const std::string& name) {
        return playerStats.findScoreByName(name);
    }


private:
    // Вспомогательные методы
    void setupGame();                    // Настройка игры (расстановка кораблей)
    void switchTurn();                   // Переключение очереди хода
    void addMoveToHistory(const std::string& move, bool isPlayerMove = true);
    void displayMoveHistory() const;     // Отображение истории ходов
};
// Шаблонная функция для вычисления средней точности

