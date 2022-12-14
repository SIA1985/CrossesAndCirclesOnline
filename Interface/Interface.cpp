#include "Interface.h"
#include "../Log/Log.h"


void CNCInterface::setHeadline(const char* __headline)
{
    headline = __headline;
}

void CNCInterface::addInterfaceItem(Key __key, InterfaceFunction __function) 
{
    keysAndFunctions.emplace(__key, __function);
}

void CNCInterface::operator()()
{
    SAVE_CURSOR_POSITION();

    while(true)
    {
        draw();

        getInput();
    }
}
    
void CNCInterface::getInput() 
{
    CONSOLE("", "\n> ", "");
    std::cin >> inputBuffer;
    
    processInput();
}


void CNCInterface::processInput()
{
    if(inputBuffer.size() != 1)
    {
        error = InterfaceErrors::IncorrectInput;

        inputBuffer.clear();

        return;
    }

    processKey();
}

void CNCInterface::processKey()
{
    Key inputKey = *inputBuffer.begin();

    auto mapIterator = keysAndFunctions.find(inputKey);

    if(mapIterator == keysAndFunctions.end())
    {
        
        error = InterfaceErrors::NoSuchKey;

        inputBuffer.clear();

        return;
    }

    CLEAR_ALL_TERMINAL();

    auto function = (*mapIterator).second;
    function();
}

void CNCInterface::logError()
{
    switch (error)
    {
    case InterfaceErrors::NoErrors:
        break;
    
    case InterfaceErrors::IncorrectInput:

        LOG("Неверный ввод!");
        break;

    case InterfaceErrors::NoSuchKey:

        LOG("Такого ключа - нет!");
        break;
    default:
        break;
    }

    error = InterfaceErrors::NoErrors;
}

void CNCInterface::draw()
{
    CLEAR_ALL_TERMINAL();

    CONSOLE('\n', headline, '\n');

    for(auto& keyAndFunctonPair : keysAndFunctions)
    {
        auto key = keyAndFunctonPair.first;
        auto functionDescription = keyAndFunctonPair.second.description;

        CONSOLE('\n',functionDescription, ' ');
        CONSOLE('(', key, ')');

    }

    logError();
}