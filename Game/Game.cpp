#include "Game.h"
#include "../Log/Log.h"


void CNCGameField::init(ushort __n)
{
    for(ushort i = 0; i < __n; i++)
    {
        field.emplace_back();

        for(ushort j = 0; j < __n; j++)
            field[i].push_back(FieldCell{i, j});
    }

    fieldDim = __n;
}

bool CNCGameField::takeCell(ushort __x, ushort __y, CellStatus __setStatus)
{
    if(!isCellInField(__x, __y))
    {
        error = FieldErrors::NoSuchCell;
        return false;
    }

    if(!isCellFree(__x, __y))
    {
        error = FieldErrors::CellIsBusy;
        return false;
    }

    field[__x][__y].status = __setStatus;

    return true;
}

bool CNCGameField::isCellInField(ushort __x, ushort __y)
{
    if(__x >= fieldDim or __y >= fieldDim)
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
}

void CNCGameField::logError()
{
    switch (error)
    {
    case FieldErrors::NoErrors:
        break;

    case FieldErrors::CellIsBusy:
        LOG("Эта клетка занята!");
        break;

    case FieldErrors::NoSuchCell:
        LOG("Нет такой клетки!");
        break;
    default:
        break;
    }

    error = FieldErrors::NoErrors;
}


CNCGame::CNCGame(ushort __fieldDim)
{
    initField(__fieldDim);

    initEvals(__fieldDim);

    fieldDim = __fieldDim;

    crossWinScore = -fieldDim;
    crossWinScore = fieldDim;

    moveCounter = fieldDim * fieldDim;
}

bool CNCGame::makeMove(ushort __x, ushort __y) 
{
    auto whosMakeNow = roolFlag ? CellStatus::Circle : CellStatus::Cross;

    if(!gameField.takeCell(__x, __y, whosMakeNow))
        return false;

    addEval(__x, __y);

    roolFlag = !roolFlag;
    
    moveCounter--;

    return true;
}

void CNCGame::initEvals(ushort __fieldDim)
{
    for(ushort i = 0; i < __fieldDim; i++)
    {
        evals.rowsEvals.push_back(0);
        
        evals.columnsEvals.push_back(0);
    }
}

void CNCGame::initField(ushort __fieldDim)
{
    gameField.init(__fieldDim);
}

bool CNCGame::getInput(ushort& __x, ushort& __y)
{
    CONSOLE("", "\n> ", "");
    std::cin >> input;

    return proccessInput(__x, __y);
}

bool CNCGame::proccessInput(ushort& __x, ushort& __y)
{
    if(input.size() != 2 || int(input[0]) < 48 || int(input[1]) < 48 
                         || int(input[0]) > 57 || int(input[1]) > 57)
    {
        error = GameErrors::IncorrectInput;
        return false;
    }

    __x = short(input[0]) - 48;
    __y = short(input[1]) - 48;

    return true;
}

short CNCGame::getEval()
{
    return roolFlag ? circleEval : crossEval;
}

void CNCGame::addEval(ushort __x, ushort __y)
{
    auto currentMoveEval = getEval();

    evals.rowsEvals[__y] += currentMoveEval;

    evals.columnsEvals[__x] += currentMoveEval;

    if(__x == __y)
        evals.mainDiagonalEval += currentMoveEval;

    if(__x == fieldDim - __y or __y == fieldDim - __x or __x == __y)
            evals.submainDiagonalEval += currentMoveEval;
}

GameStatus CNCGame::checkWin()
{
    if(evals.mainDiagonalEval == circleWinScore or evals.submainDiagonalEval == circleWinScore)
        return GameStatus::CircleWin;

    if(evals.mainDiagonalEval == crossWinScore or evals.submainDiagonalEval == crossWinScore)
        return GameStatus::CrossWin;


    for(auto& i : evals.rowsEvals)
    {
        if(i == circleWinScore)
            return GameStatus::CircleWin;

        if(i == crossWinScore)
            return GameStatus::CrossWin;
    }

    for(auto& j : evals.columnsEvals)
    {
        if(j == circleWinScore)
            return GameStatus::CircleWin;

        if(j == crossWinScore)
            return GameStatus::CrossWin;
    }

    
    if(moveCounter == 0)
        return GameStatus::Draw;
    else    
        return GameStatus::GameContinues;
}

void CNCGame::operator()()
{
    ushort x, y;

    while(true)
    {
        display();

        if(!getInput(x, y))
            continue;

        if(!makeMove(x, y))
            continue;


        //checkWin
    }
}

void CNCGame::display(/*streamToDraw*/)
{
    CLEAR_ALL_TERMINAL();

    for(auto& i : gameField.field)
    {
        CONSOLE("   ", '|', "");

        for(auto& j : i)
        {
            CONSOLE(' ', getSymbolByCell(j.status), " |");
        }

        CONSOLE('\n', "", "";)
    }

    logErrors();
}

char CNCGame::getSymbolByCell(CellStatus __status)
{
    switch (__status)
    {
    case CellStatus::Circle:
        return 'O';

    case CellStatus::Cross:
        return 'X';

    case CellStatus::Empty:
        break;
    }

    return '-';
}


void CNCGame::saveResults()
{
    
}

void CNCGame::logErrors()
{
    switch (error)
    {
    case GameErrors::IncorrectInput:
        LOG("Неккоректный ввод!");
        error = GameErrors::NoErrors;
        break;
    case GameErrors::NoErrors:
        break;
    }

    gameField.logError();
}

void CNCGame::cleanEvals()
{
    for(auto&i : evals.rowsEvals)
        i = 0;

    for(auto&j : evals.columnsEvals)
        j = 0;

    evals.mainDiagonalEval = 0;
    evals.submainDiagonalEval = 0;
}

void CNCGame::cleanField()
{
    gameField.cleanCells();

    cleanEvals();

    roolFlag = false;

    moveCounter = fieldDim * fieldDim;
}