#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>


class NetworkMember
{
public:
    NetworkMember(const char* __ipAdress, uint16_t __socket);

    virtual ~NetworkMember() {close(socketDescriptor); delete recvBuffer; }     


    virtual int TryToConnect() = 0;

    virtual void Reconnect();

    virtual void Connect() = 0;

    virtual void SendMessage() = 0;

    virtual void RecievMessage() = 0;

    std::string GetBuffer() { return buffer; }

    void ClearBuffer() { buffer.clear(); }

    bool IsConnected() { return Connected; }

protected:
    bool Connected = false; 
    int socketDescriptor = -1;
    struct sockaddr_in socketParams;

    std::string buffer;
    char* recvBuffer = new char[2];

    void SendMessagePrototype(int __socketDescriptor);

    void RecievMessagePrototype(int __socketDescriptor);
};

class Client : public NetworkMember
{
public:
    Client(const char* __ipAdress, uint16_t __socket);

    int TryToConnect() override;

    void Connect() override;

    void Reconnect() override;

    void SendMessage() override;

    void RecievMessage() override;
};

class Server : public NetworkMember
{
public:
    Server(const char* __ipAdress, uint16_t __socket);

    int TryToConnect() override;
    
    void Connect() override;

    void SendMessage() override;

    void RecievMessage() override;

private:
    int clientSocketDescriptor = -1;
};
