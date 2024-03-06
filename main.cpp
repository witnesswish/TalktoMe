#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <list>
#include <string.h>
#include <fcntl.h>

struct sockaddr_in serv_addr;
struct sockaddr_in6 serv_addr6;
std::list<int> clients;

int main()
{
    /**
    if(daemon(0, 0))
    {
        perror("daemon error");
        exit(-1);
    }**/
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3434);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int serv = socket(AF_INET, SOCK_STREAM, 0);
    if(serv < 0)
    {
        perror("socket failure");
        exit(-1);
    }
    else
    {
        std::cout<< "socket successful, your socket id is: " << serv <<std::endl;
    }
    //reuse of portal
    int opt = 1;
    unsigned int len = sizeof(opt);
    setsockopt(serv, SOL_SOCKET, SO_REUSEADDR, &opt, len);
    int ret_bind = bind(serv, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(ret_bind < 0)
    {
        perror("bind error");
        exit(-1);
    }
    int ret_listen = listen(serv, 10);
    if(ret_listen < 0)
    {
        perror("listen error");
        exit(-1);
    }
    else
    {
        std::cout<< "listening for client." <<std::endl;
    }
    char recvbuff[300];
    bool flag = true;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(serv, (struct sockaddr *)&client_addr, &client_addr_len);
    if(client_fd < 0)
    {
        perror("error accured wile accept client");
    }
    else
    {
        std::cout<< "accept client, client socket id is: " << client_fd <<std::endl;
        int ret = send(client_fd, "This is a message from server, you are now connected.", sizeof("This is a message from server, you are now connected."), 0);
        if(ret < 0)
        {
            perror("send welcome error");
        }
        for(std::list<int>::iterator it = clients.begin(); it != clients.end(); ++it)
        {
            if(*it == client_fd)
            {
                std::cout<< "client already logged.";
                flag = false;
                break;
            }
        }
        if(flag)
        {
            clients.push_back(client_fd);
        }
    }

        //fcntl(serv, F_SETFL, O_NONBLOCK);
        //fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFD, 0)| O_NONBLOCK);
        int sockstate = fcntl(serv, F_GETFL);
        int clientstate = fcntl(client_fd, F_GETFL);
    while(1)
    {
        if(sockstate)
        {
            std::cout<< "serv state code: " << sockstate <<std::endl;
        }
        if(clientstate)
        {
            std::cout<< "client state code: " << clientstate <<std::endl;
        }

        memset(recvbuff, 0, sizeof(recvbuff));
        if(recv(client_fd, recvbuff, 300, 0) < 0 && errno != EAGAIN)
        {
            perror("recv from client error");
            std::cout<< "errno is: " << errno <<std::endl;
            continue;
        }
        else
        {
            std::cout<< recvbuff;
        }
    }

    return 0;
}
