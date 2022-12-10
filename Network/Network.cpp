#include "Network.h"
#include "algorithm"
#include <arpa/inet.h>
#include "../Log/Log.h"


NetworkMember::NetworkMember(const char* __ipAdress, uint16_t __socket)
{
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0); //Ipv4 + TCP
    if(socketDescriptor < 0)
    {
        std::cout << "\nCan't create socket...\n";
    }

    socketParams.sin_family = AF_INET; 
    socketParams.sin_port = htons(__socket);
   // inet_pton(AF_INET, __ipAdress, &socketParams.sin_addr);
   std::cout << inet_addr(__ipAdress) << std::endl;
    socketParams.sin_addr.s_addr = htonl(INADDR_ANY);
    std::cout <<socketParams.sin_addr.s_addr << std::endl;
}

void NetworkMember::Reconnect()
{
    Connected = false;
    Connect();
}

void NetworkMember::SendMessagePrototype(int __socketDescriptor)
{
    if(Connected)
    {
        int Total = 0, Len = buffer.size(), BytesWereSend;

        while(Total < Len)
        {
            BytesWereSend =  send(__socketDescriptor, buffer.data() + Total, Len - Total, 0);

            if(BytesWereSend <= 0)
            {
                break;
            }

            Total += BytesWereSend;
        }
    }
    else
    {
        Reconnect();
    }
}

void NetworkMember::RecievMessagePrototype(int __socketDescriptor)
{
   if(Connected)
   {
        int Total = 0, BytesWereReciev; 

        BytesWereReciev = recv(__socketDescriptor, recvBuffer + Total, 512 - Total, 0);

        if(BytesWereReciev == 0)
            {
                Reconnect();
            }

        Total += BytesWereReciev;
        
        
        for(int i = 0; i < Total; i++)
            buffer.push_back( *(recvBuffer + i) );

   }
   else
   {
        Reconnect();
   }
}


Client::Client(const char* __ipAdress, uint16_t __socket) 
: NetworkMember(__ipAdress, __socket)
{

}

int Client::TryToConnect()
{
    //CONSOLE("\nTrying to connect...")
    std::cout << "\nTrying to connect...";

    int Result = connect(socketDescriptor, (struct sockaddr*)&socketParams, sizeof(socketParams));

    if(Result >= 0)
    {
     //   CONSOLE("Connected!\n")
        std::cout << "\nConnected!";
        Connected = true;
        //Notify
    }
    else
    {
  //      LOG("Connection is failed\nReconnection in 5 seconds...\n")
        Connected = false;
    }

    return Result;
}

void Client::Connect()
{
    while(TryToConnect() < 0)
    {
        sleep(5);
        Connected = false;
      //  OnConnectionChanged.Call();
    }
    
    Connected = true;
    //Notify
    //  OnConnectionChanged.Call();
        
}

void Client::Reconnect()
{
    Connected = false;
    close(socketDescriptor);
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0); 
    Connect();
}

void Client::SendMessage()
{
   SendMessagePrototype(socketDescriptor);
}

void Client::RecievMessage()
{
    RecievMessagePrototype(socketDescriptor);   
}


Server::Server(const char* __ipAdress, uint16_t __socket) 
: NetworkMember(__ipAdress, __socket)
{
    if(bind(socketDescriptor, (struct sockaddr*)&socketParams, sizeof(socketParams)) < 0)
    {
 //       LOG("Can't bind socket\n")
   //     CONSOLE("Waiting while OS makes socket free...\n")

        std::cout << socketParams.sin_addr.s_addr << std::endl;

        while(bind(socketDescriptor, (struct sockaddr*)&socketParams, sizeof(socketParams)) < 0)
        {
            sleep(30);
        }
    }
}

void Server::Connect()
{
    TryToConnect();
}

int Server::TryToConnect()
{
    listen(socketDescriptor, 1);

    clientSocketDescriptor = accept(socketDescriptor, NULL, NULL);
    std::cout << "yes\n"; 

    if(clientSocketDescriptor < 0)
    {
    //    LOG("Can't accept\n")
        std::cout << "\nCan't accept\n";
    }

    Connected = true;
 //   CONSOLE("Connected!\n")
    std::cout << "Connected!\n";

    return clientSocketDescriptor;
}

void Server::SendMessage()
{
    SendMessagePrototype(clientSocketDescriptor);
    
}

void Server::RecievMessage()
{
    RecievMessagePrototype(clientSocketDescriptor);
    
}
