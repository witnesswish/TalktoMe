#ifndef TALKTOME_SERVER_H
#define TALKTOME_SERVER_H

#include <list>
#include <ctime>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define EPOLL_SIZE 5000

class Server
{
public:
    Server(const char *ip="127.0.0.1", int port=3434);
    ~Server();
    void Initializer();
    void StartServer();
    void CloseServer();
    int read_cb(int fd);
private:
    int master;
    int epfd;
    static std::list<int> online_user;
    struct sockaddr_in ServAddr;
    char RecvBuffer[100];
    char SendBuffer[100];
};

#endif // TALKTOME_SERVER_H
