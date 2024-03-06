#ifndef TALKTOME_COMMON_H
#define TALKTOME_COMMON_H

#include <iostream>

void epoll_add_fd(int fd, int epfd)
{

}

struct Msg
{
    int FromID;
    int ToID;
    char RecvBuffer[1024];
    char SendBuffer[1024];
};

#endif // TALKTOME_COMMON_H
