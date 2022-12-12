#include "Player.h"
#include "../Game/Game.h"
#include "../Network/Network.h"


PlayerServer::PlayerServer()
{
    server = std::make_shared<Server>();
    game = std::make_shared<CNCGameOnline>(gameSize, PlayerSide::Cross, server.get());
}

void PlayerServer::operator()()
{
    game->operator()();
}


PlayerClient::PlayerClient()
{
    client = std::make_shared<Client>();
    game = std::make_unique<CNCGameOnline>(gameSize, PlayerSide::Circle, client.get());
}

void PlayerClient::operator()()
{
    game->operator()();
}