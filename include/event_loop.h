#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "demultiplexer.h"
#include "event_handler.h"

class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    void init(int port);
    // void setNonBlocking(int listenfd);
    void run();

    /*
     * 添加事件，并将fd注册到内核事件表中
     * fd: 注册的事件id
     * type： 事件类型
     * call：回调函数
     */
    void register_handler(int fd, Event_Type type, callback call);
    void modify_handler(int fd);
    void  remove_handler(int fd);

private:
    bool terminal_;
    // 活跃的fd
    std::vector<int> active_fds;
    // epoll指针
    std::unique_ptr<Demultiplexer> epoll_ptr;
    // 活跃的fd，以及对应的处理函数
    std::unordered_map<int, std::shared_ptr<EventHandler>> event_handles;
};

#endif