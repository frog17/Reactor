#ifndef DEMULTIPLEXER_H
#define DEMULTIPLEXER_H

#include <vector>
#include <sys/epoll.h>

enum Event_Type {
    ACCEPT_EVENT = 01,

    READ_EVENT = 02,

    WRITE_EVENT = 04,

    TIMEOUT_EVENT = 010,

    SIGNAL_EVENT = 020,

    CLOSE_EVENT = 040
};

class Demultiplexer {
public:
    Demultiplexer();
    ~Demultiplexer();

    void registerHandle(int fd);
    void modifyHandle(int fd, Event_Type mask);
    void removeHandle(int fd);

    void demultiplex(std::vector<int> &fds);

private:
    int epoll_fd;
    struct epoll_event *events;
};

#endif