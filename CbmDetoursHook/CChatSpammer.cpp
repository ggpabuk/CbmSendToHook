#include "pch.h"
#include "CChatSpammer.h"

#pragma warning (disable : 6386)

const char *CChatSpammer::m_szMessage;

void CChatSpammer::Spam(bool *pbIsEnabled)
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (!*pbIsEnabled || hook::playerId < 1) continue;

        int buflen = strlen(m_szMessage) + 5;
        char *buf = new char[buflen];

        buf[0] = PACKETID::M_CHAT;
        buf[1] = hook::playerId;

        strcpy_s(buf + 2, buflen, m_szMessage);

        buf[buflen - 3] = 0x0D;
        buf[buflen - 2] = 0x0A;
        buf[buflen - 1] = 0x01;

        hook::original(hook::lastSocket, buf, buflen, 0, hook::lastTo, hook::lastTolen);
        
        delete[] buf;
    }
}

CChatSpammer::CChatSpammer(int iKey, bool bHoldToUse) :
    CExploit(iKey, bHoldToUse, "Chat spammer")
{
    m_szMessage = " was killed by t.me/+atSs47FtngUyMGJi";
    std::thread(Spam, &m_bIsEnabled).detach();
}
