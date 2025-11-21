using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System;
namespace Sea_battle
{
    public class AI
    {
        private GameBoard ownBoard;      // Собственное поле AI (корабли AI)
        private GameBoard enemyBoard;    // Поле игрока (куда AI стреляет)
        private Random random;           // Генератор случайных чисел

        // Конструктор
        public AI(GameBoard aiBoard, GameBoard playerBoard)
        {
            ownBoard = aiBoard;
            enemyBoard = playerBoard;
            random = new Random();  // Инициализация генератора случайных чисел
        }

        // ХОД AI - СЛУЧАЙНАЯ СТРЕЛЬБА
        public void MakeMove()
        {
            Console.Write("Противник стреляет... ");

            int x=0;
            int y=0;
            bool validShot = false;

            // Ищем случайную свободную клетку
            while (!validShot)
            {
                x = random.Next(0, 10);
                y = random.Next(0, 10);

                // Проверяем, не стреляли ли уже сюда
                // CellState state = enemyBoard.GetCell(x, y).GetState();
                CellState state = enemyBoard.GetCell(x, y).State;
                if (state != CellState.Hit && state != CellState.Miss)
                {
                    validShot = true;
                }
            }

            // Совершаем выстрел
            Console.Write($"в {(char)('A' + y)}{(x + 1)}... ");

            bool wasHit = enemyBoard.ReceiveShot(x, y);
            string result = wasHit ? "ПОПАДАНИЕ!" : "ПРОМАХ!";
            Console.WriteLine(result);
        }

        // Расстановка кораблей - используем  ShipPlacer
        public bool SetupShips()
        {
            ShipPlacer placer = new ShipPlacer(ownBoard);
            bool exit;
            exit=placer.AutoPlaceShips() == true?true:false;
            return exit;
        }

        // Проверка, проиграл ли AI
        public bool HasLost()
        {
            return ownBoard.IsGameOver();  // Проверяем собственное поле!
        }
    }
}
