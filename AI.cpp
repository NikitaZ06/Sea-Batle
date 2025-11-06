#include "AI.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "ShipPlacer.hpp"
#include "GameSession.hpp"

using namespace std;

// КОНСТРУКТОР AI - инициализирует ссылки и динамическую память
AI::AI(GameBoard& aiBoard, GameBoard& playerBoard)
    : ownBoard(aiBoard), enemyBoard(playerBoard){
    srand(time(0));  // Инициализация генератора случайных чисел
    initializeShotMemory();  // Инициализация динамической памяти для запоминания ходов
}

// ДЕСТРУКТОР AI - очистка динамической памяти
AI::~AI() {
    cleanupShotMemory();  // Очистка динамического двумерного массива
}

// Инициализация динамического двумерного массива для запоминания ходов
void AI::initializeShotMemory() {
    // СОЗДАНИЕ ДИНАМИЧЕСКОГО ДВУМЕРНОГО МАССИВА с помощью new

    //  Создаем массив указателей на строки
    shotMemory = new bool* [GameSession::SIZE_BOARD];

    //  Для каждой строки создаем массив булевых значений
    for (int i = 0; i < GameSession::SIZE_BOARD; i++) {
        shotMemory[i] = new bool[GameSession::SIZE_BOARD];  // Создаем строку из boardSize элементов

        // 3. Инициализируем все значения false (еще не стреляли)
        for (int j = 0; j < GameSession::SIZE_BOARD; j++) {
            shotMemory[i][j] = false;
        }
    }
    //cout << "Динамическая память для AI инициализирована" << endl;
}

// Очистка динамического двумерного массива
void AI::cleanupShotMemory() {

    if (shotMemory != nullptr) {
        for (int i = 0; i < GameSession::SIZE_BOARD; i++) {
            delete[] shotMemory[i];  // Освобождаем память каждой строки
        }
        //  Удаляем массив указателей
        delete[] shotMemory;
        shotMemory = nullptr;  // Обнуляем указатель для безопасности
    }
    cout << "Динамическая память AI очищена" << endl;
}

// Основной метод выполнения хода AI
void AI::makeMove() {
    cout << "Противник стреляет... ";

    int x, y;
    bool validShot = false;
    int attempts = 0;//попытки

    // Поиск случайной свободной клетки 
    while (!validShot && attempts < GameSession::KOLVO_CELLS) {
        // Генерируем случайные координаты
        x = rand() % GameSession::SIZE_BOARD;
        y = rand() % GameSession::SIZE_BOARD;

        // Проверяем через дин.массив - не стреляли ли уже сюда
        if (!shotMemory[x][y]) {
            CellState state = enemyBoard.getCell(x, y).getState();
            // Проверяем, что в эту клетку еще не стреляли
            if (state != CellState::HIT && state != CellState::MISS) {
                validShot = true;
                shotMemory[x][y] = true;  // Помечаем как использованную в динамическом массиве
            }
        }
        attempts++;
    }

    //  если случайный поиск не удался
    if (!validShot) {
        // Последовательный поиск первой свободной клетки
        for (x = 0; x < GameSession::SIZE_BOARD && !validShot; x++) {
            for (y = 0; y < GameSession::SIZE_BOARD && !validShot; y++) {
                if (shotMemory[x][y])continue;//Если стреляли, то пропускаем
                CellState state = enemyBoard.getCell(x, y).getState();
                if (state != CellState::HIT && state != CellState::MISS) {
                     validShot = true;
                     shotMemory[x][y] = true;
                }
            }
        }
    }

    // Вывод информации о выстреле
    cout << "в " << char('A' + y) << (x + 1) << "... ";

    // Совершаем выстрел по полю игрока
    bool wasHit = enemyBoard.receiveShot(x, y);

    // Вывод результата выстрела
    if (wasHit) {
        cout << "ПОПАДАНИЕ!" << endl;
    }
    else {
        cout << "ПРОМАХ!" << endl;
    }
}

// Расстановка кораблей AI
bool AI::setupShips() {
    // Динамическое создание ShipPlacer
    ShipPlacer* placer = new ShipPlacer(ownBoard);  // Создаем объект 

    bool result = placer->AutoPlaceShips();  // расстановка кораблей

    delete placer;  // удаляем динамический объект

    return result;
}