#pragma once

#include "CExploit.h"
#include "hook.h"
#include "packetId.h"

class CChatSpammer :
    public CExploit
{
private:
    static const char *m_szMessage;

    static void Spam(bool *pbIsEnabled);
public:
    CChatSpammer(int iKey, bool bHoldToUse);
};
