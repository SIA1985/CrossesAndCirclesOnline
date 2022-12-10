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

                                        int socket;
                                        std::string ip;
                                        std::cin >> socket >> ip;
                                        Client client(ip.c_str(), socket);
                                        client.Connect();
                                    };

auto joinSessionFunction = []()
                                    {
                                        //Player-client

                                        //Cient client
                                        //client.join()
                                        //client.play
                                        int socket;
                                        std::string ip;
                                        std::cin >> socket >> ip;
                                        Server server(ip.c_str(), socket);
                                        server.Connect();
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