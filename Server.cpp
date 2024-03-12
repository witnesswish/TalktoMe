#include "Server.h"
#include "Common.h"

Server::Server(const char *ip, int port)
{
    master = 0;
    epfd = 0;
    ServAddr.sin_family = PF_INET;
    ServAddr.sin_addr.s_addr = inet_addr(ip);
    ServAddr.sin_port = htons(port);
}
Server::~Server()
{
    close(master);
    close(epfd);
}

void Server::Initializer()
{
    std::cout<< "Initializering Server..." <<std::endl;
    master = socket(PF_INET, SOCK_STREAM, 0);
    if(master < 0)
    {
        perror("socket master error.");
        exit(-1);
    }
    int opt = 1;
    unsigned int len = sizeof(opt);
    setsockopt(master, SOL_SOCKET, SO_REUSEADDR, &opt, len);
    if(bind(master, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) < 0)
    {
        perror("bind master error.");
        std::cout<< errno <<std::endl;
        exit(-1);
    }
    if(listen(master, 10) < 0)
    {
        perror("listen master error.");
        exit(-1);
    }
    epfd = epoll_create(EPOLL_SIZE);
    if(epfd < 0)
    {
        perror("epollfd create error.");
        exit(-1);
    }
    epoll_add_fd(master, epfd, true);
    time_t date = time(0);
    char *strdate = ctime(&date);
    std::cout<< "Server started success at " << strdate << ", Server run at " << "" <<std::endl;
}


void Server::StartServer()
{
    tag("test");
    struct epoll_event events[EPOLL_SIZE];

    Initializer();

    while(1)
    {
        int epoll_events_count = epoll_wait(epfd, events, EPOLL_SIZE, -1);
        tag(epoll_events_count);
        if(epoll_events_count < 0)
        {
            perror("epoll wait fcntl error.");
            break;
        }
        for(int i=0; i<epoll_events_count; ++i)
        {
            int socketfd = events[i].data.fd;
            if(socketfd == master)
            {
                struct sockaddr_in ClientAddr;
                socklen_t ClientAddr_len = sizeof(ClientAddr);
                int ClientFd = accept(master, (struct sockaddr *)&ClientAddr, &ClientAddr_len);
                if(ClientFd < 0)
                {
                    perror("accept from client error.");
                    continue;
                }
                std::cout<< "Client #" << ClientFd << " connect from " << inet_ntoa(ClientAddr.sin_addr) << ":" << ntohs(ClientAddr.sin_port) <<std::endl;
                epoll_add_fd(ClientFd, epfd, true);
                memcpy(SendBuffer, WELCOME_MESSAGE, sizeof(WELCOME_MESSAGE));
                if(send(ClientFd, SendBuffer, sizeof(SendBuffer), 0) < 0)
                {
                    perror("send welcome message error.");
                    continue;
                }

            }
            else
            {
                int ret = read_cb(socketfd);
                if(ret < 0)
                {
                    perror("process message error");
                    continue;
                }
            }
        }
    }

}

int Server::read_cb(int fd)
{
    std::cout<< "recv from client id #" << fd << " ." <<std::endl;
    Msg msg;
    memset(&msg, 0, sizeof(msg));
    bzero(RecvBuffer, sizeof(RecvBuffer));
    int len = recv(fd, RecvBuffer, sizeof(RecvBuffer), 0);
    tag(len);
    tag(len);
    if(len < 0)
    {
        perror("recv from client error.");
        std::cout<< errno <<std::endl;
        return -1;
    }
    if(len == 0)
    {
        std::cout<< "client close connection." <<std::endl;
        return 0;
    }
    else
    {
        std::cout<< "read from client: " << RecvBuffer <<std::endl;
        return 0;
    }
}

void Server::CloseServer()
{

}
