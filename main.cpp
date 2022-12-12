#include "Network/Network.h"
#include "Interface/Interface.h"
#include "Game/Game.h"
#include "Player/Player.h"
#include "Log/Log.h"


auto makeSessionFunction = []()
                                    {
                                        PlayerServer player;

                                        player();
                                    };

auto joinSessionFunction = []()
                                    {
                                        PlayerClient player;

                                        player();
                                    };

auto exitGameFunction = []()
                                    {
                                        SUCCESS("Всего хорошего!");
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