// See https://aka.ms/new-console-template for more information
using System;

namespace Battleship
{
    // Главный класс программы - точка входа в приложение

    class Program
    {
        // Главный метод - точка входа в программу
        static void Main()
        {
            // Устанавливаем кодировку для поддержки русского языка
            Console.OutputEncoding = System.Text.Encoding.UTF8;

            Console.WriteLine("=== МОРСКОЙ БОЙ ===");

            string playerName;
            Console.Write("Введите ваше имя: ");
            playerName = Console.ReadLine();

            // Проверка на пустое имя
            if (string.IsNullOrEmpty(playerName))
            {
                playerName = "Игрок";
            }

            // Создаем и запускаем игру
            GameSession game = new GameSession(playerName);
            game.StartGame();

            Console.WriteLine("\nСпасибо за игру!");

            // Ожидаем нажатия любой клавиши перед выходом
            Console.WriteLine("Нажмите любую клавишу для выхода...");
            Console.ReadKey();
        }
    }
}