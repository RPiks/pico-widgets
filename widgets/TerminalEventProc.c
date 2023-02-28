#include <frame.h>
#include <ui_context.h>
#include <ui_protos.h>
#include "widgets_utility.h"

#include <tftlogger.h>

typedef struct
{
    uint16_t x;
    uint16_t y;
} point2d_t;

typedef struct
{
    uint8_t flags;
    uint64_t tm_last;
} terminal_context_t;

static const int8_t skCrook[][2] =  {   {1, -1}, {1, 2},    {2, -2}, {2, 3},    {2, -1}, {2, 2}
                                    };

static const uint8_t skKeys[][4] =  {   {2, 0, 18, 16},     {22, 0, 38, 16},    {42, 0, 58, 16},    {62, 0, 78, 16}, 
                                        {82, 0, 98, 16},    {102, 0, 118, 16},  {122, 0, 138, 16},  {142, 0, 158, 16}, 
                                        {162, 0, 178, 16},  {182, 0, 198, 16},  {202, 0, 218, 16},  {222, 0, 238, 16}, 
                                        {2, 20, 18, 36},    {22, 20, 38, 36},   {42, 20, 58, 36},   {62, 20, 78, 36}, 
                                        {82, 20, 98, 36},   {102, 20, 118, 36}, {122, 20, 138, 36}, {142, 20, 158, 36}, 
                                        {162, 20, 178, 36}, {182, 20, 198, 36}, {2, 40, 18, 56}, 
                                        {22, 40, 38, 56},   {42, 40, 58, 56},   {62, 40, 78, 56},   {82, 40, 98, 56}, 
                                        {102, 40, 118, 56}, {122, 40, 138, 56}, {142, 40, 158, 56}, {162, 40, 178, 56}, 
                                        {182, 40, 198, 56}, {202, 40, 218, 56}, {2, 60, 18, 76},    {22, 60, 38, 76}, 
                                        {42, 60, 58, 76},   {62, 60, 78, 76},   {82, 60, 98, 76},   {102, 60, 118, 76}, 
                                        {122, 60, 138, 76}, {142, 60, 158, 76}, {162, 60, 178, 76}, {182, 60, 198, 76}, 
                                        {202, 60, 218, 76},

                                        {202, 20, 238, 36}, // BAK
                                        {222, 40, 238, 76}, // Enter
                                        {62, 80, 178, 98}   // Space
                                    };

static const char skKeysCap[] = "1234567890-+QWERTYUIOPASDFGHJKL:\"^ZXCVBNM,./   ";
static const char skKeysLow[] = "1234567890-+qwertyuiopasdfghjkl:\"^zxcvbnm,./   ";

void KeyboardDraw(frame *pkbd, screen_control_t *pscr, 
                  color_t paper, color_t ink, int mode);
int LocateKeyTouch(int x, int y);
void Key16x16Draw(screen_control_t *pscr, point2d_t tl);
void BrickDraw(screen_control_t *pscr, point2d_t tl, point2d_t br, int over);

int TerminalEventProc(frame *pF, frame_event fE, int x, int y, void *pctx)
{
    ASSERT(pF);
    ASSERT(pctx);

    static terminal_context_t sTermCtx = { 0 };

    ui_context *pui = pctx;

    //terminal_context_t *sTermCtx = pF->mpPayLoad;

    switch (fE)
    {
        case kEventDraw:
        KeyboardDraw(pF, &pui->mScreenCtl, 0, 4, sTermCtx.flags&1);
        break;

        case kEventClickInside:
        {
            const int i = LocateKeyTouch(x, y);
            if(-1 == i)
            {
                break;
            }
            
            const uint64_t tm_press = GetTime();
            if(tm_press - sTermCtx.tm_last < 200000L)
            {
                break;
            }
            
            sTermCtx.tm_last = tm_press;

            if('^' == skKeysCap[i])
            {
                sTermCtx.flags ^= 0b1;
                KeyboardDraw(pF, &pui->mScreenCtl, 0, 4, sTermCtx.flags&1);
            }
            else
            {
                DebugPrintf("%c", (sTermCtx.flags&1) ? skKeysCap[i] : skKeysLow[i]);
            }
            
            gpio_put(PICO_DEFAULT_LED_PIN, 1);
        }
        break;

        case kEventClickOutside:
        pF->mpPayLoad = NULL;
        PopFrame(pctx);
        ClearWidgetBBox(pF, &pui->mScreenCtl);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        break;

        case kEventClose:
        break;

        case kEventPing:
        break;

        case kEventTick:
        break;
    
        default:
        break;
    }

    return 0;
}

