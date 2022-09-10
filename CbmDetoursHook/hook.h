#pragma once

#include "packetId.h"

typedef int (WINAPI *pSendTo_t)(SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen);

namespace hook
{
    extern pSendTo_t original;
    extern char playerId;

    extern SOCKET lastSocket;
    extern const sockaddr *lastTo;
    extern int lastTolen;

    int WINAPI _sendto(SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen);
    void init();
}
