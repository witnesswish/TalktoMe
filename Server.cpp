#include "Server.h"
#include "Common.h"

Server::Server(const char *ip, int port)
{
    std::cout<< ip << ":" << port <<std::endl;
}
Server::~Server()
{

}

void Server::Initializer()
{
    std::cout<< "Initializering..." <<std::endl;
    time_t date = time(0);
    char *strdate = ctime(&date);
    std::cout<< "Server started success at " << strdate <<std::endl;
}


void Server::StartServer()
{
    Initializer();
}

void Server::CloseServer()
{

}
