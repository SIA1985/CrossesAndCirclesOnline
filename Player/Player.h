#pragma once

#include <memory>
#include <string>


class CNCGameOnline;
class Server;
class Client;


class Player
{
public:
    virtual void operator()() = 0;

protected:
    std::shared_ptr<CNCGameOnline> game;

    unsigned short gameSize = 3;
};

class PlayerServer : public Player
{
public:
    PlayerServer();

    void operator()() override;

protected:
    std::shared_ptr<Server> server;
};

class PlayerClient : public Player
{
public:
    PlayerClient();

    void operator()() override;

protected:
    std::shared_ptr<Client> client;
};