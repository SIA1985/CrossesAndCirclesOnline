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
    INPUT(inputBuffer);
    
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

    CLEAR_ALL();

    auto function = (*mapIterator).second;
    function();
}

void CNCInterface::proccesError()
{
    switch (error)
    {
    case InterfaceErrors::NoErrors:
        break;
    
    case InterfaceErrors::IncorrectInput:

        LOG("Incorrect input!");

        error = InterfaceErrors::NoErrors;
        break;
    case InterfaceErrors::NoSuchKey:

        LOG("No such key!");

        error = InterfaceErrors::NoErrors;
        break;
    default:
        break;
    }
}

void CNCInterface::draw()
{
    CLEAR_ALL();

    CONSOLE('\n', headline, '\n');

    for(auto& keyAndFunctonPair : keysAndFunctions)
    {
        auto key = keyAndFunctonPair.first;
        auto functionDescription = keyAndFunctonPair.second.description;

        CONSOLE('\n',functionDescription, ' ');
        CONSOLE('(', key, ')');

    }

    proccesError();
}