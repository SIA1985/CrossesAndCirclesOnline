#include "Network/Network.h"
#include "Interface/Interface.h"
#include "Log/Log.h"


int main()
{
    CNCInterface in;

    in.setHeadline("Проверка");

    in.addInterfaceItem('1', {"Вывод проверки", [](){CONSOLE('\n', "111", '\n');}});

    in();
}