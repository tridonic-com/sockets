
#include "aSocket.h"
#include "socket_api.h"

void aSocket::_nvEventHandler(void * arg) {
    (void) arg;
    _event = _socket.event; // TODO: (CThunk upgrade/Alpha3)
    if (_event->event == SOCKET_EVENT_DNS) {
        _onDNS(_event);
    } else {
        _eventHandler(_event);
    }
    _event = NULL; // TODO: (CThunk upgrade/Alpha3)
}

socket_event_t * aSocket::getEvent()
{
    return _event; // TODO: (CThunk upgrade/Alpha3)
}

socket_error_t aSocket::resolve(const char* address, SocketAddr *addr, handler_t onDNS)
{
    _onDNS = onDNS;
    socket_error_t err = socket_resolve(&_socket, address, addr->getAddr());
    if (err == SOCKET_ERROR_NONE) {
        socket_event_t event;
        event.event = SOCKET_EVENT_DNS;
        event.i.d.domain = address;
        event.i.d.addr.type = addr->getAddr()->type;
        event.i.d.addr.impl = addr->getImpl();
        event.i.d.sock = &_socket;
        _onDNS(&event);
    }
    return err;
}

socket_error_t aSocket::close()
{
    return socket_close(&_socket);
}
void aSocket::abort()
{
    return socket_abort(&_socket);
}
