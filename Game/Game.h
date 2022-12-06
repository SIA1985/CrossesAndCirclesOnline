#pragma once

#include <vector>
#include <string>


typedef unsigned short ushort;

enum class FieldErrors : ushort
{
     NoErrors           = 0,
     NoSuchCell         = 1,
     CellIsBusy         = 2,
     IncorrectInput     = 3

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

    virtual void cleanField() = 0;
};


class CNCGameField : public GameField
{
    friend class CNCGame;

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

class CNCGame : public Game
{
public:
    CNCGame(ushort __fieldDim);

    void operator()() override;

protected:
    void initEvals(ushort __fieldDim);

    void initField(ushort __fieldDim);

    bool getInput(ushort& __x, ushort& __y);

    bool proccessInput(ushort& __x, ushort& __y);

    bool makeMove(ushort __x, ushort __y) override;

    void addEval(ushort __x, ushort __y);

    short getEval();

    GameStatus checkWin() override;

    void display();

    char getSymbolByCell(CellStatus __status);

    void logErrors();

    void cleanEvals();

public:
    void saveResults();

    void cleanField() override;

private:
    bool roolFlag = false; //If false - take cross, true - circle
    int moveCounter;

    ushort fieldDim;

    CNCGameField gameField;
    //Network - ?

    EvalMatrix evals;
    short crossEval = -1;
    short circleEval = 1;
    short crossWinScore;
    short circleWinScore;

    std::string input;

    GameErrors error = GameErrors::NoErrors;
};


// enum class Player : ushort
// {
//     _1              = 1,
//     _2              = 2
// };

// Player WhoIsTheFirst();