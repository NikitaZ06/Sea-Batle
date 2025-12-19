#include "Player.hpp"
#include "GameSession.hpp"
#include <iostream>
#include <memory>
#include <iomanip>
using namespace std;

// Инициализация статического поля
int Player::playerCount = 0;

// Конструктор 1: создание игрока без противника
Player::Player(const std::string& playerName)
    : GameObject(playerName),    // Вызов конструктора базового класса
    enemyBoard(nullptr),       // Противник пока не установлен
    score(0) {                 // Начальный счет

    // Создаем собственное игровое поле
    ownBoard = std::make_unique<GameBoard>();

    // Увеличиваем счетчик игроков
    playerCount++;
}

// Конструктор 2: создание игрока с указанием противника
Player::Player(const std::string& playerName, GameBoard& enemyBoardRef)
    : GameObject(playerName),
    enemyBoard(&enemyBoardRef),  // Сохраняем указатель на поле противника
    score(0) {

    ownBoard = std::make_unique<GameBoard>();
    playerCount++;
}


void Player::update() {
    std::cout << "Игрок " << name << " обновляет свою стратегию" << std::endl;
    score += 1; // Например, увеличиваем счет за время
}
GameObject* Player::clone() const {
    // Глубокое клонирование - создаем полностью новый объект
    Player* newPlayer = new Player(name);
    newPlayer->score = this->score;
    // Копируем игровое поле (используем конструктор копирования GameBoard)
    if (ownBoard) {
        newPlayer->ownBoard = std::make_unique<GameBoard>(*ownBoard);
    }
    newPlayer->enemyBoard = this->enemyBoard; // Поверхностное копирование указателя
    return newPlayer;
}
// ПЕРЕОПРЕДЕЛЕНИЕ 1: Отображение информации об игроке
void Player::display() const {
    GameObject::display();

    // Затем добавляем специфичную для Player информацию
    std::cout << "  Тип: Игрок-человек" << std::endl;
    std::cout << "  Счет: " << score << std::endl;

    if (enemyBoard) {
        std::cout << "  Статус: В игре (противник установлен)" << std::endl;
    }
    else {
        std::cout << "  Статус: Ожидает противника" << std::endl;
    }
}

// ПЕРЕОПРЕДЕЛЕНИЕ 2: Получение типа объекта
std::string Player::getType() const {
    return "Игрок (Player)";
}

// ПЕРЕОПРЕДЕЛЕНИЕ 3: Выполнение действия
void Player::performAction() const {
    std::cout << name << " анализирует поле противника..." << std::endl;
    std::cout << "  'Мой ход! Пора атаковать!'" << std::endl;
}
//  Перегруженная версия   с параметром
void Player::performAction(const std::string& target) const {
    std::cout << name << " целенаправленно атакует " << target << "!" << std::endl;
    std::cout << "  'Сосредоточенный удар по " << target << "!'" << std::endl;
}
// Игровой метод: выполнение хода
bool Player::makeMove(int x, int y) {
    // Проверяем, установлен ли противник
    if (!enemyBoard) {
        std::cout << "Ошибка: противник не установлен!" << std::endl;
        return false;
    }

    // Проверка координат на валидность
    if (x < 0 || x >= GameSession::BOARD_SIZE ||
        y < 0 || y >= GameSession::BOARD_SIZE) {
        std::cout << "Ошибка: неверные координаты!" << std::endl;
        return false;
    }
    Cell& targetCell = enemyBoard->getCell(x, y);
    if (targetCell.getState() == CellState::HIT ||
        targetCell.getState() == CellState::MISS) {
        std::cout << "Вы уже стреляли в эту клетку!" << std::endl;
        return false;
    }

    std::cout << name << " стреляет в "
        << static_cast<char>('A' + y) << (x + 1) << "... ";

    // Выстрел по полю противника
    bool isHit = enemyBoard->receiveShot(x, y);

    if (isHit) {
        std::cout << "ПОПАДАНИЕ!" << std::endl;
        score += 10;  // Увеличиваем счет за попадание
    }
    else {
        std::cout << "ПРОМАХ!" << std::endl;
    }

    return isHit;
}

// Обработка выстрела противника
bool Player::receiveShot(int x, int y) {
    if (!ownBoard) {
        std::cout << "Ошибка: игровое поле не инициализировано!" << std::endl;
        return false;
    }

    // Передаем выстрел на свое поле
    return ownBoard->receiveShot(x, y);
}

// Проверка поражения
bool Player::hasLost() const {
    if (!ownBoard) return true;
    return ownBoard->isGameOver();
}

