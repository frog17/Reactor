#ifndef REACTOR_H
#define REACTOR_H

#include <memory>

#include "event_loop.h"

class EventLoop;

// Demultiplex and dispatch Event_Handlers in response to client requests.
class Reactor {
public:
    Reactor();
    ~Reactor();

    void init(int port);
    void run();
    void connectClient(int fd);
    void recvFrom(int fd);
    void sendTo(int fd);

    // Entry point into the reactive event loop.
    int handle_events(int *timeout = 0);

private:
    /* 将文件描述符设置成非阻塞 */
    int setNonBlocking(int fd);
    void setReuseAddr(int sockfd);

    std::unique_ptr<EventLoop> loop;
};

#endif