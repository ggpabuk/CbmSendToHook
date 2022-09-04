#pragma once

namespace hook
{
    int WINAPI sendTo(SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen);
    void init();
}
