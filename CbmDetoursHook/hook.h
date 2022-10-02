#pragma once

#include "packetId.h"
#include <map>
#include <functional>

typedef int (WINAPI *pSendTo_t)(SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen);
typedef std::function<void(PACKETID, char *, int)> fnModifer_t;
class hook
{

    static std::map<PACKETID, std::vector<fnModifer_t>> g_packetModifiers;

    static int WINAPI _sendto(SOCKET s, const char *buf, int len, int flags, const sockaddr *to, int tolen);
    static void modifyPacket(PPACKETID pPacketId, char *pPacket, int len);

public:
    static char playerId;

    static SOCKET lastSocket;
    static const sockaddr *lastTo;
    static int lastTolen;

    static pSendTo_t original;
    static void init();
    static void addPacketModifier(PACKETID packetId, fnModifer_t modifierFunction);
};