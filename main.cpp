#include "Network/Network.h"
#include "Interface/Interface.h"
#include "Game/Game.h"
#include "Log/Log.h"

#include <string>

auto makeSessionFunction = []()
                                    {
                                        /*
                                        Player-Server() :
                                        while(true)
                                        {
                                            Game();

                                            Restart?
                                                Game.restart()
                                            break;
                                        }*/
                                        //Player-server:
                                        // CNCGame serverGame(5); // спросить о размерности поля
                                        // serverGame();

                                        Server server;

                                        server.Connect();

                                        std::cout << " \nПодключено!\n";

                                        sleep(5);   
                                    };

auto joinSessionFunction = []()
                                    {
                                        //Player-client

                                        //Cient client
                                        //client.join()
                                        //client.play()

                                        Client client;

                                        client.Connect();

                                        std::cout << "\nПодключено!\n";

                                        sleep(5);
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