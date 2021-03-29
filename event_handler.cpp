#include "include/event_handler.h"

EventHandler::EventHandler(int fd) : fd(fd), readable(false), writable(false) {}

EventHandler::~EventHandler() {}

void EventHandler::setReadable(bool flag)
{
    readable = flag;
}

void EventHandler::setWritable(bool flag)
{
    writable = flag;
}

void EventHandler::setReadHandle(callback call)
{
    readHandle = call;
}

void EventHandler::setWriteHandle(callback call)
{
    writeHandle = call;
}

void EventHandler::handleEvent(int fd)
{
    if (readable && readHandle) {
        readHandle(fd);
    } else if (writable && writeHandle) {
        writeHandle(fd);
    }
}