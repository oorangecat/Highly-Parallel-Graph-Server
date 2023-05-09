#ifndef EPOLLINSTANCE_H
#define EPOLLINSTANCE_H

#include <stdint.h>
#include "EpollEntry.hh"

#define EPOLL_MAX_EVENTS 64

class EpollInstance
{
public:
    EpollInstance();
    ~EpollInstance();
    void registerEpollEntry(EpollEntry &e) const;
    void unregisterEpollEntry(EpollEntry &e) const;
    void waitAndHandleEvents(void) const;

private:
    int fd;
};

#endif // EPOLLINSTANCE_H
