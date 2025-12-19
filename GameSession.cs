
using Sea_battle;
using System;
using System;
using System.Collections.Generic;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Sea_battle
{
    public class GameSession
    {
        private GameBoard aiBoard;
        private Player humanPlayer;
        private AI computerAI;
        private bool isPlayerTurn;

        // Контейнер для игровых объектов
        private List<IGameObject> gameObjects = new List<IGameObject>();


        public GameSession(string playerName)
        {
            aiBoard = new GameBoard();
            humanPlayer = new Player(playerName);
            humanPlayer.SetEnemyBoard(aiBoard);

            computerAI = new AI("Компьютер", 1);
            computerAI.SetEnemyBoard(humanPlayer.GetOwnBoard());

            isPlayerTurn = true;
        }

        public void StartGame()
        {
            Console.WriteLine("=== МОРСКОЙ БОЙ ===");

            // Расстановка кораблей
            humanPlayer.SetupShips();
            computerAI.SetupShips();

            Console.WriteLine("\nНажмите Enter для начала игры...");
            Console.ReadLine();

            PlayGame();
        }

        private void PlayGame()
        {
            while (!humanPlayer.HasLost() && !computerAI.HasLost())
            {
                Console.Clear();
                Console.WriteLine("=== ХОД ИГРЫ ===");

                if (isPlayerTurn)
                {
                    PlayerTurn();
                }
                else
                {
                    ComputerTurn();
                }

                isPlayerTurn = !isPlayerTurn;

                if (!humanPlayer.HasLost() && !computerAI.HasLost())
                {
                    Console.WriteLine("\nНажмите Enter для продолжения...");
                    Console.ReadLine();
                }
            }

            Console.WriteLine("\n=== ИГРА ЗАВЕРШЕНА ===");
            if (humanPlayer.HasLost())
                Console.WriteLine("Вы проиграли! Все ваши корабли потоплены.");
            else
                Console.WriteLine("Поздравляем! Вы победили компьютера!");
        }

        private void PlayerTurn()
        {
            Console.WriteLine("\n--- ВАШ ХОД ---");
            humanPlayer.DisplayBoards();

            int x = -1, y = -1;
            bool validInput = false;

            do
            {
                Console.Write("\nВведите координаты для выстрела (например, A1): ");
                string input = Console.ReadLine()?.ToUpper();

                if (string.IsNullOrEmpty(input) || input.Length < 2)
                {
                    Console.WriteLine("Неверный формат. Пример: A1, B5, J10");
                    continue;
                }

                char letter = input[0];
                if (letter < 'A' || letter > 'J')
                {
                    Console.WriteLine("Буква должна быть от A до J");
                    continue;
                }

                string numberPart = input.Substring(1);
                if (!int.TryParse(numberPart, out int number) || number < 1 || number > 10)
                {
                    Console.WriteLine("Число должно быть от 1 до 10");
                    continue;
                }

                x = number - 1;
                y = letter - 'A';

                // Проверяем, можно ли сделать выстрел в эту клетку
                CellState state = aiBoard.GetCell(x, y).State;
                if (state == CellState.Hit || state == CellState.Miss)
                {
                    Console.WriteLine("Вы уже стреляли в эту клетку. Попробуйте другие координаты.");
                    continue;
                }

                validInput = true;

            } while (!validInput);

            // Выполняем выстрел через игрока
            bool validShot = humanPlayer.Shoot(x, y);
            Console.WriteLine(validShot ? "ПОПАДАНИЕ!" : "ПРОМАХ!");

            // Показываем обновленные поля
            Console.WriteLine("\nРезультат вашего выстрела:");
            humanPlayer.DisplayBoards();
        }

        private void ComputerTurn()
        {
            Console.WriteLine("\n--- ХОД КОМПЬЮТЕРА ---");
            computerAI.MakeMove();
            Console.WriteLine("Компьютер сделал ход.");

            // Показываем обновленное поле игрока
            Console.WriteLine("\nВаше поле после хода компьютера:");
            humanPlayer.OwnBoard.Display(true);
        }
    }
}