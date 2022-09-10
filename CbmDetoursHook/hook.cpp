#include "pch.h"
#include "hook.h"

pSendTo_t hook::original;
char hook::playerId = -1;

SOCKET hook::lastSocket;
const sockaddr *hook::lastTo;
int hook::lastTolen;

int WINAPI hook::_sendto(SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen)
{
    playerId = buf[1];

    lastSocket = s;
    lastTo = to;
    lastTolen = tolen;

    char *patchedbuf = new char[len];
    memcpy(patchedbuf, buf, len);

    PPACKETID pPacketId = reinterpret_cast<PPACKETID>(patchedbuf);

    if (*pPacketId == PACKETID::M_CHAT)
    {
        for (uint32_t i = 0; i < len; ++i)
        {
            printf("%X ", patchedbuf[i]);
        }
        std::cout << '\n';
    }

    int result = original(s, patchedbuf, len, flags, to, tolen);

    delete[] patchedbuf;
    return result;
}

void hook::init()
{
    original = reinterpret_cast<pSendTo_t>(DetourFunction(reinterpret_cast<PBYTE>(sendto), reinterpret_cast<PBYTE>(hook::_sendto)));
}
