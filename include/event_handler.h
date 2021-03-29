#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <functional>

using callback = std::function<void(int)>;

/*
 * 每个EventHandler封装一个fd,fd事件注册,改变,处理均调用此类,但是生命周期不由此类管理
 */
class EventHandler {
public:
    EventHandler(int);
    ~EventHandler();

    // Hook methods that are called back by the Initiation Dispatcher to handle particular types of events.
    int handle_accept(void);
    
    void setReadable(bool);
    void setReadHandle(callback);

    void setWritable(bool);
    void setWriteHandle(callback);

    int handle_close(void);

    void handleEvent(int);

    // Hook method that returns the underlying I/O Handle.
    // virtual Handle get_handle (void) const = 0;

private:
    int fd;
    bool readable;
    bool writable;
    callback readHandle;
    callback writeHandle;
};

#endif