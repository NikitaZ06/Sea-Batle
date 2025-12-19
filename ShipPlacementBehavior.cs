using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sea_battle
{
    // Стратегия размещения кораблей
    public interface IShipPlacementBehavior
    {
        bool PlaceShips(GameBoard board);
        string GetPlacementInfo();
    }

    // Автоматическое размещение
    public class AutoPlacementBehavior : IShipPlacementBehavior
    {
        private Random random;

        public AutoPlacementBehavior()
        {
            random = new Random();
        }

        public bool PlaceShips(GameBoard board)
        {

            // Размещаем корабли стандартного размера
            int[] shipSizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

            foreach (int size in shipSizes)
            {
                bool placed = false;
                int attempts = 0;

                while (!placed && attempts < 100)
                {
                    int x = random.Next(0, 10);
                    int y = random.Next(0, 10);
                    bool horizontal = random.Next(0, 2) == 0;

                    if (board.PlaceShip(x, y, size, horizontal))
                    {
                        placed = true;
                    }

                    attempts++;
                }

                if (!placed)
                {
                    Console.WriteLine($"Не удалось разместить корабль размером {size}");
                    return false;
                }
            }

            return true;
        }

        public string GetPlacementInfo()
        {
            return "Автоматическое размещение";
        }
    }

    // Ручное размещение 
    public class ManualPlacementBehavior : IShipPlacementBehavior
    {
        public bool PlaceShips(GameBoard board)
        {
            Console.WriteLine("Ручное размещение кораблей (в разработке)");
            // Здесь будет логика ручного размещения
            return false;
        }

        public string GetPlacementInfo()
        {
            return "Ручное размещение";
        }
    }
}
