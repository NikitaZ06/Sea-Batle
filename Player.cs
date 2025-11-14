using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Laba4
{

    // Класс, представляющий игрока
    public class Player
    {
        private string name;
        private GameBoard ownBoard;
        private GameBoard enemyBoard;  // Ссылка на поле противника
        private int score;

        // Конструктор игрока
        public Player(string playerName, GameBoard enemyBoardRef)
        {
            name = playerName;
            enemyBoard = enemyBoardRef;
            ownBoard = new GameBoard();
            score = 0;
        }

       
        // Выполнение хода игроком
        public bool MakeMove(int x, int y)
        {
            // Проверка повторного выстрела
            CellState state = enemyBoard.GetCell(x, y).GetState();
            if (state == CellState.Hit || state == CellState.Miss)
            {
                Console.WriteLine("Вы уже стреляли в эту клетку!");
                return false;
            }

            // Выстрел по enemyBoard (который ссылается на поле противника)
            bool isHit = enemyBoard.ReceiveShot(x, y);
            return isHit;
        }
        /// Обработка выстрела по собственному полю игрока
        public bool ReceiveShot(int x, int y)
        {
            return ownBoard.ReceiveShot(x, y);
        }

        // Проверка, проиграл ли игрок (все его корабли потоплены)
        public bool HasLost()
        {
            return ownBoard.IsGameOver();
        }

        // Получение имени игрока

        public string GetName()
        {
            return name;
        }

        // Простая проверка для AI (все корабли потоплены)
        private bool AIHasLost()
        {
            return ownBoard.IsGameOver();
        }


        // Отображение обоих игровых полей (своего и противника)
        public void DisplayBoards()
        {
            Console.Write("=== ВАШЕ ПОЛЕ ===");
            Console.Write("      ");
            Console.WriteLine("=== ПОЛЕ ПРОТИВНИКА ===");

            // Верхние координаты (буквы)
            Console.Write("  ");
            for (int i = 0; i < 10; i++)
            {
                Console.Write(" " + (char)('A' + i));
            }
            Console.Write("     ");
            for (int i = 0; i < 10; i++)
            {
                Console.Write(" " + (char)('A' + i));
            }
            Console.WriteLine();

            // Отображение строк полей
            for (int i = 0; i < 10; i++)
            {
                // Левое поле (собственное)
                Console.Write(i + 1);
                Console.Write(i < 9 ? " " : "");  // Выравнивание для двузначных чисел

                for (int j = 0; j < 10; j++)
                {
                    CellState state = ownBoard.GetCell(i, j).GetState();
                    char symbol = '~';

                    switch (state)
                    {
                        case CellState.Empty: symbol = '~'; break;
                        case CellState.Ship: symbol = 'S'; break;
                        case CellState.Hit: symbol = 'X'; break;
                        case CellState.Miss: symbol = 'O'; break;
                    }
                    Console.Write(" " + symbol);
                }

                Console.Write("   ");

                // Правое поле (противника)
                Console.Write(i + 1);
                Console.Write(i < 9 ? " " : "");  // Выравнивание для двузначных чисел

                for (int j = 0; j < 10; j++)
                {
                    CellState state = enemyBoard.GetCell(i, j).GetState();
                    char symbol = '~';

                    switch (state)
                    {
                        case CellState.Empty:
                        case CellState.Ship:
                            symbol = '~'; break;  // На поле противника не показываем корабли
                        case CellState.Hit: symbol = 'X'; break;
                        case CellState.Miss: symbol = 'O'; break;
                    }
                    Console.Write(" " + symbol);
                }
                Console.WriteLine();
            }
        }

        // Геттеры для доступа к полям (аналогично C++ версии)

 
        // Получение собственного поля игрока
        public GameBoard GetOwnBoard()
        {
            return ownBoard;
        }

        // Получение поля противника
        public GameBoard GetEnemyBoard()
        {
            return enemyBoard;
        }
    }
}
