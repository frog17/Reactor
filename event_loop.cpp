#include "include/event_loop.h"

EventLoop::EventLoop() : terminal_(false), epoll_ptr(new Demultiplexer) {}

EventLoop::~EventLoop()
{
    terminal_ = true;
}

void EventLoop::run()
{
    while (!terminal_) {
        std::vector<int> temp_fds;
        epoll_ptr->demultiplex(active_fds);  // epoll监听
        active_fds.swap(temp_fds);
        
        for (int i = 0; i < temp_fds.size(); i++) {
            event_handles[temp_fds[i]]->handleEvent(temp_fds[i]);
        }
    }
}

void EventLoop::register_handler(int fd, Event_Type type, callback call)
{
    switch (type)
    {
    case READ_EVENT: {
        auto handle_ptr = std::make_shared<EventHandler>(fd);
        handle_ptr->setReadable(true);
        handle_ptr->setReadHandle(call);
        event_handles[fd] = handle_ptr;
        printf("add a read fd :%d\n",fd);
        epoll_ptr->registerHandle(fd);
        break;
    }
    case WRITE_EVENT:
        event_handles[fd]->setReadable(false);
        event_handles[fd]->setWritable(true);
        event_handles[fd]->setWriteHandle(call);
        printf("add a write fd: %d\n",fd);
        epoll_ptr->modifyHandle(fd, WRITE_EVENT );
    default:
        break;
    }
}

void EventLoop::modify_handler(int fd)
{
    printf("change to read :%d\n",fd);
    event_handles[fd]->setWritable(false);
    event_handles[fd]->setReadable(true);
    epoll_ptr->modifyHandle(fd, WRITE_EVENT);
}

void EventLoop::remove_handler(int fd)
{
    printf("remove a fd :%d\n",fd);
    auto iter = event_handles.find(fd);
    if (iter != event_handles.end()) {
        event_handles.erase(iter);
        epoll_ptr->removeHandle(fd);
    }
}