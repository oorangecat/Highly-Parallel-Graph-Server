#include <unistd.h>
#include <sys/epoll.h>
#include <cstring>
#include <stdexcept>
#include "EpollInstance.hh"

EpollInstance::EpollInstance()
{
    this->fd = epoll_create1(0);
    if (this->fd == -1) {
        throw std::runtime_error(
                std::string("epoll_create1: ") + std::strerror(errno));
    }
}

EpollInstance::~EpollInstance()
{
    close(this->fd);
}

void
EpollInstance::registerEpollEntry(EpollEntry &e) const
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));

    ev.events = e.get_events();
    ev.data.ptr = &e;

    if (epoll_ctl(this->fd, EPOLL_CTL_ADD, e.get_fd(), &ev) == -1) {
        throw std::runtime_error(
                std::string("epoll_ctl: ") + std::strerror(errno));
    }
}

void
EpollInstance::unregisterEpollEntry(EpollEntry &e) const
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));

    ev.events = 0;
    ev.data.ptr = &e;

    if (epoll_ctl(this->fd, EPOLL_CTL_DEL, e.get_fd(), &ev) == -1) {
        throw std::runtime_error(
                std::string("epoll_ctl: ") + std::strerror(errno));
    }
}

void
EpollInstance::waitAndHandleEvents(void) const
{
    struct epoll_event events[EPOLL_MAX_EVENTS];
    int n = epoll_wait(this->fd, events, EPOLL_MAX_EVENTS, -1);
    if (n == -1) {
        throw std::runtime_error(
                std::string("epoll_wait: ") + std::strerror(errno));
    }
    for (int i = 0; i < n; i++) {
        EpollEntry *e = static_cast<EpollEntry *>(events[i].data.ptr);
        if (!e->handleEvent(events[i].events)) {
            this->unregisterEpollEntry(*e);
        }
    }
}
