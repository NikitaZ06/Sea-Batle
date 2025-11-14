using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public enum CellState
{
    Empty,
    Ship,
    Hit,
    Miss
}

public class Cell
{
    private CellState state;    // Текущее состояние ячейки
    private bool hasShip;       // Флаг наличия корабля

    // Конструктор
    public Cell()
    {
        state = CellState.Empty;  // Инициализация state значением Empty
        hasShip = false;          // Инициализация hasShip значением false
    }

    // Возвращает текущее состояние ячейки
    public CellState GetState()
    {
        return state;  // Просто возвращаем значение поля state
    }

    // Возвращает true, если в ячейке есть корабль
    public bool GetHasShip()
    {
        return hasShip;  // Возвращаем значение поля hasShip
    }

    // Устанавливает новое состояние ячейки
    public void SetState(CellState newState)
    {
        state = newState;  // Присваиваем полю state новое значение
    }

    // Устанавливает флаг наличия корабля в ячейке
    public void SetHasShip(bool hasShipValue)
    {
        hasShip = hasShipValue;  // Устанавливаем новое значение

        // Если в ячейке теперь есть корабль,
        state=hasShip ? CellState.Ship : CellState.Empty;
    }
}