// Отображение игровых полей
void Player::displayBoards() const {
    std::cout << "\n=== ИГРОВАЯ СИТУАЦИЯ ===" << std::endl;
    std::cout << "Игрок: " << name << std::endl;
    std::cout << "Счет: " << score << std::endl;

    if (!ownBoard || !enemyBoard) {
        std::cout << "Игровые поля не готовы!" << std::endl;
        return;
    }

    std::cout << "\nВаше поле                    Поле противника " << std::endl;
    std::cout << "----------------------        -------------------------------" << std::endl;

    // Получаем оба поля в виде строк
    std::vector<std::string> ownBoardLines;
    std::vector<std::string> enemyBoardLines;

    // Выводим заголовки с буквами
    std::cout << "  ";
    for (int i = 0; i < GameSession::BOARD_SIZE; i++) {
        std::cout << " " << static_cast<char>('A' + i);
    }
    std::cout << "        ";
    for (int i = 0; i < GameSession::BOARD_SIZE; i++) {
        std::cout << " " << static_cast<char>('A' + i);
    }
    std::cout << std::endl;

    // Выводим поля построчно
    for (int i = 0; i < GameSession::BOARD_SIZE; i++) {
        // Левое поле (свое)
        std::cout << std::setw(2) << (i + 1);
        for (int j = 0; j < GameSession::BOARD_SIZE; j++) {
            CellState state = ownBoard->getCell(i, j).getState();
            char symbol = '~';
            switch (state) {
            case CellState::EMPTY: symbol = '~'; break;
            case CellState::SHIP: symbol = 'S'; break; // Показываем корабли
            case CellState::HIT: symbol = 'X'; break;
            case CellState::MISS: symbol = 'O'; break;
            }
            std::cout << " " << symbol;
        }

        // Разделитель между полями
        std::cout << "      ";

        // Правое поле (противника)
        cout << setw(2) << (i + 1);
        for (int j = 0; j < GameSession::BOARD_SIZE; j++) {
            CellState state = enemyBoard->getCell(i, j).getState();
            char symbol = '~';
            switch (state) {
            case CellState::EMPTY: symbol = '~'; break;
            case CellState::SHIP: symbol = '~'; break; // Не показываем корабли противника
            case CellState::HIT: symbol = 'X'; break;
            case CellState::MISS: symbol = 'O'; break;
            }
            std::cout << " " << symbol;
        }
        std::cout << std::endl;
    }

    std::cout << "\nОбозначения:" << std::endl;
    std::cout << "  ~ - вода (пустая клетка)" << std::endl;
    std::cout << "  S - ваш корабль (видно только на вашем поле)" << std::endl;
    std::cout << "  X - попадание" << std::endl;
    std::cout << "  O - промах" << std::endl;
}

// Статический метод - получение количества игроков
int Player::getPlayerCount() {
    return playerCount;
}
//  Перегрузка оператора присваивания для объектов базового класса
Player& Player::operator=(const GameObject& other) {
    std::cout << "[Player] Оператор присваивания для GameObject: ";

    // 1. Присваиваем базовую часть
    GameObject::operator=(other);  // Вызываем оператор присваивания базового класса

    // 2. Проверяем, является ли other на самом деле Player
    const Player* otherPlayer = dynamic_cast<const Player*>(&other);
    if (otherPlayer) {
        // Если other - Player, копируем все поля
        std::cout << "копирование Player-полей" << std::endl;
        score = otherPlayer->score;
        // ownBoard нельзя просто скопировать (unique_ptr), создаем новый
        if (otherPlayer->ownBoard) {
            ownBoard = std::make_unique<GameBoard>(*(otherPlayer->ownBoard));
        }
        enemyBoard = otherPlayer->enemyBoard;  // Копируем указатель
    }
    else {
        // Если other - не Player, сбрасываем поля к значениям по умолчанию
        std::cout << "сброс Player-полей к значениям по умолчанию" << std::endl;
        score = 0;
        ownBoard = std::make_unique<GameBoard>();
        enemyBoard = nullptr;
    }

    return *this;
}

//  Обычный оператор присваивания для объектов того же класса
Player& Player::operator=(const Player& other) {
    std::cout << "[Player] Оператор присваивания для Player" << std::endl;

    if (this == &other) return *this;  // Проверка на самоприсваивание

    // 1. Присваиваем базовую часть
    GameObject::operator=(other);

    // 2. Присваиваем поля Player
    score = other.score;

    // 3. Глубокое копирование игрового поля
    if (other.ownBoard) {
        ownBoard = std::make_unique<GameBoard>(*(other.ownBoard));
    }
    else {
        ownBoard.reset();
    }

    // 4. Копируем указатель (поверхностное копирование)
    enemyBoard = other.enemyBoard;

    return *this;
}