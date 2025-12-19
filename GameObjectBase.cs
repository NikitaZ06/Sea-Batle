using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System;

using System;

namespace Sea_battle
{
    // Базовый класс для всех игровых объектов
    public abstract class GameObjectBase : IGameObject, IShooter, IShipPlacer
    {
        protected string name;
        protected int score;
        protected GameBoard ownBoard;
        protected GameBoard enemyBoard;

        // Делегаты поведения
        protected IShootingBehavior shootingBehavior;
        protected IShipPlacementBehavior placementBehavior;

        // Реализация IGameObject
        public string Name => name;
        public int Score
        {
            get => score;
            set => score = value;
        }

        protected GameObjectBase(string objectName)
        {
            name = objectName;
            score = 0;
            ownBoard = new GameBoard();
            enemyBoard = null;
        }

        // Методы для установки поведения
        public void SetShootingBehavior(IShootingBehavior behavior)
        {
            shootingBehavior = behavior;
        }

        public IShootingBehavior GetShootingBehavior()
        {
            return shootingBehavior;
        }

        public void SetPlacementBehavior(IShipPlacementBehavior behavior)
        {
            placementBehavior = behavior;
        }

        public IShipPlacementBehavior GetPlacementBehavior()
        {
            return placementBehavior;
        }

        // Метод для увеличения очков
        protected void IncreaseScore(int points)
        {
            score += points;
        }

        // Абстрактные методы IGameObject
        public abstract void DisplayInfo();
        public abstract void PerformAction();
        public abstract void Update();

        // Реализация IGameObject.HasLost()
        public virtual bool HasLost()
        {
            return ownBoard.IsGameOver();
        }

        // Реализация IShooter.Shoot()
        public bool Shoot(int x, int y)
        {
            if (enemyBoard == null)
            {
                Console.WriteLine($"Ошибка: {name} не имеет поля противника для стрельбы");
                return false;
            }

            return Shoot(x, y, enemyBoard);
        }

        // Перегруженный метод для стрельбы по конкретной доске
        public bool Shoot(int x, int y, GameBoard targetBoard)
        {
            if (shootingBehavior == null)
            {
                Console.WriteLine($"Ошибка: у {name} не установлено поведение стрельбы");
                return false;
            }

            return shootingBehavior.Shoot(x, y, targetBoard);
        }

        // Реализация IShooter.GetShooterInfo()
        public string GetShooterInfo()
        {
            string behaviorInfo = shootingBehavior != null
                ? $" ({shootingBehavior.GetBehaviorInfo()})"
                : " (нет поведения)";
            return $"{name}{behaviorInfo}";
        }

        // Реализация IShipPlacer.SetupShips()
        public bool SetupShips()
        {
            if (placementBehavior == null)
            {
                Console.WriteLine($"Ошибка: у {name} не установлено поведение размещения кораблей");
                return false;
            }

            return placementBehavior.PlaceShips(ownBoard);
        }

        // Реализация IShipPlacer.GetOwnBoard()
        public GameBoard GetOwnBoard() => ownBoard;

        // Методы для управления досками
        public void SetEnemyBoard(GameBoard board)
        {
            enemyBoard = board;
        }

        public GameBoard GetEnemyBoard() => enemyBoard;

        // Свойства для удобства доступа
        public GameBoard OwnBoard => ownBoard;
        public GameBoard EnemyBoard => enemyBoard;
    }
}