#include "pch.h"
#include "hook.h"

typedef int (WINAPI *pSendTo_t)(SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen);
static pSendTo_t original;

int WINAPI hook::sendTo(SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen)
{
    printf("%d bytes sent\n", len);

    return original(s, buf, len, flags, to, tolen);
}

void hook::init()
{
    original = reinterpret_cast<pSendTo_t>(DetourFunction(reinterpret_cast<PBYTE>(sendto), reinterpret_cast<PBYTE>(hook::sendTo)));
}
