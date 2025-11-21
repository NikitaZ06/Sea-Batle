using System;
using System.Collections.Generic;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sea_battle
{
    // Класс для автоматической расстановки кораблей на игровом поле
    public class ShipPlacer
    {
        private const int SIZE = 10;
        private GameBoard board; // Ссылка на игровое поле
        private int[] tipShip = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 }; // Размеры кораблей для расстановки

        private Random random; // Генератор случайных чисел

        // Конструктор класса ShipPlacer
        public ShipPlacer(GameBoard gameBoard)
        {
            board = gameBoard;
            random = new Random();

            // Стандартный флот для морского боя:
            // 1x4, 2x3, 3x2, 4x1
        }

        // Автоматическая расстановка всех кораблей на поле
        public bool AutoPlaceShips()
        {
            Console.WriteLine("=== АВТОМАТИЧЕСКАЯ РАССТАНОВКА ===");

            // Расставляем корабли от большего к меньшему - большие корабли требуют больше места
            for (int i = 0; i < SIZE; i++)
            {
                int placeLen = tipShip[i]; // Размер корабля
                bool placed = false;       // Размещен или нет 
                int attempts = 0;
                const int MAX_ATTEMPTS = 100;
                int horizontalRandom = 0;

                while (!placed && attempts < MAX_ATTEMPTS)
                {
                    // Генерируем случайные координаты
                    int x = random.Next(0, 10); // От 0 до 9 включительно
                    int y = random.Next(0, 10);

                    bool horizontal;
                    if (horizontalRandom % 2 == 0)
                    {
                        horizontal = true;
                    }
                    else
                    {
                        horizontal = false;
                    }
                    horizontalRandom++;

                    // Пытаемся разместить корабль
                    if (board.PlaceShip(x, y, placeLen, horizontal))
                    {
                        placed = true;
                        Console.WriteLine($" Корабль {placeLen} палуб размещен");
                    }
                    attempts++;
                }

                if (!placed)
                {
                    Console.WriteLine($"Не удалось разместить корабль длиной {placeLen}. Попробуйте заново");
                    Console.WriteLine($"Попытка номер {attempts}");
                    return false;
                }
            }

            Console.WriteLine("Закончено");
            return true;
        }
    }
}
