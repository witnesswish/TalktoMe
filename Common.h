#ifndef TALKTOME_COMMON_H
#define TALKTOME_COMMON_H

#include <iostream>
#include <sys/epoll.h>
#include <fcntl.h>

#define WELCOME_MESSAGE "You are connected to TalktoMe server, welcome."

void epoll_add_fd(int fd, int epfd, bool et_enable)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if(et_enable)
    {
        ev.events = EPOLLIN | EPOLLET;
    }
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);
}

static int tag_count = 0;

template <typename T>
void tag(T t)
{
    tag_count += 1;
    std::cout<< "tag here ------- " << "tag num is: " << tag_count << ": " << t <<std::endl;
}

struct Msg
{
    int FromID;
    int ToID;
    char Content[1024];
};

#endif // TALKTOME_COMMON_H
