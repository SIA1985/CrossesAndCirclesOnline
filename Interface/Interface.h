#pragma once

#include <functional>
#include <map>
#include <string>


enum class InterfaceErrors : unsigned short
{
    NoErrors        = 0,
    IncorrectInput  = 1,
    NoSuchKey       = 2,
    InvalidFunction = 3
};


typedef char Key;

struct InterfaceFunction
{
    void operator()()
    {
        function();
    }

    std::string description;
    std::function<void ()> function;
};


class Interface
{
public:
    virtual void setHeadline(const char* __headline) = 0;

    virtual void addInterfaceItem(Key __key, InterfaceFunction __function) = 0;

    virtual void operator()() = 0;
    
    virtual void getInput() = 0;

protected:
    virtual void processInput() = 0;

public:
    virtual void draw() = 0;
};


class CNCInterface : public Interface
{
public:
    void setHeadline(const char* __headline) override;

    void addInterfaceItem(Key __key, InterfaceFunction __function) override;

    void operator()() override;
    
    void getInput() override;

protected:
    void processInput() override;

    void processKey();

    void logError();

    void draw() override;

private:
    std::string headline;
    std::map<Key, InterfaceFunction> keysAndFunctions;

    std::string inputBuffer;

    InterfaceErrors error = InterfaceErrors::NoErrors;
};