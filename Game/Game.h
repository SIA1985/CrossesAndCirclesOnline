#pragma once

#include <vector>
#include <string>


class NetworkMember;

typedef unsigned short ushort;

enum class FieldErrors : ushort
{
     NoErrors           = 0,
     NoSuchCell         = 1,
     CellIsBusy         = 2,

};

enum class GameErrors : ushort
{
     NoErrors           = 0,
     IncorrectInput     = 1

};

enum class CellStatus : ushort
{
    Empty               = 0,
    Cross               = 1,
    Circle              = 2
};

enum class GameStatus : ushort
{
    GameContinues       = 0,
    CrossWin            = 1,
    CircleWin           = 2,
    Draw                = 3
};

enum class PlayerSide : ushort
{
    Cross               = 0,
    Circle              = 1
};

struct FieldCell
{
    ushort x;
    ushort y;

    CellStatus status = CellStatus::Empty;
};

typedef std::vector<std::vector<FieldCell>> FieldCellsContainer;

class GameField
{
public:
    virtual bool takeCell(ushort __x, ushort __y, CellStatus __setStatus) = 0;

    virtual void cleanCells() = 0;
};

struct EvalMatrix
{
    std::vector<short> rowsEvals;
    std::vector<short> columnsEvals;
    short mainDiagonalEval = 0, submainDiagonalEval = 0;
};

class Game
{
public:
    virtual void operator()() = 0;

protected:
    virtual bool makeMove(ushort __x, ushort __y) = 0;

    virtual GameStatus checkWin() = 0;
};


class CNCGameField : public GameField
{
    friend class CNCGameOnline;

public:
    void init(ushort __n);

    bool takeCell(ushort __x, ushort __y, CellStatus __setStatus) override;

    void cleanCells() override;

protected:
    bool isCellInField(ushort __x, ushort __y);

    bool isCellFree(ushort __x, ushort __y);

    void logError(); 

private:
    FieldCellsContainer field;

    ushort fieldDim;

    FieldErrors error;
};

class CNCGameOnline : public Game
{
public:
    CNCGameOnline(ushort __fieldDim, PlayerSide __side, NetworkMember* __network);

    void operator()() override;

protected:
    void initEvals(ushort __fieldDim);

    void initField(ushort __fieldDim);


    bool getInput(ushort& __x, ushort& __y);

    bool proccessGameInput(ushort& __x, ushort& __y);

    void proccessSaveResultsInput();


    bool makeMove(ushort __x, ushort __y) override;

    void gameProccessing(ushort __x, ushort __y);

    void addEval(ushort __x, ushort __y);

    short getEval();

    GameStatus checkWin() override;


    template<typename T>
    void drawTopNumbering(T& __stream);

    template<typename T>
    void drawLeftSideNumbering(T& __stream);

    template<typename T>
    void drawGameStatus(T& __stream);

    template<typename T>
    void drawGame(T& __stream);

    void display();

    char getSymbolByCell(CellStatus __status);

    void logErrors();


    bool restart();

    bool proccessRestartInput();

    bool newGame();

    void cleanEvals();

    void cleanField();


    void saveResultsInFile();

public:
    void save();

private:
    PlayerSide side = PlayerSide::Cross;

    NetworkMember* network;
    bool Connected = false;

    bool roolFlag = false; //If false - take cross, true - circle
    bool gameOverFlag = false;

    int moveCounter;

    ushort fieldDim;

    CNCGameField gameField;

    EvalMatrix evals;
    short crossEval = -1;
    short circleEval = 1;

    short crossWinEval;
    short circleWinEval;

    std::string input;

    GameStatus status = GameStatus::GameContinues; 

    GameErrors error = GameErrors::NoErrors;

    short leftSideNumering = 0;
};