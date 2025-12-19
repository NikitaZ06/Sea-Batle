using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sea_battle
{
    // Стратегия стрельбы (делегирование поведения)
    public interface IShootingBehavior
    {
        bool Shoot(int x, int y, GameBoard targetBoard);
        string GetBehaviorInfo();
    }


    // Реализация для человеческого игрока

    public class HumanShootingBehavior : IShootingBehavior
    {
        private string playerName;

        public HumanShootingBehavior(string name)
        {
            playerName = name;
        }

        public bool Shoot(int x, int y, GameBoard targetBoard)
        {
            
            // Логика прицеливания человека
            return targetBoard.ReceiveShot(x, y);
        }

        public string GetBehaviorInfo()
        {
            return $"Стрельба человека ({playerName})";
        }
    }


    // Реализация для AI
 
    public class AIShootingBehavior : IShootingBehavior
    {
        private string aiName;
        private int difficulty;
        private Random random;
        private List<(int, int)> shotHistory;

        public AIShootingBehavior(string name, int difficultyLevel = 1)
        {
            aiName = name;
            difficulty = difficultyLevel;
            random = new Random();
            shotHistory = new List<(int, int)>();
        }

        public bool Shoot(int x, int y, GameBoard targetBoard)
        {
            // В зависимости от сложности выбираем координаты
            (int targetX, int targetY) = difficulty switch
            {
                1 => GetRandomShot(),
                2 => GetSmartShot(),
                3 => GetStrategicShot(),
                _ => GetRandomShot()
            };

           
            shotHistory.Add((targetX, targetY));

            return targetBoard.ReceiveShot(targetX, targetY);
        }

        private (int, int) GetRandomShot()
        {
            return (random.Next(0, 10), random.Next(0, 10));
        }

        private (int, int) GetSmartShot()
        {
            // "Умная" логика стрельбы
            // Можно добавить анализ предыдущих выстрелов
            return GetRandomShot();
        }

        private (int, int) GetStrategicShot()
        {
            // Стратегическая логика
            // Поиск паттернов, анализ поля
            return GetRandomShot();
        }

        public string GetBehaviorInfo()
        {
            return $"Стрельба AI ({aiName}, уровень {difficulty})";
        }
    }
}