void KeyboardDraw(frame *pkbd, screen_control_t *pscr, 
                  color_t paper, color_t ink, int mode)
{
    ASSERT(pkbd);

    for(int j = 27; j < 40; ++j)
        for(int i = 0; i < 30; ++i)
        {
            TftPutColorAttr(pscr, i, j, paper, ink);
        }

    for(int i = 0; i < _countof(skKeys); ++i)
    {
        point2d_t pt0 = { skKeys[i][0], 220 + skKeys[i][1] };
        point2d_t pt1 = { skKeys[i][2], 220 + skKeys[i][3] };
        BrickDraw(pscr, pt0, pt1, 1);

        char buf[2];
        buf[0] = mode ? skKeysCap[i] : skKeysLow[i];
        buf[1] = 0x00;
        TftPutTextLabel(pscr, buf, pt0.x + 4, pt0.y + 5, 0);
    }

    {   // Backspace
        point2d_t pt0 = { skKeys[_countof(skKeys)-3][0], 
                        220 + skKeys[_countof(skKeys)-3][1] };
        TftPutTextLabel(pscr, "BSP", pt0.x + 4, pt0.y + 5, 0);
    }
    
    {   // Enter
        point2d_t pt0 = { 227, 290 };
        point2d_t pt1 = { 235, 292 };
        BrickDraw(pscr, pt0, pt1, 0);
        for(int i = 0; i < 6; ++i)
        {
            CLR_DATA_BIT(pscr->mpPixBuffer, pt0.x + skCrook[i][0] 
                        + (pt0.y + skCrook[i][1]) * PIX_WIDTH);
        }

        pt0 = pt1;
        pt0.x -= 2; pt0.y -= 22;
        BrickDraw(pscr, pt0, pt1, 0);
    }

    return;
}

void Key16x16Draw(screen_control_t *pscr, point2d_t tl)
{
    TftPutLine(pscr, tl.x + 3, tl.y, tl.x + 16 - 3, tl.y);              // TOP.
    TftPutLine(pscr, tl.x + 3, tl.y + 16, tl.x + 16 - 3, tl.y + 16);    // BOT.
    TftPutLine(pscr, tl.x, tl.y + 3, tl.x, tl.y + 16 - 3);              // LFT.
    TftPutLine(pscr, tl.x + 16, tl.y + 3, tl.x + 16, tl.y + 16 - 3);    // RGT.
}

void BrickDraw(screen_control_t *pscr, point2d_t tl, point2d_t br, int over)
{
    for(int j = tl.y; j < br.y; ++j)
    {
        const int bitline = PIX_WIDTH * j;
        for(int i = tl.x; i < br.x; ++i)
        {
            if(over)
            {
                SET_DATA_BIT(pscr->mpPixBuffer, i + bitline);
            }
            else
            {
                CLR_DATA_BIT(pscr->mpPixBuffer, i + bitline);
            }
        }
    }
}

int LocateKeyTouch(int x, int y)
{
    for(int i = 0; i < _countof(skKeys); ++i)
    {
        const frame_rect rct =
        {
            skKeys[i][0],
            220 + skKeys[i][1],
            skKeys[i][2] - skKeys[i][0],
            skKeys[i][3] - skKeys[i][1]
        };

        if(IsInsideRect(&rct, x, y))
        {
            return i;
        }
    }

    return -1;
}
