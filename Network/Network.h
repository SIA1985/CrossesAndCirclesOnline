#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>


enum class NetworkErrors : unsigned short
{
    NoErrors                = 0,   
    CantCreateSocket        = 1,
    CantConnect             = 2,
    CantBindSocket          = 3,
    CantAccept              = 4
};

class NetworkMemberBase
{
public:
    virtual void Connect() = 0;

    virtual void Reconnect() = 0;

    virtual void SendMessage() = 0;

    virtual void RecievMessage() = 0;
};

class NetworkMember : public NetworkMemberBase
{
public:
    virtual void Connect() = 0;

    virtual void Reconnect() = 0;

    virtual void SendMessage() = 0;

    virtual void RecievMessage() = 0;

protected:
    void SendMessagePrototype(int __socketDescriptor);

    void RecievMessagePrototype(int __socketDescriptor);

    void proccessError();

    std::string buffer;
    char* readWriteBuffer = new char[4];

    NetworkErrors error = NetworkErrors::NoErrors;
};


class Client : public NetworkMember
{
public:
    Client();

    bool TryToConnect(std::string __serverIPAddress, uint16_t __sereverSocket);

    void Connect() override;

    void Reconnect() override;

    void SendMessage() override;

    void RecievMessage() override;

protected:
    void getUserInput(std::string& __serverIPAddress, uint16_t& __sereverSocket);

private:
    int serverSocketDescriptor;
    struct sockaddr_in serverSocketParams;
};

class Server : public NetworkMember
{
public:
    Server();

    bool TryToConnect(uint16_t __socket);
    
    void Connect() override;

    void Reconnect() override {};

    void SendMessage() override;

    void RecievMessage() override;

protected:
    void getUserInput(uint16_t& __socket);

private:
    int socketDescriptor;
    struct sockaddr_in socketParams;

    int clientSocketDescriptor = -1;
};
