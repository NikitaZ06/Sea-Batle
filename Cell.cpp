#include "Cell.h"

// Конструктор по умолчанию - инициализирует поля
Cell::Cell()
    : state(CellState::EMPTY)  // Инициализация state значением EMPTY
    , hasShip(false)           // Инициализация hasShip значением false
{
    // Тело конструктора пустое, т.к. вся инициализация выполнена в списке инициализации
}

// Возвращает текущее состояние ячейки
CellState Cell::getState() const {
    return state;  // Просто возвращаем значение поля state
}

// Возвращает true, если в ячейке есть корабль
bool Cell::getHasShip() const {
    return hasShip;  // Возвращаем значение поля hasShip
}

// Устанавливает новое состояние ячейки
void Cell::setState(CellState newState) {
    state = newState;  // Присваиваем полю state новое значение
}

// Устанавливает флаг наличия корабля в ячейке
void Cell::setHasShip(bool hasShip) {
    this->hasShip = hasShip;  // Устанавливаем новое значение (this-> для устранения неоднозначности)

    // Автоматически обновляем состояние в зависимости от наличия корабля
    if (hasShip) {
        state = CellState::SHIP;  // Если есть корабль - состояние "корабль"
    }
    else {
        state = CellState::EMPTY; // Если корабль убран - состояние "пусто"
    }
}
