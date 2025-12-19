#include "AI.hpp"
#include "Player.hpp"
#include "GameSession.hpp"
#include "ShipPlacer.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Конструктор AI
AI::AI(const std::string& aiName, int difficulty)
    : GameObject(aiName),          // Вызов конструктора базового класса
    enemyBoard(nullptr),         // Пока нет противника
    difficultyLevel(difficulty) {

    // Инициализация генератора случайных чисел
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Создаем собственное игровое поле
    ownBoard = std::make_unique<GameBoard>();

    // Инициализируем память для ходов
    initializeShotMemory();

}



// ПЕРЕОПРЕДЕЛЕНИЕ 1: Отображение информации об AI
void AI::display() const {
    // Вызываем базовую реализацию
    GameObject::display();

    // Добавляем специфичную для AI информацию
    std::cout << "  Тип: Искусственный интеллект" << std::endl;
    std::cout << "  Уровень сложности: " << difficultyLevel;

    switch (difficultyLevel) {
    case 1: std::cout << " (новичок)"; break;
    case 2: std::cout << " (средний)"; break;
    case 3: std::cout << " (эксперт)"; break;
    }
    std::cout << std::endl;

    // Отображение статуса
    if (hasLost()) {
        std::cout << "  Статус: ПРОИГРАЛ" << std::endl;
    }
    else if (enemyBoard) {
        std::cout << "  Статус: В бою" << std::endl;
    }
    else {
        std::cout << "  Статус: Ожидает игрока" << std::endl;
    }
}

// ПЕРЕОПРЕДЕЛЕНИЕ 2: Получение типа объекта
std::string AI::getType() const {
    return "Искусственный интеллект (AI)";
}

// ПЕРЕОПРЕДЕЛЕНИЕ 3: Выполнение действия
void AI::performAction() const {
    std::cout << name << " вычисляет оптимальную стратегию..." << std::endl;

    // Разное поведение в зависимости от уровня сложности
    switch (difficultyLevel) {
    case 1:
        std::cout << "  'Случайная атака!'" << std::endl;
        break;
    case 2:
        std::cout << "  'Анализирую поле противника...'" << std::endl;
        break;
    case 3:
        std::cout << "  'Вычисляю вероятность расположения кораблей...'" << std::endl;
        break;
    }
}
void AI::performAction(int difficultyBoost) const {
    int boostedLevel = difficultyLevel + difficultyBoost;
    std::cout << name << " повышает уровень сложности на " << difficultyBoost << std::endl;
    std::cout << "  'Новый уровень: " << boostedLevel << "! Алгоритмы улучшены!'" << std::endl;
}

// Инициализация памяти для запоминания ходов
void AI::initializeShotMemory() {
    // Изменяем размер вектора под размер игрового поля
    shotMemory.resize(GameSession::BOARD_SIZE);

    // Инициализируем каждую строку
    for (int i = 0; i < GameSession::BOARD_SIZE; i++) {
        shotMemory[i].resize(GameSession::BOARD_SIZE, false);
    }

}

// Основной метод: AI делает ход
bool AI::makeMove() {
    // Проверяем, установлен ли противник
    if (!enemyBoard) {
        std::cout << "Ошибка: противник не установлен!" << std::endl;
        return false;
    }

    // Выбор стратегии в зависимости от уровня сложности
    std::pair<int, int> target;

    if (difficultyLevel == 1 || recentHits.empty()) {
        // Легкий уровень или нет информации для умного поиска
        target = findRandomCell();
    }
    else {
        // Средний или сложный уровень - используем "умный" поиск
        target = findSmartCell();
    }

    int x = target.first;
    int y = target.second;

    // Проверяем, что в эту клетку еще не стреляли
    if (hasShotAt(x, y)) {
        // Если уже стреляли, ищем другую клетку
        target = findRandomCell();
        x = target.first;
        y = target.second;
    }

    // Помечаем клетку как использованную
    shotMemory[x][y] = true;

    std::cout << name << " стреляет в "
        << static_cast<char>('A' + y) << (x + 1) << "... ";

    // Выполняем выстрел
    bool isHit = enemyBoard->receiveShot(x, y);

    // Запоминаем успешные попадания для "умного" поиска
    if (isHit && difficultyLevel > 1) {
        recentHits.push_back({ x, y });
        std::cout << "ПОПАДАНИЕ! " << std::endl;
    }
    else if (isHit) {
        std::cout << "ПОПАДАНИЕ!" << std::endl;
    }
    else {
        std::cout << "промах" << std::endl;
    }
    
    return isHit;
}

