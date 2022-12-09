#include "Game.h"
#include "../Log/Log.h"

#include <fstream>
#include <chrono>


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
    circleWinScore = fieldDim;

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
    CONSOLE('\n', "> ", "");
    std::cin >> input;

    return proccessGameInput(__x, __y);
}

bool CNCGame::proccessGameInput(ushort& __x, ushort& __y)
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

bool CNCGame::proccessSaveResultsInput()
{
    if(input.size() != 1 || (input != "y" && input != "n"))
    {
        error = GameErrors::IncorrectInput;
        return false;
    }
    
    if(input == "y")
    {
        saveResultsInFile();
        return true;
    }

    return true;
    
}

short CNCGame::getEval()
{
    return roolFlag ? circleEval : crossEval;
}

void CNCGame::addEval(ushort __x, ushort __y)
{
    auto currentMoveEval = getEval();

    evals.rowsEvals[__x] += currentMoveEval;

    evals.columnsEvals[__y] += currentMoveEval;

    if(__x == __y)
        evals.mainDiagonalEval += currentMoveEval;

    if(__x == fieldDim - 1 - __y || __y == fieldDim - 1 - __x)
            evals.submainDiagonalEval += currentMoveEval;
}

GameStatus CNCGame::checkWin()
{
    if(evals.mainDiagonalEval == circleWinScore || evals.submainDiagonalEval == circleWinScore)
        return GameStatus::CircleWin;

    if(evals.mainDiagonalEval == crossWinScore || evals.submainDiagonalEval == crossWinScore)
        return GameStatus::CrossWin;


    for(auto i : evals.rowsEvals)
    {
        if(i == circleWinScore)
            return GameStatus::CircleWin;

        if(i == crossWinScore)
            return GameStatus::CrossWin;
    }

    for(auto j : evals.columnsEvals)
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

void CNCGame::operator()() //bool if gameSaved?
{
    ushort x, y;

    while(true)
    {
        //while(notConneted){};

        display();

        if(status != GameStatus::GameContinues)
        {
            if(save())
                break;
            
            continue;
        }

        if(!getInput(x, y))
            continue;

        //if smone wins - stop making moves
        if(!makeMove(x, y))
            continue;

        status = checkWin();

    }
}

void CNCGame::restartGame()
{
    cleanEvals();
    cleanField();

    status = GameStatus::GameContinues;
}

template<typename T>
void CNCGame::drawTopNumbering(T& __stream)
{
    IN_STREAM_CONSOLE(__stream, "   ", "    ", "");

    for(short k = 0; k < fieldDim; k++)
    {
        IN_STREAM_CONSOLE(__stream, "", k, "   ");
    }

    IN_STREAM_CONSOLE(__stream, "\n", "", "\n");
}

template<typename T>
void CNCGame::drawLeftSideNumbering(T& __stream)
{
    IN_STREAM_CONSOLE(__stream, " ", leftSideNumering++, "");      

    if(leftSideNumering == fieldDim)
        leftSideNumering = 0;
}

template<typename T>
void CNCGame::drawGameStatus(T& __stream)
{
    switch (status)
    {
    case GameStatus::GameContinues:
        return;

    case GameStatus::CrossWin:
        IN_STREAM_CONSOLE(__stream, '\n', "Крестики выиграли!", '\n');
        break;

    case GameStatus::CircleWin:
        IN_STREAM_CONSOLE(__stream, '\n', "Нолики выиграли!", '\n');
        break;

    case GameStatus::Draw:
        IN_STREAM_CONSOLE(__stream, '\n', "Ничья!", '\n');
        break;
    }
}

template<typename T>
void CNCGame::drawGame(T& __stream)
{
    drawTopNumbering<T>(__stream); 

    for(auto& i : gameField.field)  
    {
        drawLeftSideNumbering<T>(__stream);

        IN_STREAM_CONSOLE(__stream, "   ", '|', "");

        for(auto& j : i)
        {

            IN_STREAM_CONSOLE(__stream, " ", getSymbolByCell(j.status), " |");
        }

        IN_STREAM_CONSOLE(__stream, '\n', "", "");
    }   
}

void CNCGame::display()
{
    CLEAR_ALL_TERMINAL();

    drawGame<std::ostream>(std::cout);

    drawGameStatus<std::ostream>(std::cout);

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


bool CNCGame::save()
{
    CONSOLE('\n', "Сохранить результат? (y/n)", "");
    CONSOLE('\n', "> ", "");

    std::cin >> input;

    return proccessSaveResultsInput();
}

void CNCGame::saveResultsInFile()
{
    auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::ofstream saveFile("Игра от " + std::string(std::ctime(&currentTime)));

    IN_STREAM_CONSOLE(saveFile, "", "Игра от", " ");
    IN_STREAM_CONSOLE(saveFile, "", std::string(std::ctime(&currentTime)), "\n\n");

    drawGame<std::ofstream>(saveFile);

    drawGameStatus<std::ostream>(saveFile);
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