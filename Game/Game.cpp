#include "Game.h"
#include "../Log/Log.h"


CNCGameField::CNCGameField(ushort __n)
{
    for(ushort i = 0; i < __n; i++)
    {
        field.emplace_back();

        for(ushort j = 0; j < __n; j++)
            field[i].push_back(FieldCell{i, j});
    }

    fieldDim = __n;
}

bool CNCGameField::takeCell(ushort __x, ushort __y)
{
    if(!isCellInField(__x, __y))
    {
        error = GameErrors::NoSuchCell;
        return false;
    }

    if(!isCellFree(__x, __y))
    {
        error = GameErrors::CellIsBusy;
        return false;
    }

    field[__x][__y].status = roolFlag ? CellStatus::Circle : CellStatus::Cross;

    roolFlag = !roolFlag;

    return true;
}

bool CNCGameField::isCellInField(ushort __x, ushort __y)
{
    if(__x > fieldDim or __y > fieldDim or __x == 0 or __y == 0)
        return false;
    else
        return true;
}

bool CNCGameField::isCellFree(ushort __x, ushort __y)
{
    return field[__x][__y].status == CellStatus::Empty;
}

void CNCGameField::cleanCells()
{
    for(auto& i : field)
        for(auto& j : i)
            j.status = CellStatus::Empty;

    roolFlag = false;
}

void CNCGameField::proccessError()
{
    switch (error)
    {
    case GameErrors::NoErrors:
        break;

    case GameErrors::CellIsBusy:
        LOG("Эта клетка занята!");
        break;

    case GameErrors::NoSuchCell:
        LOG("Нет такой клетки!");
        break;
    default:
        break;
    }

    error = GameErrors::NoErrors;
}


bool CNCGame::makeMove() 
{
    return true;
}


bool CNCGame::checkWin()
{
    return true;
}

void CNCGame::operator()()
{

}

void CNCGamedisplay()
{

}


void CNCGame::saveResults()
{

}