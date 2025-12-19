// See https://aka.ms/new-console-template for more information
using Sea_battle;
using System;
using System;
using System.Collections.Generic;
using System.Linq;

class Program
{
    static void Main()
    {
        Console.OutputEncoding = System.Text.Encoding.UTF8;

       

        Console.WriteLine("\n=== ЗАПУСК  ИГРЫ ===");

        string playerName;
        Console.Write("Введите ваше имя: ");
        playerName = Console.ReadLine();

        if (string.IsNullOrEmpty(playerName))
        {
            playerName = "Игрок";
        }

        // Запускаем игру
        GameSession game = new GameSession(playerName);

        game.StartGame();

        Console.WriteLine("\nСпасибо за игру!");
        Console.WriteLine("Нажмите любую клавишу для выхода...");
        Console.ReadKey();
    }
}