#include "GameSession.hpp"
#include <iostream>
#include <limits>
#include "ShipPlacer.hpp"

using namespace std;
// Определение статических констант
const int GameSession::BOARD_SIZE;
const int GameSession::MAX_CELLS;

// Конструктор GameSession
GameSession::GameSession(const std::string& playerName,
    const std::string& aiName,
    int aiDifficulty)
    : isPlayerTurn(true), gameActive(false) {


    //  Создаем AI (сначала, чтобы потом передать его поле игроку)
    ai = make_unique<AI>(aiName, aiDifficulty);

    //  Создаем игрока, передавая ему поле AI
    player = make_unique<Player>(playerName, ai->getOwnBoard());

    //  Устанавливаем AI поле игрока
    ai->setEnemyBoard(player->getOwnBoard());
   
}

// Запуск игры
void GameSession::startGame() {
    if (gameActive) {
        cout << "Игра уже запущена!" << endl;
        return;
    }

    
    cout << "Добро пожаловать, " << player->getName() << "!" << endl;


    // 1. Расстановка кораблей
    setupGame();

    // 2. Показываем начальное состояние
    player->displayBoards();

    
    gameActive = true;
    isPlayerTurn = true;  // Игрок ходит первым

    cout << "\n=== ИГРА НАЧАЛАСЬ! ===" << endl;
    cout << "Первым ходит " << player->getName() << endl;
}

// Настройка игры (расстановка кораблей)
void GameSession::setupGame() {
    cout << "\n--- РАССТАНОВКА КОРАБЛЕЙ ---" << endl;


    // Создаем ShipPlacer для игрока (с умным указателем)
    auto playerPlacer = make_unique<ShipPlacer>(player->getOwnBoard());
    if (playerPlacer->AutoPlaceShips()) {
        cout << "Корабли игрока расставлены успешно!" << endl;
    }
    else {
        cout << "Ошибка при расстановке кораблей игрока!" << endl;
    }


    if (ai->setupShips()) {
        cout << "Корабли противника расставлены!" << endl;
    }
    else {
        cout << "Ошибка при расстановке кораблей противника!" << endl;
    }
}

// Игрок делает ход
bool GameSession::makePlayerMove(int x, int y) {
    if (!gameActive) {
        cout << "Игра не активна!" << endl;
        return false;
    }

    if (!isPlayerTurn) {
        cout << "Сейчас не ваш ход!" << endl;
        return false;
    }

    cout << "\n--- ХОД ИГРОКА ---" << endl;

    // Выполняем ход игрока
    bool hit = player->makeMove(x, y);

    
    // Добавляем ход в историю
    string moveInfo = player->getName() + ": выстрел в " +
        string(1, 'A' + y) + to_string(x + 1) +
        " - " + (hit ? "ПОПАДАНИЕ" : "промах");
    addMoveToHistory(moveInfo, true);

    // Проверяем, не выиграл ли игрок
    if (ai->hasLost()) {
        cout << "\n" << player->getName() << " ПОБЕДИЛ!" << endl;
        gameActive = false;

        // Показываем финальную статистику
        displayGameState();
        showStats(*this);
    }
    else {
        // Переключаем ход на AI
        switchTurn();
    }

    return hit;
}

// AI делает ход
bool GameSession::makeAIMove() {
    if (!gameActive) {
        cout << "Игра не активна!" << endl;
        return false;
    }

    if (isPlayerTurn) {
        cout << "Сейчас ходит игрок!" << endl;
        return false;
    }

    cout << "\n--- ХОД AI ---" << endl;

    // Выполняем ход AI
    bool hit = ai->makeMove();

    // Добавляем ход в историю
    string moveInfo = ai->getName() + " сделал ход - " +
        (hit ? "ПОПАДАНИЕ" : "промах");
    addMoveToHistory(moveInfo, false);

    // Проверяем, не выиграл ли AI
    if (player->hasLost()) {
        cout << "\n" << ai->getName() << " ПОБЕДИЛ!" << endl;
        gameActive = false;

        // Показываем финальную статистику
        displayGameState();
        showStats(*this);
    }
    else {
        // Переключаем ход на игрока
        switchTurn();
    }
    system("cls");
    player->displayBoards();
    return hit;
}


