#include "Game.h"
#include "../Log/Log.h"
#include "../Network/Network.h"

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


CNCGameOnline::CNCGameOnline(ushort __fieldDim, PlayerSide __side, NetworkMember* __network)
{
    initField(__fieldDim);

    initEvals(__fieldDim);

    side = __side;

    fieldDim = __fieldDim;

    crossWinEval = -fieldDim;
    circleWinEval = fieldDim;

    moveCounter = fieldDim * fieldDim;

    network = __network;
}

void CNCGameOnline::initEvals(ushort __fieldDim)
{
    for(ushort i = 0; i < __fieldDim; i++)
    {
        evals.rowsEvals.push_back(0);
        
        evals.columnsEvals.push_back(0);
    }
}

void CNCGameOnline::initField(ushort __fieldDim)
{
    gameField.init(__fieldDim);
}


bool CNCGameOnline::makeMove(ushort __x, ushort __y) 
{
    auto whosMakeNow = roolFlag ? CellStatus::Circle : CellStatus::Cross;

    if(!gameField.takeCell(__x, __y, whosMakeNow))
        return false;

    gameProccessing(__x, __y);

    return true;
}

void CNCGameOnline::gameProccessing(ushort __x, ushort __y)
{
    if(roolFlag == (side == PlayerSide::Cross ? false : true))  
    {
        network->SendMessage(input);
    }

    addEval(__x, __y);

    roolFlag = !roolFlag;
    
    moveCounter--;
}

void CNCGameOnline::addEval(ushort __x, ushort __y)
{
    auto currentMoveEval = getEval();

    evals.rowsEvals[__x] += currentMoveEval;

    evals.columnsEvals[__y] += currentMoveEval;

    if(__x == __y)
        evals.mainDiagonalEval += currentMoveEval;

    if(__x == fieldDim - 1 - __y || __y == fieldDim - 1 - __x)
            evals.submainDiagonalEval += currentMoveEval;
}

short CNCGameOnline::getEval()
{
    return roolFlag ? circleEval : crossEval;
}


bool CNCGameOnline::getInput(ushort& __x, ushort& __y)
{
    if(roolFlag == (side == PlayerSide::Cross ? false : true))
    {
        CONSOLE('\n', "> ", "");
        std::cin >> input;
    }
    else
    {
        CONSOLE('\n', "Ожидаем ход соперника...", std::endl);
        input = network->RecievMessage();
    }

    return proccessGameInput(__x, __y);
}

bool CNCGameOnline::proccessGameInput(ushort& __x, ushort& __y)
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


void CNCGameOnline::operator()()
{
    ushort x, y;

    network->Connect();

    while(true)
    {
        display();

        if(status != GameStatus::GameContinues)
        {
            save();

            if(!restart())
                break;
            
            continue;
        }

        if(!getInput(x, y))
            continue;

        if(!makeMove(x, y))
            continue;

        status = checkWin();
    }
}

GameStatus CNCGameOnline::checkWin()
{
    if(evals.mainDiagonalEval == circleWinEval || evals.submainDiagonalEval == circleWinEval)
        return GameStatus::CircleWin;

    if(evals.mainDiagonalEval == crossWinEval || evals.submainDiagonalEval == crossWinEval)
        return GameStatus::CrossWin;


    for(auto i : evals.rowsEvals)
    {
        if(i == circleWinEval)
            return GameStatus::CircleWin;

        if(i == crossWinEval)
            return GameStatus::CrossWin;
    }

    for(auto j : evals.columnsEvals)
    {
        if(j == circleWinEval)
            return GameStatus::CircleWin;

        if(j == crossWinEval)
            return GameStatus::CrossWin;
    }

    
    if(moveCounter == 0)
        return GameStatus::Draw;
    else    
        return GameStatus::GameContinues;
}


void CNCGameOnline::display()
{
    CLEAR_ALL_TERMINAL();

    drawGame<std::ostream>(std::cout);

    drawGameStatus<std::ostream>(std::cout);

    logErrors();
}


template<typename T>
void CNCGameOnline::drawGame(T& __stream)
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

template<typename T>
void CNCGameOnline::drawTopNumbering(T& __stream)
{
    IN_STREAM_CONSOLE(__stream, "   ", "    ", "");

    for(short k = 0; k < fieldDim; k++)
    {
        IN_STREAM_CONSOLE(__stream, "", k, "   ");
    }

    IN_STREAM_CONSOLE(__stream, "\n", "", "\n");
}

template<typename T>
void CNCGameOnline::drawLeftSideNumbering(T& __stream)
{
    IN_STREAM_CONSOLE(__stream, " ", leftSideNumering++, "");      

    if(leftSideNumering == fieldDim)
        leftSideNumering = 0;
}

template<typename T>
void CNCGameOnline::drawGameStatus(T& __stream)
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

void CNCGameOnline::logErrors()
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

char CNCGameOnline::getSymbolByCell(CellStatus __status)
{
    switch (__status)
    {
    case CellStatus::Circle:
        return 'O';

    case CellStatus::Cross:
        return 'X';

    case CellStatus::Empty:
        return '-';
    }

    return '-';
}


void CNCGameOnline::save()
{
    CONSOLE('\n', "Сохранить результат? (y/n)", "");
    CONSOLE('\n', "> ", "");

    std::cin >> input;

    proccessSaveResultsInput();
}

void CNCGameOnline::proccessSaveResultsInput()
{
    if(input.size() != 1 || input != "y")
    {
        return;
    }
    
    saveResultsInFile();
}

void CNCGameOnline::saveResultsInFile()
{
    auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::ofstream saveFile("Игра от " + std::string(std::ctime(&currentTime)));

    IN_STREAM_CONSOLE(saveFile, "", "Игра от", " ");
    IN_STREAM_CONSOLE(saveFile, "", std::string(std::ctime(&currentTime)), "\n\n");

    drawGame<std::ofstream>(saveFile);

    drawGameStatus<std::ostream>(saveFile);
}


bool CNCGameOnline::restart()
{
    CONSOLE('\n', "Сыграть ещё? (y/n)", "");
    CONSOLE('\n', "> ", "");

    std::cin >> input;

    return proccessRestartInput();
}

bool CNCGameOnline::proccessRestartInput()
{
     if(input.size() != 1 || input != "y")
    {
        network->SendMessage("n");
        return false;
    }
    
    return newGame();
}

bool CNCGameOnline::newGame()
{
    network->SendMessage(input);

    CONSOLE('\n', "Ожидаем ответа...", std::endl);
    auto answer = network->RecievMessage();

    if(answer != "y")
        return false;

    cleanEvals();
    cleanField();

    status = GameStatus::GameContinues;

    return true;
}

void CNCGameOnline::cleanEvals()
{
    for(auto&i : evals.rowsEvals)
        i = 0;

    for(auto&j : evals.columnsEvals)
        j = 0;

    evals.mainDiagonalEval = 0;
    evals.submainDiagonalEval = 0;
}

void CNCGameOnline::cleanField()
{
    gameField.cleanCells();

    roolFlag = false;

    moveCounter = fieldDim * fieldDim;
}