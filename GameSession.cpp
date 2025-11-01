#include "GameSession.h"
#include <iostream>
#include <limits>
#include "ShipPlacer.h"

using namespace std;

// КОНСТРУКТОР GameSession - создает все объекты динамически
GameSession::GameSession(const std::string& playerName)
    : isPlayerTurn(true), moveCount(0), maxMoves(100) {

    // ДИНАМИЧЕСКОЕ СОЗДАНИЕ ОБЪЕКТОВ с помощью оператора NEW

    // 1. Создаем поле AI
    aiBoard = new GameBoard();

    // 2. Создаем игрока, передавая ссылку на поле AI
    humanPlayer = new Player(playerName, *aiBoard);  

    // 3. Создаем AI, передавая ссылки на оба поля
    computerAI = new AI(*aiBoard, humanPlayer->getOwnBoard());

    // 4. Создаем ДИНАМИЧЕСКИЙ МАССИВ для истории ходов
    moveHistory = new string[maxMoves];

    cout << "Динамические объекты созданы!" << endl;
}

// ДЕСТРУКТОР GameSession - очистка всей динамической памяти
GameSession::~GameSession() {
    // ОЧИСТКА ПАМЯТИ 

    delete computerAI;    // Удаляем AI
    delete humanPlayer;   // Удаляем игрока
    delete aiBoard;       // Удаляем поле AI
    delete[] moveHistory; // Удаляем динамический массив (используем delete[])

    cout << "Динамическая память освобождена!" << endl;
}

// Демонстрация работы с динамическими объектами и массивами
void GameSession::DynamicObjects() {
    cout << "\n=== ДЕМОНСТРАЦИЯ ДИНАМИЧЕСКИХ ОБЪЕКТОВ ===" << endl;

    // 1. ДИНАМИЧЕСКИЙ МАССИВ ОБЪЕКТОВ КЛАССА Cell
    cout << "1. Создание динамического массива объектов Cell..." << endl;
    Cell* cellArray = new Cell[5];  // Массив из 5 объектов Cell

    // Работа с массивом объектов
    for (int i = 0; i < 5; i++) {
        cellArray[i].setHasShip(i % 2 == 0);  // Устанавливаем корабли в четные ячейки
        cout << "Cell[" << i << "]: hasShip = " << cellArray[i].getHasShip() << endl;
    }

    delete[] cellArray;  // Очистка динамического массива объектов
    cout << "Динамический массив Cell удален" << endl;

    // 2. МАССИВ ДИНАМИЧЕСКИХ ОБЪЕКТОВ GameBoard
    cout << "\n2. Создание массива динамических объектов GameBoard..." << endl;
    GameBoard** boardArray = new GameBoard * [3];  // Массив указателей на GameBoard

    // Создаем каждый объект динамически
    for (int i = 0; i < 3; i++) {
        boardArray[i] = new GameBoard();  // Создаем объект в куче
        cout << "GameBoard " << i << " создан, размер: " << boardArray[i]->getSize() << endl;
    }

    // Очистка массива динамических объектов
    for (int i = 0; i < 3; i++) {
        delete boardArray[i];  // Удаляем каждый объект
    }
    delete[] boardArray;  // Удаляем массив указателей
    cout << "Массив динамических объектов GameBoard удален" << endl;
}

// Демонстрация работы с указателями и ссылками
void GameSession::YkazObject() {
    cout << "\n=== ДЕМОНСТРАЦИЯ УКАЗАТЕЛЕЙ И ССЫЛОК ===" << endl;

    // 1. РАБОТА С УКАЗАТЕЛЯМИ
    cout << "1. Работа с указателями:" << endl;
    int x = 5;
    int* ptr = &x;  // Указатель хранит адрес переменной x

    cout << "   Значение x: " << x << endl;
    cout << "   Адрес x: " << &x << endl;
    cout << "   Значение через указатель: " << *ptr << endl;  // * - разыменование

    *ptr = 10;  // Изменяем значение через указатель
    cout << "   Новое значение x после *ptr = 10: " << x << endl;

    // 2. РАБОТА С ССЫЛКАМИ
    cout << "\n2. Работа со ссылками:" << endl;
    int y = 20;
    int& ref = y;  // Ссылка - псевдоним для переменной y

    cout << "   Значение y: " << y << endl;
    cout << "   Значение через ссылку: " << ref << endl;

    ref = 30;  // Изменяем значение через ссылку
    cout << "   Новое значение y после ref = 30: " << y << endl;

    // 3. УКАЗАТЕЛИ НА МЕТОДЫ КЛАССА
    cout << "\n3. Указатели на методы класса:" << endl;

    // Объявляем указатель на метод класса GameBoard
    bool (GameBoard:: * methodPtr)(int, int) = &GameBoard::receiveShot;

    // Вызов метода через указатель на метод
    bool result = (aiBoard->*methodPtr)(0, 0);  // ->* - специальный оператор
    cout << "   Результат выстрела через указатель на метод: " << result << endl;

    // 4. ПЕРЕДАЧА ОБЪЕКТОВ ПО ССЫЛКЕ И УКАЗАТЕЛЮ
    cout << "\n4. Передача объектов по ссылке и указателю:" << endl;

    // Ссылка не может быть перенаправлена, указатель - может
    GameBoard& boardRef = *aiBoard;  // Ссылка на существующий объект
    GameBoard* boardPtr = aiBoard;   // Указатель на тот же объект

    cout << "   Размер поля через ссылку: " << boardRef.getSize() << endl;
    cout << "   Размер поля через указатель: " << boardPtr->getSize() << endl;
}

// Основной метод запуска игры
void GameSession::startGame() {
    cout << "=== МОРСКОЙ БОЙ ===" << endl;
    cout << "Добро пожаловать, " << humanPlayer->getName() << "!" << endl;

    // Демонстрация работы с динамической памятью
   // demonstrateDynamicObjects();
    //demonstratePointersAndReferences();

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
    cout << "--- ВАШ ХОД ---" << endl;

    int x, y;
    bool validInput = false;

    // Цикл ввода координат
    while (!validInput) {
        string input;
        cout << "Введите координаты (например A1): ";
        cin >> input;

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
        if (x < 0 || x >= 10 || y < 0 || y >= 10) {
            cout << "Координаты вне поля!" << endl;
            continue;
        }

        validInput = true;
    }

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
    computerAI->makeMove();

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
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            CellState aiState = aiBoard->getCell(i, j).getState();
            humanPlayer->getEnemyBoard().getCell(i, j).setState(aiState);
        }
    }
}
