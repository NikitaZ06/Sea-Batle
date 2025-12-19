using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// NewBehaviors.cs
namespace Sea_battle
{
    // Стратегическое размещение кораблей
    public class StrategicPlacementBehavior : IShipPlacementBehavior
    {
        private Random random;

        public StrategicPlacementBehavior()
        {
            random = new Random();
        }

        public bool PlaceShips(GameBoard board)
        {
            Console.WriteLine("Стратегическое размещение кораблей...");

            // Размещаем корабли по углам и краям
            int[] shipSizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

            foreach (int size in shipSizes)
            {
                bool placed = false;
                int attempts = 0;

                while (!placed && attempts < 50)
                {
                    // Стратегия: большие корабли размещаем по краям
                    if (size >= 3)
                    {
                        bool horizontal = random.Next(0, 2) == 0;
                        int x = random.Next(0, 2) == 0 ? 0 : 10 - (horizontal ? 1 : size);
                        int y = random.Next(0, 2) == 0 ? 0 : 10 - (horizontal ? size : 1);

                        if (board.PlaceShip(x, y, size, horizontal))
                        {
                            placed = true;
                        }
                    }
                    else
                    {
                        // Маленькие корабли размещаем в центре
                        int x = random.Next(3, 7);
                        int y = random.Next(3, 7);
                        bool horizontal = random.Next(0, 2) == 0;

                        if (board.PlaceShip(x, y, size, horizontal))
                        {
                            placed = true;
                        }
                    }
                    attempts++;
                }

                if (!placed)
                {
                    // Если не удалось разместить стратегически, используем обычный метод
                    return new AutoPlacementBehavior().PlaceShips(board);
                }
            }

            return true;
        }

        public string GetPlacementInfo()
        {
            return "Стратегическое размещение";
        }
    }

    /// <summary>
    /// Помощник при стрельбе (подсказки игроку)
    /// </summary>
    public class AssistedShootingBehavior : IShootingBehavior
    {
        private string playerName;
        private Random random;
        private List<(int, int)> previousShots;

        public AssistedShootingBehavior(string name)
        {
            playerName = name;
            random = new Random();
            previousShots = new List<(int, int)>();
        }

        public bool Shoot(int x, int y, GameBoard targetBoard)
        {
            // Если игрок не указал координаты, предлагаем подсказку
            if (x < 0 || y < 0)
            {
                Console.WriteLine($"{playerName}: система предлагает выстрел...");
                (x, y) = GetSuggestedShot();
            }

            Console.WriteLine($"{playerName} стреляет в {x},{y} (с подсказкой)");
            previousShots.Add((x, y));

            bool hit = targetBoard.ReceiveShot(x, y);

            if (hit)
            {
                Console.WriteLine("Попадание! Система рекомендует проверить соседние клетки.");
            }

            return hit;
        }

        private (int, int) GetSuggestedShot()
        {
            // Простая логика предложения выстрела
            // В реальной игре здесь была бы более сложная логика

            int x, y;
            do
            {
                x = random.Next(0, 10);
                y = random.Next(0, 10);
            } while (previousShots.Contains((x, y)));

            return (x, y);
        }

        public string GetBehaviorInfo()
        {
            return "Стрельба с помощником";
        }
    }
}