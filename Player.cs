
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System;

// Player.cs
using System;

namespace Sea_battle
{
    public class Player : GameObjectBase
    {
        public Player(string playerName) : base(playerName)
        {
            // Установка поведения по умолчанию
            SetShootingBehavior(new HumanShootingBehavior(playerName));
            SetPlacementBehavior(new AutoPlacementBehavior());

        }

        public Player(string playerName, GameBoard enemyBoardRef) : this(playerName)
        {
            SetEnemyBoard(enemyBoardRef);
        }

        // Реализация IGameObject.DisplayInfo()
        public override void DisplayInfo()
        {
            Console.WriteLine($"=== ИГРОК ===");
            Console.WriteLine($"Имя: {name}");
            Console.WriteLine($"Очки: {score}");
            Console.WriteLine($"Статус: {(HasLost() ? "Проиграл" : "В игре")}");
            Console.WriteLine($"Тип: Человек");

            var shootingBehavior = GetShootingBehavior();
            if (shootingBehavior != null)
                Console.WriteLine($"Поведение стрельбы: {shootingBehavior.GetBehaviorInfo()}");

            var placementBehavior = GetPlacementBehavior();
            if (placementBehavior != null)
                Console.WriteLine($"Поведение размещения: {placementBehavior.GetPlacementInfo()}");
        }

        // Реализация IGameObject.PerformAction()
        public override void PerformAction()
        {
            Console.WriteLine($"{name} обдумывает следующий ход...");
            IncreaseScore(1); // Начисляем очки за размышления
        }

        // Реализация IGameObject.Update()
        public override void Update()
        {
            // Логика обновления состояния игрока
            IncreaseScore(1);
        }

        // Дополнительные методы для отображения
        public void DisplayBoards()
        {
            Console.WriteLine("=== ВАШЕ ПОЛЕ ===\t=== ПОЛЕ ПРОТИВНИКА ===");

            Console.Write("  ");
            for (int i = 0; i < 10; i++) Console.Write(" " + (char)('A' + i));
            Console.Write("\t  ");
            for (int i = 0; i < 10; i++) Console.Write(" " + (char)('A' + i));
            Console.WriteLine();

            for (int i = 0; i < 10; i++)
            {
                // Собственное поле
                Console.Write($"{i + 1,2}");
                for (int j = 0; j < 10; j++)
                {
                    char symbol = GetSymbol(ownBoard.GetCell(i, j).State, true);
                    Console.Write(" " + symbol);
                }

                Console.Write("\t");

                // Поле противника
                Console.Write($"{i + 1,2}");
                for (int j = 0; j < 10; j++)
                {
                    char symbol = GetSymbol(GetEnemyBoard()?.GetCell(i, j).State ?? CellState.Empty, false);
                    Console.Write(" " + symbol);
                }
                Console.WriteLine();
            }
        }

        private char GetSymbol(CellState state, bool isOwnBoard)
        {
            return state switch
            {
                CellState.Empty => '~',
                CellState.Ship => isOwnBoard ? 'S' : '~',
                CellState.Hit => 'X',
                CellState.Miss => 'O',
                _ => '~'
            };
        }
    }
}