#include "tftlogger.h"

void DebugPrintf(const char* str, ...)
{
    static int seq = 0;

    screen_control_t *pscr = &GetUIContext()->mScreenCtl;

    if(!seq)
    {
        pscr->mCursorX = 0;
        pscr->mCursorY = 16;
    }
    ++seq;

    va_list argptr;
    va_start(argptr, str);
    
    char buf[TEXT_WIDTH * 4];                   // 4 rows of text max.
    vsnprintf(buf, sizeof(buf), str, argptr);
    va_end(argptr);

    TftPutString(pscr, buf, 16, 22, 0, 5);
}
