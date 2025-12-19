using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System;
using System;
using System.Collections.Generic;

// AI.cs
using System;

namespace Sea_battle
{
    public class AI : GameObjectBase
    {
        private int difficultyLevel;

        public AI(string aiName, int difficulty = 1) : base(aiName)
        {
            difficultyLevel = difficulty;

            // Установка поведения в зависимости от сложности
            SetShootingBehavior(new AIShootingBehavior(aiName, difficulty));
            SetPlacementBehavior(new AutoPlacementBehavior());

        }

        // Реализация IGameObject.DisplayInfo()
        public override void DisplayInfo()
        {
            Console.WriteLine($"=== ИСКУССТВЕННЫЙ ИНТЕЛЛЕКТ ===");
            Console.WriteLine($"Имя: {name}");
            Console.WriteLine($"Очки: {score}");
            Console.WriteLine($"Уровень сложности: {difficultyLevel}");
            Console.WriteLine($"Статус: {(HasLost() ? "Уничтожен" : "Активен")}");
            Console.WriteLine($"Тип: AI");

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
            Console.WriteLine($"{name} анализирует игровую ситуацию...");

            switch (difficultyLevel)
            {
                case 1:
                    Console.WriteLine("  'Случайная атака...'");
                    break;
                case 2:
                    Console.WriteLine("  'Анализирую поле противника...'");
                    break;
                case 3:
                    Console.WriteLine("  'Вычисляю оптимальную стратегию...'");
                    break;
            }

            IncreaseScore(difficultyLevel);
        }

        // Реализация IGameObject.Update()
        public override void Update()
        {
            // AI может "обучаться" во время игры
            IncreaseScore(difficultyLevel * 2); // Чем сложнее AI, тем больше очков
        }

        // Метод для автономной игры (использует делегированное поведение)
        public bool MakeMove()
        {
            if (GetEnemyBoard() == null)
            {
                Console.WriteLine("Ошибка: у AI нет поля противника");
                return false;
            }

            Console.WriteLine($"{name} делает ход...");
            PerformAction();

            // Вызываем делегированное поведение
            var shootingBehavior = GetShootingBehavior();
            if (shootingBehavior != null)
            {
                return shootingBehavior.Shoot(-1, -1, GetEnemyBoard());
            }

            return false;
        }

        // Свойство только для чтения
        public int Difficulty => difficultyLevel;
    }
}