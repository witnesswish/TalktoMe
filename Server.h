#ifndef TALKTOME_SERVER_H
#define TALKTOME_SERVER_H

#include <list>
#include <ctime>

class Server
{
public:
    Server(const char *ip="127.0.0.1", int port=3434);
    ~Server();
    void Initializer();
    void StartServer();
    void CloseServer();
private:
    int master;
    std::list<int> online_user;

};

#endif // TALKTOME_SERVER_H
