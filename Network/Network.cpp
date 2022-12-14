#include "Network.h"
#include "algorithm"
#include <arpa/inet.h>
#include <ifaddrs.h>
#include "../Log/Log.h"



int NetworkMember::SendMessagePrototype(int __socketDescriptor)
{
    int total = 0, len = buffer.size(), bytesWereSend;

    while(total < len)
    {
        bytesWereSend =  send(__socketDescriptor, buffer.data() + total, len - total, 0);

        if(bytesWereSend == 0)
        {
            //Reconnect();
            return 0;
        }

        total += bytesWereSend;
    }

    return -1;
}

int NetworkMember::RecievMessagePrototype(int __socketDescriptor)
{
   int total = 0, bytesWereReciev, recievBufferSize = sizeof(readWriteBuffer); 

    bytesWereReciev = recv(__socketDescriptor, readWriteBuffer + total, recievBufferSize - total, 0);

    if(bytesWereReciev == 0)
    {
        //Reconnect();
        return 0;
    }

    total += bytesWereReciev;
        
        
    for(int i = 0; i < total; i++)
        buffer.push_back( *(readWriteBuffer + i) );

    return -1;
}

void NetworkMember::logError()
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

    logError();

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

int Client::SendMessage(std::string __message)
{
    buffer = __message;

   int result = SendMessagePrototype(serverSocketDescriptor);

   buffer.clear();

   return result;
}

int Client::RecievMessage(std::string& __input)
{
    buffer.clear();

    int result = RecievMessagePrototype(serverSocketDescriptor);

    __input = buffer;   

    return result;
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

    logError();

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

void Server::Reconnect() 
{
    Connect();
} 

int Server::SendMessage(std::string __message)
{
    buffer = __message;

    int result = SendMessagePrototype(clientSocketDescriptor);
    
    buffer.clear();

    return result;
}

int Server::RecievMessage(std::string& __input)
{   
    buffer.clear();

    int result = RecievMessagePrototype(clientSocketDescriptor);
    
    __input = buffer;

    return result;
}
