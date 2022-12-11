#include "Network.h"
#include "algorithm"
#include <arpa/inet.h>
#include <ifaddrs.h>
#include "../Log/Log.h"



void NetworkMember::SendMessagePrototype(int __socketDescriptor)
{
    int Total = 0, Len = buffer.size(), BytesWereSend;

    while(Total < Len)
    {
        BytesWereSend =  send(__socketDescriptor, buffer.data() + Total, Len - Total, 0);

        if(BytesWereSend == 0)
        {
            Reconnect();
        }

        Total += BytesWereSend;
    }
}

void NetworkMember::RecievMessagePrototype(int __socketDescriptor)
{
   int Total = 0, BytesWereReciev; 

    BytesWereReciev = recv(__socketDescriptor, readWriteBuffer + Total, 512 - Total, 0);

    if(BytesWereReciev == 0)
    {
        Reconnect();
    }

    Total += BytesWereReciev;
        
        
    for(int i = 0; i < Total; i++)
        buffer.push_back( *(readWriteBuffer + i) );
}

void NetworkMember::proccessError()
{
    switch (error)
    {
    case NetworkErrors::NoErrors:
        break;

    case NetworkErrors::CantCreateSocket:
        LOG("Не удалось создать сокет!");
        break;
    
    case NetworkErrors::CantConnect:
        LOG("Не удалось установить соединение!");
        break; 

    case NetworkErrors::CantBindSocket:
        LOG("Не удалось зарезервировать сокет!");
        break; 

    case NetworkErrors::CantAccept:
        LOG("Не удалось принять подключение!");
        break; 
    }
}


Client::Client()
{
    serverSocketParams.sin_family = AF_INET; 
}

bool Client::TryToConnect(std::string __serverIPAddress, uint16_t __sereverSocket)
{
    serverSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocketDescriptor == -1)
    {
        error = NetworkErrors::CantCreateSocket;
        return false;
    }

    serverSocketParams.sin_addr.s_addr = inet_addr(__serverIPAddress.c_str()); 
    serverSocketParams.sin_port = htons(__sereverSocket);

    //inet_pton(AF_INET, __ipAdress, &socketParams.sin_addr);

    CONSOLE("", "Устанавливается соединение, ожидайте...", '\n');
    int result = connect(serverSocketDescriptor, (struct sockaddr*)&serverSocketParams, sizeof(serverSocketParams));

    if(result < 0)
    {
        error = NetworkErrors::CantConnect;
        return false;
    }

    return true;
}

void Client::getUserInput(std::string& __serverIPAddress, uint16_t& __sereverSocket)
{
    CLEAR_ALL_TERMINAL();

    proccessError();

    CONSOLE('\n', "IPv4 сервера:", " ");
    std::cin >> __serverIPAddress;
    CONSOLE("", "", '\n');

    CONSOLE("", "Порт:", " ");
    std::cin >> __sereverSocket;
    CONSOLE("", "", '\n');
}

void Client::Connect()
{
    std::string serverIPAddress;
    uint16_t serverSocket;

    do
    {
        getUserInput(serverIPAddress, serverSocket);
    }
    while(!TryToConnect(serverIPAddress, serverSocket));

}

void Client::Reconnect()
{
    close(serverSocketDescriptor);
    serverSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0); 

    Connect();
}

void Client::SendMessage()
{
   SendMessagePrototype(serverSocketDescriptor);
}

void Client::RecievMessage()
{
    RecievMessagePrototype(serverSocketDescriptor);   
}


Server::Server()
{
    socketParams.sin_family = AF_INET; 
    socketParams.sin_addr.s_addr = htonl(INADDR_ANY);
}

bool Server::TryToConnect(uint16_t __socket)
{
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socketDescriptor == -1)
    {
        error = NetworkErrors::CantCreateSocket;
        return false;
    }

    socketParams.sin_port = htons(__socket);

    int result = bind(socketDescriptor, (struct sockaddr*)&socketParams, sizeof(socketParams));

    if(result < 0)
    {
        error = NetworkErrors::CantBindSocket;
        return false;
    }

    CONSOLE("", "Ожидайте подключения...",  '\n'); 
    listen(socketDescriptor, 1);

    clientSocketDescriptor = accept(socketDescriptor, NULL, NULL);

    if(clientSocketDescriptor < 0)
    {
        error = NetworkErrors::CantAccept;
        return false;
    }

    return true;
}

void Server::getUserInput(uint16_t& __socket)
{
    CLEAR_ALL_TERMINAL();

    proccessError();

    CONSOLE("", "Порт:", " ");
    std::cin >> __socket;
    CONSOLE("", "", '\n');
}

void Server::Connect()
{
    uint16_t serverSocket;

    do
    {
        getUserInput(serverSocket);
    }
    while(!TryToConnect(serverSocket));
}

void Server::SendMessage()
{
    SendMessagePrototype(clientSocketDescriptor);
    
}

void Server::RecievMessage()
{
    RecievMessagePrototype(clientSocketDescriptor);
    
}
