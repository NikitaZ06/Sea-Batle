#include "GameSession.hpp"
#include <iostream>
#include <limits>
#include "ShipPlacer.hpp"
#include <memory>

using namespace std;

// КОНСТРУКТОР GameSession - создает все объекты динамически
GameSession::GameSession(const std::string& playerName)
    : isPlayerTurn(true), moveCount(0), maxMoves(KOLVO_CELLS) {

    // СОЗДАНИЕ ОБЪЕКТОВ ЧЕРЕЗ УМНЫЕ УКАЗАТЕЛИ

    // 1. Создаем поле AI через make_unique
    aiBoard = std::make_unique<GameBoard>();

    // 2. Создаем игрока, передавая ссылку на поле AI
    humanPlayer = std::make_unique<Player>(playerName, *aiBoard);

    // 3. Создаем AI, передавая ссылки на оба поля
    computerAI = std::make_unique<AI>(*aiBoard, humanPlayer->getOwnBoard());

    // 4. Создаем ДИНАМИЧЕСКИЙ МАССИВ для истории ходов
    moveHistory = std::make_unique<std::string[]>(maxMoves);
    aiHistory = std::make_unique<std::string[]>(maxMoves);

    cout << "Объекты созданы через умные указатели!" << endl;
}



// Основной метод запуска игры
void GameSession::startGame() {
    cout << "=== МОРСКОЙ БОЙ ===" << endl;
    cout << "Добро пожаловать, " << humanPlayer->getName() << "!" << endl;


    // 1. расстановка кораблей
    setupGame();
    cout << "\nНачальная расстановка:" << endl;
    humanPlayer->displayBoards();
    cout << endl;

    // 2. ОСНОВНОЙ ИГРОВОЙ ЦИКЛ
    while (!isGameOver()) {
        if (isPlayerTurn) {
            playerTurn();  // Ход игрока
        }
        else {
            aiTurn();      // Ход компьютера

            // Показываем текущую ситуацию после хода AI
            cout << "\n=== ТЕКУЩАЯ СИТУАЦИЯ ===" << endl;
            humanPlayer->displayBoards();
            cout << endl;
        }

        isPlayerTurn = !isPlayerTurn;  // Передача хода
    }

    // 3. ЗАВЕРШЕНИЕ ИГРЫ
    cout << "\n=== Итог ===" << endl;
    humanPlayer->displayBoards();
    displayMoveHistory();  // Показываем историю ходов
    showStats(*this);
    cout << "\n=== ИГРА ЗАВЕРШЕНА ===" << endl;
    if (humanPlayer->hasLost()) {
        cout << "ВЫ ПРОИГРАЛИ!" << endl;
    }
    else {
        cout << "ВЫ ПОБЕДИЛИ!" << endl;
    }
}

// Настройка игры - расстановка кораблей
void GameSession::setupGame() {
    cout << "\n--- РАССТАНОВКА КОРАБЛЕЙ ---" << endl;

    // ДИНАМИЧЕСКОЕ СОЗДАНИЕ И УДАЛЕНИЕ ShipPlacer для игрока
    ShipPlacer* playerPlacer = new ShipPlacer(humanPlayer->getOwnBoard());//передаём ссылку на поле для через метод
    cout << "Авторастановка:" << endl;
    playerPlacer->AutoPlaceShips();//автоматическая расстановка
    delete playerPlacer;  // Очистка памяти

    // ДИНАМИЧЕСКОЕ СОЗДАНИЕ И УДАЛЕНИЕ ShipPlacer для AI
    cout << "\nПротивник расставляет корабли..." << endl;
    ShipPlacer* aiPlacer = new ShipPlacer(*aiBoard);
    aiPlacer->AutoPlaceShips();
    delete aiPlacer;  // Очистка памяти

    cout << "\nНАЧИНАЕМ ИГРУ!" << endl;
}

