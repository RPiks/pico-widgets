#include <frame.h>
#include <ui_context.h>
#include <ui_protos.h>
#include "widgets_utility.h"

#include <tftlogger.h>

void KeyboardDraw(frame *pkbd, screen_control_t *pscr, 
                  color_t paper, color_t ink, int mode);

int TerminalEventProc(frame *pF, frame_event fE, int x, int y, void *pctx)
{
    ASSERT(pF);
    ASSERT(pctx);

    ui_context *pui = pctx;

    switch (fE)
    {
        case kEventDraw:
        KeyboardDraw(pF, &pui->mScreenCtl, 0, 4, 1);
        //SettingsListDraw(pF, pctx);
        break;

        case kEventClickInside:
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        
        break;

        case kEventClickOutside:
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

typedef struct
{
    uint16_t x;
    uint16_t y;
} point2d_t;
static const char skKeysCapital[][13] = {"1234567890-+","QWERTYUIOP\x08","ASDFGHJKL:\"","^ZXCVBNM,./","\x0b/ :"};
static const char skKeysLowerCase[][13] = {"1234567890-+","qwertyuiop\x08","asdfghjkl\x0d\x0b","zxcvbnm,./","\x0b/ :"};
static const point2d_t skKeyBBTopLefts[] = { {2, 220}, {2, 240}, {2, 260}, {2, 280}, {22, 300} };

void Key16x16Draw(screen_control_t *pscr, point2d_t tl);
void BrickDraw(screen_control_t *pscr, point2d_t tl, point2d_t br, int over);

void KeyboardDraw(frame *pkbd, screen_control_t *pscr, 
                  color_t paper, color_t ink, int mode)
{
    ASSERT(pkbd);

    for(int j = 27; j < 40; ++j)
        for(int i = 0; i < 30; ++i)
        {
            TftPutColorAttr(pscr, i, j, paper, ink);
        }

    char buf[2] = {0};
    for(int j = 0; j < 4; ++j)
    {
        const int len = strlen(skKeysCapital[j]);
        point2d_t pt = skKeyBBTopLefts[j];
        point2d_t pt2 = pt;
        pt2.x += 16; pt2.y += 16;
        for(int i = 0; i < len; ++i)
        {
            BrickDraw(pscr, pt, pt2, 1);
            buf[0] = mode ? skKeysCapital[j][i]
                          : skKeysLowerCase[j][i];
            TftPutTextLabel(pscr, buf, pt.x + 4, pt.y + 5, 0);
            pt.x += 20; pt2.x += 20;
        }
    }

    point2d_t pt = skKeyBBTopLefts[1];
    pt.x += 20 * 10; point2d_t pt2 = pt;
    pt2.x += 20 + 16; pt2.y += 16;
    BrickDraw(pscr, pt, pt2, 1);   // Backspace.
    TftPutTextLabel(pscr, "BAK", pt.x + 6, pt.y + 5, 0);

    /* Enter key. */
    pt = skKeyBBTopLefts[2];
    pt.x += 20 * 11; pt2 = pt;
    pt2.x += 16; pt2.y += 36;
    BrickDraw(pscr, pt, pt2, 1);   // Enter.
    
    pt2.x -= 3; pt2.y -= 4;
    pt = pt2; pt.x -= 8; pt.y -= 2;
    BrickDraw(pscr, pt, pt2, 0);
    CLR_DATA_BIT(pscr->mpPixBuffer, pt.x + 1 + (pt.y - 1) * PIX_WIDTH);
    CLR_DATA_BIT(pscr->mpPixBuffer, pt.x + 1 + (pt.y + 2) * PIX_WIDTH);
    CLR_DATA_BIT(pscr->mpPixBuffer, pt.x + 2 + (pt.y - 2) * PIX_WIDTH);
    CLR_DATA_BIT(pscr->mpPixBuffer, pt.x + 2 + (pt.y + 3) * PIX_WIDTH);
    CLR_DATA_BIT(pscr->mpPixBuffer, pt.x + 2 + (pt.y - 1) * PIX_WIDTH);
    CLR_DATA_BIT(pscr->mpPixBuffer, pt.x + 2 + (pt.y + 2) * PIX_WIDTH);

    pt = pt2;
    pt.x -= 2; pt.y -= 22;
    BrickDraw(pscr, pt, pt2, 0);

    /* Space brick. */
    pt = skKeyBBTopLefts[4];
    pt.x += 20 * 2; pt2 = pt;
    pt2.x += 20 * 6 - 4; pt2.y += 18;
    BrickDraw(pscr, pt, pt2, 1);
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
