#include <stdio.h>
#include <string.h>
#include "include/demultiplexer.h"

namespace
{
    const int MAX_EVENT_NUMBER = 1024;
}

Demultiplexer::Demultiplexer()
{
    epoll_fd = epoll_create(MAX_EVENT_NUMBER);
    events = new epoll_event[MAX_EVENT_NUMBER];
}

Demultiplexer::~Demultiplexer()
{
    if (!events) {
        delete[] events;
        events = nullptr;
    }
}

void Demultiplexer::registerHandle(int fd)
{
    struct epoll_event et;
    bzero(&et, sizeof(et));

    et.events |= EPOLLIN | EPOLLET;
    et.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &et);
}

void Demultiplexer::modifyHandle(int fd, Event_Type mask)
{
    struct epoll_event event;
    switch (mask)
    {
    case WRITE_EVENT: {
        event.events |= EPOLLOUT;
        event.data.fd=fd;
        break;
    }
    case READ_EVENT: {
        event.events |=EPOLLIN;
        event.data.fd=fd;
        break;
    }
    
    default:
        break;
    }
    epoll_ctl(epoll_fd,EPOLL_CTL_MOD,fd,&event);
}

void Demultiplexer::removeHandle(int fd)
{
    struct epoll_event et;
    bzero(&et, sizeof(et));
    
    et.events = EPOLLOUT;
    et.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &et);
}

void Demultiplexer::demultiplex(std::vector<int> &fds)
{
    int ret = epoll_wait(epoll_fd, events, MAX_EVENT_NUMBER, -1);
    if (ret < 0) {
        perror("epoll wait failed!");
    }

    for (int i = 0; i < ret; i++) {
        fds.push_back(events[i].data.fd);
    }
}