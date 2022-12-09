#include "Network/Network.h"
#include "Interface/Interface.h"
#include "Game/Game.h"
#include "Log/Log.h"


auto makeSessionFunction = []()
                                    {
                                        //Player-server:
                                        CNCGame serverGame(3);
                                        serverGame();
                                    };

auto joinSessionFunction = []()
                                    {
                                        //Player-client

                                        //Cient client
                                        //client.join()
                                        //client.play
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