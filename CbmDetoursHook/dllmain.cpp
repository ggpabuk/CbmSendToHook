#include "pch.h"
#include "hook.h"
#include "CChatSpammer.h"

typedef std::unique_ptr<CExploit> exploitPtr;
static std::vector<exploitPtr> g_exploits{};

std::map<PACKETID, std::vector<std::function<void(PACKETID, char *, int)>>> hook::g_packetModifiers;

void createConsole()
{
    AllocConsole();
    FILE *f;

    freopen_s(&f, "CONIN$", "r", stdin);
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
}

void catchHotkeys()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        for (const exploitPtr &exploit : g_exploits)
        {
            const short nKeyState = GetAsyncKeyState(exploit->m_iKey);

            if (exploit->m_bHoldToUse)
            {
                if (nKeyState & 0x8000)
                {
                    if (!exploit->m_bIsEnabled)
                    {
                        exploit->Enable();
                    }
                }
                else if (exploit->m_bIsEnabled)
                {
                    exploit->Disable();
                }

                continue;
            }

            if (nKeyState & 1)
            {
                exploit->Toggle();
            }
        }
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        createConsole();
        hook::init();

        hook::addPacketModifier(M_CHAT, [](PACKETID packetId, char *pPacket, int len)
        {
            for (uint32_t i = 0; i < len; ++i)
            {
                printf("%02X ", pPacket[i] & 0xFF);
            }
            std::cout << '\n';
        });

        g_exploits.push_back(std::make_unique<CChatSpammer>('C', true));

        std::thread(catchHotkeys).detach();
    }

    return TRUE;
}
