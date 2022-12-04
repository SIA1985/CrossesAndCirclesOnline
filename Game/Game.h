#pragma once

#include <vector>


typedef unsigned short ushort;

enum class GameErrors : ushort
{
     NoErrors           = 0,
     NoSuchCell         = 1,
     CellIsBusy         = 2

};

enum class CellStatus : ushort
{
    Empty               = 0,
    Cross               = 1,
    Circle              = 2
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
    virtual bool takeCell(ushort __x, ushort __y) = 0;

    virtual void cleanCells() = 0;
};

class Game
{
public:
    virtual bool makeMove() = 0;

protected:
    virtual bool checkWin() = 0;

    virtual void operator()() = 0;
};


class CNCGameField : public GameField
{
    friend class CNCGame;

public:
    CNCGameField(ushort __n);

    bool takeCell(ushort __x, ushort __y) override;

    void cleanCells() override;

protected:
    bool isCellInField(ushort __x, ushort __y);

    bool isCellFree(ushort __x, ushort __y);

    void proccessError(); 

private:
    FieldCellsContainer field;

    ushort fieldDim;

    GameErrors error;

    bool roolFlag = false; //If false - take cross, true - circle
};

class CNCGame : public Game
{
public:
    bool makeMove() override;

protected:
    bool checkWin() override;

    void operator()() override;

    void display();

public:
    void saveResults();

private:
    CNCGameField gameField;
    //Network - ?
};