// Переключение очереди хода
void GameSession::switchTurn() {
    isPlayerTurn = !isPlayerTurn;

    if (isPlayerTurn) {
        cout << "\nТеперь ходит " << player->getName() << endl;
    }
    else {
        cout << "\nТеперь ходит " << ai->getName() << endl;
    }
}

// Проверка окончания игры
bool GameSession::isGameOver() const {
    // Игра окончена, если кто-то проиграл ИЛИ игра не активна
    return !gameActive || player->hasLost() || ai->hasLost();
}

// Добавление хода в историю
void GameSession::addMoveToHistory(const std::string& move, bool isPlayerMove) {
    if (isPlayerMove) {
        moveHistory.push_back(move);
    }
    else {
        aiHistory.push_back(move);
    }
}

// Отображение истории ходов
void GameSession::displayMoveHistory() const {
    cout << "\n=== ИСТОРИЯ ХОДОВ ===" << endl;

    cout << "\nХоды игрока (" << moveHistory.size() << "):" << endl;
    for (size_t i = 0; i < moveHistory.size(); i++) {
        cout << i + 1 << ". " << moveHistory[i] << endl;
    }

    cout << "\nХоды AI (" << aiHistory.size() << "):" << endl;
    for (size_t i = 0; i < aiHistory.size(); i++) {
        cout << i + 1 << ". " << aiHistory[i] << endl;
    }
}

// Отображение состояния игры
void GameSession::displayGameState() const {
    cout << "\n=== ТЕКУЩЕЕ СОСТОЯНИЕ ===" << endl;

    // Информация об игроке
    cout << "\n--- ИГРОК ---" << endl;
    player->display();

    // Информация об AI
    cout << "\n--- ПРОТИВНИК ---" << endl;
    ai->display();

    // Чей ход
    cout << "\nОчередь хода: ";
    if (isPlayerTurn) {
        cout << player->getName() << endl;
    }
    else {
        cout << ai->getName() << endl;
    }

    // Игровые поля
    cout << "\n--- ИГРОВЫЕ ПОЛЯ ---" << endl;
    player->displayBoards();
}

// Функция для отображения статистики 
void showStats(const GameSession& session) {
    cout << "\n=== СТАТИСТИКА ИГРЫ ===" << endl;

    // Простая статистика
    int totalMoves = session.moveHistory.size() + session.aiHistory.size();
    cout << "Всего ходов: " << totalMoves << endl;

    // Статистика игрока
    int playerHits = 0;
    for (const auto& move : session.moveHistory) {
        if (move.find("ПОПАДАНИЕ") != string::npos) {
            playerHits++;
        }
    }

    // Статистика AI
    int aiHits = 0;
    for (const auto& move : session.aiHistory) {
        if (move.find("ПОПАДАНИЕ") != string::npos) {
            aiHits++;
        }
    }

    cout << "\n--- ЭФФЕКТИВНОСТЬ ---" << endl;
    if (!session.moveHistory.empty()) {
        double playerAccuracy = (static_cast<double>(playerHits) / session.moveHistory.size()) * 100;
        cout << "Точность игрока: " << playerHits << "/" << session.moveHistory.size()
            << " (" << playerAccuracy << "%)" << endl;
    }

    if (!session.aiHistory.empty()) {
        double aiAccuracy = (static_cast<double>(aiHits) / session.aiHistory.size()) * 100;
        cout << "Точность AI: " << aiHits << "/" << session.aiHistory.size()
            << " (" << aiAccuracy << "%)" << endl;
    }

    // Победитель
    cout << "\n--- РЕЗУЛЬТАТ ---" << endl;
    if (session.player->hasLost()) {
        cout << "ПОБЕДИТЕЛЬ: " << session.ai->getName() << endl;
    }
    else if (session.ai->hasLost()) {
        cout << "ПОБЕДИТЕЛЬ: " << session.player->getName() << endl;
    }
    else {
        cout << "Игра еще продолжается!" << endl;
    }
}
