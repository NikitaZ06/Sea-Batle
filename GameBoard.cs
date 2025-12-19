using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System;

namespace Sea_battle
{
    public class GameBoard
    {
        private const int SIZE = 10;
        private Cell[,] grid;

        public GameBoard()
        {
            grid = new Cell[SIZE, SIZE];
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    grid[i, j] = new Cell();
                }
            }
        }

        public void Display(bool showShips = false)
        {
            Console.Write("  ");
            for (int i = 0; i < SIZE; i++)
            {
                Console.Write(" " + (char)('A' + i));
            }
            Console.WriteLine();

            for (int i = 0; i < SIZE; i++)
            {
                Console.Write($"{i + 1,2}");
                for (int j = 0; j < SIZE; j++)
                {
                    CellState state = grid[i, j].State;
                    char symbol = '~';

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

        public bool PlaceShip(int x, int y, int size, bool horizontal)
        {
            if (!CanPlaceShip(x, y, size, horizontal))
                return false;

            if (horizontal)
            {
                for (int i = 0; i < size; i++)
                    grid[x, y + i].HasShip = true;
            }
            else
            {
                for (int i = 0; i < size; i++)
                    grid[x + i, y].HasShip = true;
            }
            return true;
        }

        public bool ReceiveShot(int x, int y)
        {
            if (!IsValidPosition(x, y)) return false;

            // Проверка, не стреляли ли уже в эту клетку
            if (grid[x, y].State == CellState.Hit || grid[x, y].State == CellState.Miss)
                return false;

            if (grid[x, y].HasShip)
            {
                grid[x, y].State = CellState.Hit;
                return true;
            }
            else
            {
                grid[x, y].State = CellState.Miss;
                return false;
            }
        }

        public bool IsGameOver()
        {
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    if (grid[i, j].HasShip && grid[i, j].State != CellState.Hit)
                        return false;
                }
            }
            return true;
        }

        public int Size => SIZE;

        public Cell GetCell(int x, int y) => grid[x, y];

        public bool IsValidPosition(int x, int y) =>
            x >= 0 && x < SIZE && y >= 0 && y < SIZE;

        public bool CanPlaceShip(int x, int y, int size, bool horizontal)
        {
            if (horizontal ? (y + size > SIZE) : (x + size > SIZE))
                return false;

            for (int i = 0; i < size; i++)
            {
                int checkX = horizontal ? x : x + i;
                int checkY = horizontal ? y + i : y;

                if (!IsValidPosition(checkX, checkY) || grid[checkX, checkY].HasShip)
                    return false;
            }

            return IsAreaClear(x, y, size, horizontal);
        }

        public bool IsAreaClear(int x, int y, int size, bool horizontal)
        {
            int startX = Math.Max(0, x - 1);
            int endX = Math.Min(SIZE - 1, horizontal ? x + 1 : x + size);
            int startY = Math.Max(0, y - 1);
            int endY = Math.Min(SIZE - 1, horizontal ? y + size : y + 1);

            for (int i = startX; i <= endX; i++)
            {
                for (int j = startY; j <= endY; j++)
                {
                    if (grid[i, j].HasShip)
                        return false;
                }
            }
            return true;
        }
    }
}