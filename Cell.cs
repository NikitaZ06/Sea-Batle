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
    private CellState state;

    // Свойство состояния
    public CellState State
    {
        get => state;
        set => state = value;
    }

    // Вычисляемое свойство наличия корабля
    public bool HasShip
    {
        get => state == CellState.Ship;
        set => state = value ? CellState.Ship : CellState.Empty;
    }
}
