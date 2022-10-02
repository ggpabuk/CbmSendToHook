#include "pch.h"
#include "hook.h"

char hook::playerId;

SOCKET hook::lastSocket;
const sockaddr *hook::lastTo;
int hook::lastTolen;

pSendTo_t hook::original;

int WINAPI hook::_sendto(SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen)
{
    if(buf[0] == M_UPDATE) playerId = buf[1];

    lastSocket = s;
    lastTo = to;
    lastTolen = tolen;

    char *patchedbuf = new char[len];
    memcpy(patchedbuf, buf, len);
    PPACKETID pPacketId = reinterpret_cast<PPACKETID>(patchedbuf);

    modifyPacket(pPacketId, patchedbuf, len);

    int result = original(s, patchedbuf, len, flags, to, tolen);

    delete[] patchedbuf;
    return result;
}

void hook::init()
{
    original = reinterpret_cast<pSendTo_t>(DetourFunction(reinterpret_cast<PBYTE>(sendto), reinterpret_cast<PBYTE>(hook::_sendto)));
}

void hook::modifyPacket(PPACKETID pPacketId, char *pPacket, int len)
{
    auto it = g_packetModifiers.find(*pPacketId);
    if (it == g_packetModifiers.end()) return;

    for (auto &fnModifier : it->second)
    {
        fnModifier(*pPacketId, pPacket, len);
    }
}

void hook::addPacketModifier(PACKETID packetId, fnModifer_t modifierFunction)
{
    g_packetModifiers[packetId].push_back(modifierFunction);
}
