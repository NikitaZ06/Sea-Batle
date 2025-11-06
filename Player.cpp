#include "Player.hpp"
#include "GameSession.hpp"
#include <iostream>

using namespace std;

// КОНСТРУКТОР - создает динамические объекты
Player::Player(const std::string& playerName, GameBoard& enemyBoardRef)
    : name(playerName), enemyBoard(enemyBoardRef), score(0) {

    // ДИНАМИЧЕСКОЕ СОЗДАНИЕ ОБЪЕКТА - используем оператор new
    ownBoard = new GameBoard();

    // Инициализация динамического массива для статистики
    //initializeStatistics();

    //cout << "Динамический объект GameBoard для игрока создан" << endl;
}

// ДЕСТРУКТОР - очистка динамической памяти
Player::~Player() {
    // УДАЛЕНИЕ ДИНАМИЧЕСКИХ ОБЪЕКТОВ - используем оператор delete
    delete ownBoard;           // Удаляем одиночный объект
    delete[] hitStatistics;    // Удаляем динамический массив

   // cout << "Динамическая память игрока освобождена" << endl;
}

// Выполнение хода игрока
bool Player::makeMove(int x, int y) {
    // Проверка повторного выстрела
    CellState state = enemyBoard.getCell(x, y).getState();
    if (state == CellState::HIT || state == CellState::MISS) {
        cout << "Вы уже стреляли в эту клетку!" << endl;
        return false;
    }

    // Выстрел по полю противника
    bool isHit = enemyBoard.receiveShot(x, y);

    // Обновление статистики
    //updateStatistics(x, y, isHit);

    return isHit;
}

// Обработка выстрела по полю игрока
bool Player::receiveShot(int x, int y) {
    // Передаем выстрел на собственное поле
    return ownBoard->receiveShot(x, y);  // Используем -> для доступа к методам через указатель
}

// Проверка проигрыша игрока (все корабли потоплены)
bool Player::hasLost() const {
    return ownBoard->isGameOver();  // -> для доступа через указатель
}

// Получение имени игрока
string Player::getName() const {
    return name;
}

// Отображение обоих полей (игрока и противника)
void Player::displayBoards() const {
    cout << "=== ВАШЕ ПОЛЕ ===";
    cout << "      ";
    cout << "=== ПОЛЕ ПРОТИВНИКА ===" << endl;

    // Верхние буквенные координаты для обоих полей
    cout << "  ";
    for (int i = 0; i < GameSession::SIZE_BOARD; i++) {
        cout << " " << static_cast<char>('A' + i);
    }
    cout << "     ";
    for (int i = 0; i < GameSession::SIZE_BOARD; i++) {
        cout << " " << static_cast<char>('A' + i);
    }
    cout << endl;

    // Отображение строк обоих полей
    for (int i = 0; i < GameSession::SIZE_BOARD; i++) {
        // Левое поле (собственное)
        cout << (i + 1) << (i < GameSession::SIZE_BOARD-1 ? " " : "");  // Номер строки с выравниванием
        for (int j = 0; j < GameSession::SIZE_BOARD; j++) {
            CellState state = ownBoard->getCell(i, j).getState();
            char symbol = '~';

            // Определение символа для отображения
            switch (state) {
            case CellState::EMPTY: symbol = '~'; break;
            case CellState::SHIP: symbol = 'S'; break;       // На своем поле показываем корабли
            case CellState::HIT: symbol = 'X'; break;
            case CellState::MISS: symbol = 'O'; break;
            }
            cout << " " << symbol;
        }

        cout << "   ";  // Разделитель между полями

        // Правое поле (противника)
        cout << (i + 1) << (i < GameSession::SIZE_BOARD-1 ? " " : "");
        for (int j = 0; j < GameSession::SIZE_BOARD; j++) {
            CellState state = enemyBoard.getCell(i, j).getState();  // Используем ссылку
            char symbol = '~';

            // На поле противника не показываем неподбитые корабли
            switch (state) {
            case CellState::EMPTY:
            case CellState::SHIP:  // Корабли противника не показываем!
                symbol = '~'; break;
            case CellState::HIT: symbol = 'X'; break;
            case CellState::MISS: symbol = 'O'; break;
            }
            cout << " " << symbol;
        }
        cout << endl;
    }

    // Отображение статистики
   // displayStatistics();
}