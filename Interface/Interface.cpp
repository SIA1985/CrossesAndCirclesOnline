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
    while(true)
    {
        draw();

        getInput();
    }
}
    
void CNCInterface::getInput() 
{
    std::cin >> inputBuffer;
    
    processInput();
}


void CNCInterface::processInput()
{
    if(inputBuffer.size() != 1)
    {
        LOG("Inccorect input");
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
        LOG("No such key");
        inputBuffer.clear();
    }

    auto function = (*mapIterator).second;
    function();
}

void CNCInterface::draw()
{
    CONSOLE('\n', headline, '\n');

    for(auto& keyAndFunctonPair : keysAndFunctions)
    {
        auto key = keyAndFunctonPair.first;
        auto functionDescription = keyAndFunctonPair.second.description;

        CONSOLE('\n',functionDescription, ' ');
        CONSOLE('(', key, ')');

    }
}