// Поиск случайной свободной клетки
std::pair<int, int> AI::findRandomCell() const {
    int x, y;
    int attempts = 0;
    const int maxAttempts = GameSession::BOARD_SIZE * GameSession::BOARD_SIZE;

    // Пытаемся найти случайную свободную клетку
    do {
        x = std::rand() % GameSession::BOARD_SIZE;
        y = std::rand() % GameSession::BOARD_SIZE;
        attempts++;

        // Если исчерпали попытки, ищем первую свободную
        if (attempts >= maxAttempts) {
            for (int i = 0; i < GameSession::BOARD_SIZE; i++) {
                for (int j = 0; j < GameSession::BOARD_SIZE; j++) {
                    if (!shotMemory[i][j]) {
                        return { i, j };
                    }
                }
            }
        }
    } while (shotMemory[x][y]);  // Пока не найдем свободную

    return { x, y };
}

// "Умный" поиск клетки (на основе предыдущих попаданий)
std::pair<int, int> AI::findSmartCell() {
    // Если есть недавние попадания, ищем рядом с ними
    if (!recentHits.empty()) {
        auto lastHit = recentHits.back();
        int x = lastHit.first;
        int y = lastHit.second;

        // Проверяем соседние клетки (вверх, вниз, влево, вправо)
        std::vector<std::pair<int, int>> directions = {
            {x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}
        };

        for (const auto& dir : directions) {
            int nx = dir.first;
            int ny = dir.second;

            // Проверяем границы и то, что еще не стреляли
            if (nx >= 0 && nx < GameSession::BOARD_SIZE &&
                ny >= 0 && ny < GameSession::BOARD_SIZE &&
                !hasShotAt(nx, ny)) {
                return { nx, ny };
            }
        }

        // Если все соседние клетки проверены, удаляем это попадание
        recentHits.pop_back();
    }

    // Если не нашли рядом, ищем случайную
    return findRandomCell();
}

// Проверка, делал ли AI уже выстрел в клетку
bool AI::hasShotAt(int x, int y) const {
    if (x < 0 || x >= GameSession::BOARD_SIZE ||
        y < 0 || y >= GameSession::BOARD_SIZE) {
        return true;  // Выход за границы считаем как "уже стреляли"
    }
    return shotMemory[x][y];
}

// Обработка выстрела игрока
bool AI::receiveShot(int x, int y) {
    if (!ownBoard) {
        std::cout << "Ошибка: игровое поле AI не инициализировано!" << std::endl;
        return false;
    }

    return ownBoard->receiveShot(x, y);
}

// Проверка поражения AI
bool AI::hasLost() const {
    if (!ownBoard) return true;
    return ownBoard->isGameOver();
}

// Расстановка кораблей AI
bool AI::setupShips() {
    if (!ownBoard) {
        std::cout << "Ошибка: нет игрового поля для расстановки кораблей!" << std::endl;
        return false;
    }

    // Создаем ShipPlacer для автоматической расстановки
    auto shipPlacer = std::make_unique<ShipPlacer>(*ownBoard);

    std::cout << name << " расставляет корабли... ";

    bool success = shipPlacer->AutoPlaceShips();

    if (success) {
        std::cout << "корабли расставлены!" << std::endl;
    }
    else {
        std::cout << "ошибка при расстановке кораблей!" << std::endl;
    }

    return success;
}
void AI::update() {
    std::cout << "AI " << name << " обновляет алгоритмы" << std::endl;
}

GameObject* AI::clone() const {
    // Глубокое клонирование
    AI* newAI = new AI(name, difficultyLevel);

    // Копируем игровое поле
    if (ownBoard) {
        newAI->ownBoard = std::make_unique<GameBoard>(*ownBoard);
    }

    // Копируем память о выстрелах (глубокое копирование векторов)
    newAI->shotMemory = this->shotMemory;
    newAI->recentHits = this->recentHits;

    // Поверхностное копирование указателя
    newAI->enemyBoard = this->enemyBoard;

    return newAI;
}