using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sea_battle
{
    /// Класс, представляющий игровое поле
    public class GameBoard
    {
        private const int SIZE = 10;  // Размер поля 10x10
        private Cell[,] grid;         // Двумерный массив клеток

        /// Конструктор - создает поле и инициализирует все ячейки
        public GameBoard()
        {
            // Создаем двумерный массив ячеек
            grid = new Cell[SIZE, SIZE];

            // Инициализируем все ячейки
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    grid[i, j] = new Cell();
                }
            }

            Console.WriteLine($"Конструктор: создано поле {SIZE}x{SIZE} ячеек");
        }

        // Отображение игрового поля
        public void Display(bool showShips = false)
        {
            // Вывод буквенных координат (A-J)
            Console.Write("  ");
            for (int i = 0; i < SIZE; i++)
            {
                Console.Write(" " + (char)('A' + i));
            }
            Console.WriteLine();

            // Вывод поля с цифровыми координатами
            for (int i = 0; i < SIZE; i++)
            {
                Console.Write($"{i + 1,2}");  // Выравнивание цифр //,2 Выравнивание по праому краю занимая 2 символа 

                for (int j = 0; j < SIZE; j++)
                {
                    CellState state = grid[i, j].State;
                    char symbol = '~';  // Символ по умолчанию

                    switch (state)
                    {
                        case CellState.Empty: symbol = '~'; break;
                        case CellState.Ship: symbol = showShips ? 'S' : '~'; break;
                        case CellState.Hit: symbol = 'X'; break;
                        case CellState.Miss: symbol = 'O'; break;
                    }
                    Console.Write(" " + symbol);
                }
                Console.WriteLine();
            }
        }

        // Размещение корабля на поле
        public bool PlaceShip(int x, int y, int size, bool horizontal)
        {
            // Проверяем можно ли разместить корабль
            if (!CanPlaceShip(x, y, size, horizontal))
            {
                return false; // Размещение невозможно
            }

            // Размещаем корабль
            if (horizontal)
            {
                for (int i = 0; i < size; i++)
                {
                    // grid[x, y + i].SetHasShip(true);
                    grid[x, y + i].HasShip = true; ;
                }
            }
            else
            {
                for (int i = 0; i < size; i++)
                {
                    grid[x + i, y].HasShip = true;
                }
            }
            return true; // Размещение успешно
        }

        // Обработка выстрела по полю
        public bool ReceiveShot(int x, int y)
        {
            if (!IsValidPosition(x, y)) return false;

            if (grid[x, y].HasShip)
            {
                grid[x, y].State = CellState.Hit;
                return true;  // Попадание
            }
            else
            {
                grid[x, y].State = CellState.Miss;
                return false; // Промах
            }
        }

        // Проверка окончания игры (все корабли потоплены)
        public bool IsGameOver()
        {
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    // Если есть корабль, который еще не подбит
                    if (grid[i, j].HasShip &&
                        grid[i, j].State != CellState.Hit)
                    {
                        return false;  // Игра не окончена
                    }
                }
            }
            return true;  // Все корабли потоплены
        }

        // Получение размера поля
        public int Size
        {
            get { return SIZE; }
        }
        // Получение ячейки по координата
        public Cell GetCell(int x, int y)
        {
            return grid[x, y];
        }


        // Проверка валидности позиции
        public bool IsValidPosition(int x, int y)
        {
            return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
        }

        /// Проверка возможности размещения корабля
        public bool CanPlaceShip(int x, int y, int size, bool horizontal)
        {
            // Проверка выхода за границы поля
            if (horizontal ? (y + size > SIZE) : (x + size > SIZE))
            {
                return false;
            }
            /* if (horizontal)
            {
                if (y + size > SIZE)
                {
                    return false; // Корабль выходит за правую границу
                }
            }
            else
            {
                if (x + size > SIZE)
                {
                    return false; // Корабль выходит за нижнюю границу
                }
            }*/

            // Проверка что все целевые клетки свободны
            for (int i = 0; i < size; i++)
            {
                int checkX = horizontal ? x : x + i;
                int checkY = horizontal ? y + i : y;

                // Если клетка невалидна или уже занята кораблем
                if (!IsValidPosition(checkX, checkY) ||
                    grid[checkX, checkY].HasShip)
                {
                    return false;
                }
            }

            // Проверка зоны вокруг корабля
            if (!IsAreaClear(x, y, size, horizontal))
            {
                return false;
            }

            return true; // Все проверки пройдены
        }

        // Проверка зоны вокруг корабля (чтобы корабли не касались друг друга)

        public bool IsAreaClear(int x, int y, int size, bool horizontal)
        {
            // Определяем зону проверки (корабль + 1 клетка вокруг)
            int startX = Math.Max(0, x - 1); // Math.Max(0, -1) → 0
            int endX = Math.Min(SIZE - 1, horizontal ? x + 1 : x + size);
            int startY = Math.Max(0, y - 1);
            int endY = Math.Min(SIZE - 1, horizontal ? y + size : y + 1);

            // Проверяем всю зону
            for (int i = startX; i <= endX; i++)
            {
                for (int j = startY; j <= endY; j++)
                {
                    // Если в этой клетке есть корабль
                    if (grid[i, j].HasShip)
                    {
                        return false;
                    }
                }
            }

            return true; // Вся зона чистая
        }
    }
}
