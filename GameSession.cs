
using System;
using System;
using System.Collections.Generic;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sea_battle
{

    //Класс управления игровой сессией "Морской бой"

    public class GameSession
    {
        private GameBoard aiBoard;           // Поле AI
        private Player humanPlayer;          // Игрок-человек
        private AI computerAI;               // Искусственный интеллект
        private bool isPlayerTurn;           // Чей сейчас ход (true - игрок, false - AI)

        // Конструктор игровой сессии
        public GameSession(string playerName)
        {
            aiBoard = new GameBoard();                           // 1. Сначала создаем aiBoard
            humanPlayer = new Player(playerName, aiBoard);       // 2. Затем Player с ссылкой на aiBoard
            computerAI = new AI(aiBoard, humanPlayer.OwnBoard); // 3. AI получает aiBoard и поле игрока
            isPlayerTurn = true;
        }

        // Запуск игры - основной игровой цикл
        public void StartGame()
        {
            Console.WriteLine("=== МОРСКОЙ БОЙ ===");
            Console.WriteLine($"Добро пожаловать, {humanPlayer.Name}!");

            // 1. НАСТРОЙКА  
            SetupGame();
            Console.WriteLine("\nНачальная расстановка:");
            humanPlayer.DisplayBoards();
            Console.WriteLine();

            // Основной игровой цикл
            while (!IsGameOver())
            {
                if (isPlayerTurn)
                {
                    PlayerTurn();
                }
                else
                {
                    AITurn();

                    // ПОКАЗЫВАЕМ РЕЗУЛЬТАТЫ ПОСЛЕ ХОДА AI
                    Console.WriteLine("\n=== ТЕКУЩАЯ СИТУАЦИЯ ===");
                    humanPlayer.DisplayBoards();
                    Console.WriteLine();
                }

                isPlayerTurn = !isPlayerTurn; // Передача хода
            }

            // Финальное отображение
            Console.WriteLine("\n=== ФИНАЛЬНАЯ СИТУАЦИЯ ===");
            humanPlayer.DisplayBoards();

            Console.WriteLine("\n=== ИГРА ЗАВЕРШЕНА ===");
            string t;
            t=humanPlayer.HasLost() ? "ВЫ ПРОИГРАЛИ!" : "ВЫ ПОБЕДИЛИ!";
            Console.WriteLine(t);
            
        }

        // Настройка игры - расстановка кораблей
        private void SetupGame()
        {
            Console.WriteLine("\n--- РАССТАНОВКА КОРАБЛЕЙ ---");

            // ИГРОК расставляет
            ShipPlacer playerPlacer = new ShipPlacer(humanPlayer.OwnBoard);
            Console.WriteLine("Ваша расстановка:");
            playerPlacer.AutoPlaceShips();

            // КОМПЬЮТЕР расставляет
            Console.WriteLine("\nПротивник расставляет корабли...");
            ShipPlacer aiPlacer = new ShipPlacer(aiBoard);
            aiPlacer.AutoPlaceShips();

            Console.WriteLine("\nНАЧИНАЕМ ИГРУ!");
        }

        // Ход игрока
        private void PlayerTurn()
        {
            Console.WriteLine("--- ВАШ ХОД ---");

            int x = -1, y = -1;
            bool validInput = false;

            // Цикл для получения корректных координат
            while (!validInput)
            {
                Console.Write("Введите координаты (например A1): ");
                string input = Console.ReadLine();

                if (string.IsNullOrEmpty(input) || input.Length < 2)
                {
                    Console.WriteLine("Неверный формат!");
                    continue;
                }

                // Преобразуем буквенную координату в число
                y = char.ToUpper(input[0]) - 'A';

                // Преобразуем числовую координату
                if (int.TryParse(input.Substring(1), out int xCoord))//проверка координат 
                {
                    x = xCoord - 1;
                }
                else
                {
                    Console.WriteLine("Неверные координаты!");
                    continue;
                }

                // Проверка границ поля
                if (x < 0 || x >= 10 || y < 0 || y >= 10)
                {
                    Console.WriteLine("Координаты вне поля!");
                    continue;
                }

                validInput = true;
            }

            // Проверяем, не стреляли ли уже сюда
            CellState aiCellState = aiBoard.GetCell(x, y).State;

            if (aiCellState == CellState.Hit || aiCellState == CellState.Miss)
            {
                Console.WriteLine("Вы уже стреляли в эту клетку! Ход пропускается.");
            }
            else
            {
                // Делаем выстрел
                bool wasHit = aiBoard.ReceiveShot(x, y);

                // Обновляем поле противника у игрока
                if (wasHit)
                {
                    Console.WriteLine("ПОПАДАНИЕ! Вы попали в корабль противника!");
                    humanPlayer.EnemyBoard.GetCell(x, y).State=CellState.Hit;
                }
                else
                {
                    Console.WriteLine("ПРОМАХ!");
                    humanPlayer.EnemyBoard.GetCell(x, y).State = CellState.Miss;
                }
            }

            Console.Write("Нажмите Enter для передачи хода противнику...");
            Console.ReadLine();
        }

       
        // Ход компьютера (AI)
        
        private void AITurn()
        {
            Console.WriteLine("--- ХОД ПРОТИВНИКА ---");

            int x=0, y=0;
            bool validShot = false;
            Random random = new Random();

            // Поиск валидной клетки для выстрела
            while (!validShot)
            {
                x = random.Next(0, 10);
                y = random.Next(0, 10);

                // Проверяем, не стреляли ли уже сюда
                CellState state = humanPlayer.OwnBoard.GetCell(x, y).State;
                if (state != CellState.Hit && state != CellState.Miss)
                {
                    validShot = true;
                }
            }

            // Вычисляем координаты для отображения
            char column = (char)('A' + y);
            int row = x + 1;

            Console.Write($"Противник стреляет в {column}{row}... ");

            // AI стреляет по полю игрока
            bool wasHit = humanPlayer.OwnBoard.ReceiveShot(x, y);

            string t;
            t=wasHit ? "ПОПАДАНИЕ!" : "ПРОМАХ!";
            Console.WriteLine(t);

            Console.Write("Нажмите Enter...");
            Console.ReadLine();
        }

        // Проверка окончания игры
        private bool IsGameOver()
        {
            bool playerLost = humanPlayer.HasLost();//true если все корабли потоплены
            bool aiLost = computerAI.HasLost();
            return playerLost || aiLost;
        }

        // Синхронизация поля противника у игрока с актуальным состоянием
        private void SyncPlayerEnemyBoard()
        {
            // Копируем все состояния из aiBoard в enemyBoard игрока
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    CellState aiState = aiBoard.GetCell(i, j).State;
                    humanPlayer.EnemyBoard.GetCell(i, j).State = aiState;
                }
            }
        }

        // Отображение текущего состояния игры
        private void DisplayGameState()
        {
            humanPlayer.DisplayBoards();
        }
    }
}
