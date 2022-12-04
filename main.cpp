#include "Network/Network.h"
#include "Interface/Interface.h"
#include "Game/Game.h"
#include "Log/Log.h"



auto makeSessionFunction = []()
                                    {
                                        CONSOLE('\n', "1й", '\n');
                                        sleep(5);
                                    };

auto joinSessionFunction = []()
                                    {
                                        CONSOLE('\n', "2й", '\n');
                                        sleep(5);
                                    };

auto exitGameFunction = []()
                                    {
                                        SUCCESS("Всего хорошего!\n");
                                        exit(0);
                                    };

CNCInterface interface;


int main()
{
    interface.setHeadline("Крестики-нолики online");

    interface.addInterfaceItem('1', {"Создать сессию", makeSessionFunction});
    
    interface.addInterfaceItem('2', {"Присоединиться к сессии", joinSessionFunction});

    interface.addInterfaceItem('3', {"Выйти из игры", exitGameFunction});

    interface();
}