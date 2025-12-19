using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System;

namespace Sea_battle
{
    // Базовый интерфейс для всех игровых объектов
    public interface IGameObject
    {
        // Основные свойства
        string Name { get; }
        int Score { get; }

        // Основные методы
        void DisplayInfo();
        void PerformAction();
        void Update();

        // Методы для игровой логики
        bool HasLost();
    }

    // Интерфейс для объектов, которые могут стрелять
    public interface IShooter : IGameObject
    {
        bool Shoot(int x, int y);
        string GetShooterInfo();
    }

    // Интерфейс для объектов, которые могут размещать корабли
    public interface IShipPlacer : IGameObject
    {
        bool SetupShips();
        GameBoard GetOwnBoard();
    }
}