// Ход игрока
void GameSession::playerTurn() {

    int x, y;
    bool validInput = false;


    while (!validInput) {
        try {
            string input;
            cout << "Введите координаты (например A1): ";
            cin >> input;

            if (input == "S" || input == "s") {
                // Показываем текущую статистику
                showStats(*this);
                cout << "Нажмите Enter для продолжения...";
                cin.ignore();
                cin.get();
            }

            if (input.length() > 2) {  
                throw invalid_argument("Неверный формат! Нужно 2 символа.");
                
            }

            // Преобразование буквенной координаты
            y = toupper(input[0]) - 'A';
            if (y < 0 || y >= SIZE_BOARD) {
                throw out_of_range("Буква должна быть от A до " + string(1, 'A' + SIZE_BOARD - 1));
            }

            // Преобразование числовой координаты
            x = stoi(input.substr(1)) - 1;
            if (x < 0 || x >= SIZE_BOARD) {
                throw out_of_range("Число должно быть от 1 до " + to_string(SIZE_BOARD));
            }

            validInput = true;
        }
        catch (const invalid_argument& e) {
            cout << "Ошибка формата: " << e.what() << endl;
            cin.clear();
            cin.ignore(1000, '\n');
        }
        catch (const out_of_range& e) {
            cout << "Ошибка диапазона: " << e.what() << endl;
        }
        catch (const exception& e) {
            cout << "Неожиданная ошибка: " << e.what() << endl;
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    /*
        // Цикл ввода координат
        while (!validInput) {
            cout << "--- ВАШ ХОД ---" << endl;
            cout << "Команды: S - статистика, Q - выйти" << endl;  // Добавили команду статистики

            string input;
            cout << "Введите координаты или команду: ";
            cin >> input;

            // Обработка команд
            if (input == "S" || input == "s") {
                // Показываем текущую статистику
                showStats(*this);
                cout << "Нажмите Enter для продолжения...";
                cin.ignore();
                cin.get();
                return;  // Пропускаем ход
            }
            else if (input == "Q" || input == "q") {
                cout << "Выход из игры..." << endl;
                exit(0);
            }
           // string input;
           // cout << "Введите координаты (например A1): ";
           // cin >> input;

            // Проверка формата ввода
            if (input.length() < 2) {
                cout << "Неверный формат!" << endl;
                continue;
            }

            // Преобразование буквенной координаты в число (A=0, B=1, ...)
            y = toupper(input[0]) - 'A';

            // Преобразование числовой координаты
            try {
                x = stoi(input.substr(1)) - 1;
            }
            catch (...) {
                cout << "Неверные координаты!" << endl;
                continue;
            }

            // Проверка границ поля
            if (x < 0 || x >= SIZE_BOARD || y < 0 || y >= SIZE_BOARD) {
                cout << "Координаты вне поля!" << endl;
                continue;
            }

            validInput = true;
        }*/

        // Проверка, не стреляли ли уже в эту клетку
        //Объукту aiCellState через метод из GameBoard присваиваем значение клетки через класс CellState
    CellState aiCellState = aiBoard->getCell(x, y).getState();

    if (aiCellState == CellState::HIT || aiCellState == CellState::MISS) {
        cout << "Вы уже стреляли в эту клетку! Ход пропускается." << endl;
        addMoveToHistory("Игрок: повторный выстрел в " + string(1, 'A' + y) + to_string(x + 1));
    }
    else {
        // Выстрел по полю AI
        bool wasHit = aiBoard->receiveShot(x, y);

        // Обновление поля противника у игрока
        if (wasHit) {
            cout << "ПОПАДАНИЕ! Вы попали в корабль противника!" << endl;
            humanPlayer->getEnemyBoard().getCell(x, y).setState(CellState::HIT);
            addMoveToHistory("Игрок: попадание в " + string(1, 'A' + y) + to_string(x + 1));
        }
        else {
            cout << "ПРОМАХ!" << endl;
            humanPlayer->getEnemyBoard().getCell(x, y).setState(CellState::MISS);
            addMoveToHistory("Игрок: промах в " + string(1, 'A' + y) + to_string(x + 1));
        }
    }

    // Пауза перед передачей хода
    cout << "Нажмите Enter для передачи хода противнику...";
    cin.ignore();
    cin.get();

}

// Ход компьютера
void GameSession::aiTurn() {
    cout << "--- ХОД ПРОТИВНИКА ---" << endl;

    // AI делает ход через свой метод makeMove()
    if (computerAI->makeMove()) {
        if (moveCount < maxMoves) {
            aiHistory[moveCount++] = "Противник попадение";
        }
    }
    else {
        aiHistory[moveCount++] = "Противник промах";
    }

    // Пауза для удобства 
    cout << "Нажмите Enter...";
    cin.ignore();
    cin.get();
}

// Проверка окончания игры
bool GameSession::isGameOver() {
    // Игра заканчивается, если кто-то проиграл
    return humanPlayer->hasLost() || computerAI->hasLost();
}

// Добавление хода в историю
void GameSession::addMoveToHistory(const std::string& move) {
    // Проверяем, не переполнен ли массив
    if (moveCount < maxMoves) {
        moveHistory[moveCount++] = move;  // Добавляем ход и увеличиваем счетчик
    }
}

// Отображение истории ходов
void GameSession::displayMoveHistory() const {
    cout << "\n=== ИСТОРИЯ ХОДОВ (" << moveCount << " ходов) ===" << endl;
    for (int i = 0; i < moveCount; i++) {
        cout << i + 1 << ". " << moveHistory[i] << endl;
    }
}

// Синхронизация полей 
void GameSession::syncPlayerEnemyBoard() {
    // Копируем все состояния из aiBoard в enemyBoard игрока
    for (int i = 0; i < SIZE_BOARD; i++) {
        for (int j = 0; j < SIZE_BOARD; j++) {
            CellState aiState = aiBoard->getCell(i, j).getState();
            humanPlayer->getEnemyBoard().getCell(i, j).setState(aiState);
        }
    }
}
void showStats(const GameSession& session) {
    std::cout << "\n=== СТАТИСТИКА ИГРЫ ===" << std::endl;

    int totalShots = 0;
    int playerShots = 0;
    int playerHits = 0;
    int aiShots = 0;
    int aiHits = 0;

    // Анализируем историю ходов
    for (int i = 0; i < session.moveCount; i++) {
        const std::string& move = session.moveHistory[i];
        const std::string& aimove = session.aiHistory[i];
        totalShots++;

        if (move.find("Игрок") != std::string::npos) {
            playerShots++;
            if (move.find("попадание") != std::string::npos) {
                playerHits++;
            }
        }
        if (aimove.find("Противник") != std::string::npos) {
            aiShots++;
            if (aimove.find("попадание") != std::string::npos) {
                aiHits++;
            }
        }
    }

    // Выводим простую статистику
    std::cout << "Всего ходов: " << totalShots << std::endl;
    std::cout << "---" << std::endl;
    std::cout << "Игрок: " << playerHits << " попаданий из " << playerShots << " выстрелов" << std::endl;
    std::cout << "Компьютер: " << aiHits << " попаданий из " << aiShots << " выстрелов" << std::endl;

    // Простой расчет процентов
    if (playerShots > 0) {
        int playerPercent = (playerHits * 100) / playerShots;
        std::cout << "Ваша точность: " << playerPercent << "%" << std::endl;
    }

    if (aiShots > 0) {
        int aiPercent = (aiHits * 100) / aiShots;
        std::cout << "Точность компьютера: " << aiPercent << "%" << std::endl;
    }

    // Простой анализ победителя
    if (session.humanPlayer->hasLost()) {
        std::cout << "ПОБЕДИТЕЛЬ: КОМПЬЮТЕР" << std::endl;
    }
    else {
        std::cout << "ПОБЕДИТЕЛЬ: ИГРОК" << std::endl;
    }
}
