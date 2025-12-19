// Itog.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

// laba3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
//
#include "GameSession.hpp"
#include "Cell.hpp"
#include "GameBoard.hpp"
#include <iostream>
#include "ShipPlacer.hpp"
#include <locale>
#include <fstream>
#include "ShipInfo.hpp"

using namespace std;

// Точка входа в программу
int main() {
    setlocale(LC_ALL, "Russian");  // Установка русской локали


    cout << "=== МОРСКОЙ БОЙ ===" << endl;
    try {
        std::ofstream file("data.txt");
        if (!file) throw std::runtime_error("Cannot open file");
        file << "Data";
    }
    catch (const std::exception& e) {
        std::cout << "File error: " << e.what() << std::endl;
    }

    // Ввод имени игрока
    string playerName;
    cout << "Введите ваше имя: ";
    cin >> playerName;

    // Проверка на пустое имя
    if (playerName.empty()) {
        playerName = "Игрок";
    }


    // Создаем игровую сессию
    GameSession game(playerName, "Компьютер", 2);

    // Запускаем игру
    game.startGame();

    // Основной игровой цикл
    while (game.isGameActive() && !game.isGameOver()) {
        //game.displayGameState();

        if (game.getIsPlayerTurn()) {
            // Ход игрока
            char column;
            int row;

            cout << "\nВаш ход! Введите координаты (например, A5): ";

            // Очищаем буфер ввода
            cin.clear();
         
            
            // Читаем ввод
            if (!(cin >> column >> row)) {
                cout << "Ошибка ввода! Используйте формат 'A5'" << endl;
                continue;
            }

            // Преобразуем букву в число (A=0, B=1, ...)
            column = toupper(column);
            int y = column - 'A';
            int x = row - 1; // Пользователь вводит 1-10, нам нужно 0-9

            if (x < 0 || x >= 10 || y < 0 || y >= 10) {
                cout << "Неверные координаты! Используйте A-J и 1-10" << endl;
                continue;
            }
            
           if( game.makePlayerMove(x, y)==false){}
        }
        else {
            // Ход компьютера
            cout << "\nНажмите Enter для хода компьютера...";
            cin.ignore();
            // Проверяем, не хочет ли пользователь выйти
            if (cin.peek() == 'Q' || cin.peek() == 'q') {
                cin.get(); // Считываем Q
                cout << "\nИгра завершена досрочно!" << endl;
                // Записываем результат в файл
                return 0;
            }
            cin.get();
            game.makeAIMove();
        }
    }

    cout << "\n=== ИГРА ОКОНЧЕНА ===" << endl;
    ofstream resultFile("game_result.txt", ios::app);
    if (resultFile.is_open()) {
        resultFile << "Игра: " << playerName << " vs " << game.getAI()->getName() << endl;
        if (game.getPlayer()->hasLost()) {
            resultFile << "Победитель: " << game.getAI()->getName() << endl;
        }
        else {
            resultFile << "Победитель: " << playerName << endl;
        }
        resultFile << "Дата: " << __DATE__ << " Время: " << __TIME__ << endl;
        resultFile << "----------------------" << endl;
        resultFile.close();
        cout << "Результат сохранен в файле game_result.txt" << endl;
    }
    cout << "\nСпасибо за игру!" << endl;

    return 0;

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
