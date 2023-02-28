#include <frame.h>
#include <ui_context.h>
#include <ui_protos.h>
#include "widgets_utility.h"

#include <tftlogger.h>

static frame_rect sTopBarActivationRect[4] = {0};

void TopBarDraw(frame *pF, ui_context *pctx);
void TopBarClickInside(frame *pF, int x, int y, ui_context *pctx);

int TopBarEventProc(frame *pF, frame_event fE, int x, int y, void *pctx)
{
    ASSERT(pF);
    ASSERT(pctx);

    switch (fE)
    {
        case kEventDraw:
        TopBarDraw(pF, pctx);
        break;

        case kEventClickInside:
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        TopBarClickInside(pF, x, y, pctx);
        break;

        case kEventClickOutside:
        // No reaction: this is persistent widget.
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

void TopBarDraw(frame *pF, ui_context *pctx)
{
    ASSERT(pF->mFrameType == kFrameBBox);

    int kTitleLength[4];
    for(int i = 0; i < _countof(kTitleLength); ++i)
    {
        kTitleLength[i] = 8 * strlen(GetUIItem(2 + i)->mpTitle);
    }

    const int kGap = FitTitlesHoriz(kTitleLength, _countof(kTitleLength),
                                    pF->mRegion.mWidth);

    DrawBBox(&pctx->mScreenCtl, &pF->mRegion, 0);

    for(int i = 0, x_draw = kGap; i < _countof(kTitleLength); ++i)
    {
        const char *pith_title = GetUIItem(2 + i)->mpTitle;
        const uint16_t y_top = pF->mRegion.mTly;
        const uint16_t xleft = x_draw;

        TftPutTextLabel(&pctx->mScreenCtl, pith_title, x_draw, y_top + 4, 1);
        x_draw += kGap + kTitleLength[i];

        TftPutLine(&pctx->mScreenCtl, x_draw, y_top, x_draw, 
                   y_top + (pF->mRegion.mHeight) - 1);

        /* Assign activation rect. */
        AssignRect(sTopBarActivationRect + i, xleft, y_top,
                   x_draw - xleft, pF->mRegion.mHeight);

        x_draw += kGap + 1;
    }

    frame_rect rect = { pF->mRegion.mTlx>>3, pF->mRegion.mTly>>3,
                        pF->mRegion.mWidth>>3, pF->mRegion.mHeight>>3};
    SetRegionColor(&pctx->mScreenCtl, &rect, pF->mPaper, pF->mInk);
}

void TopBarClickInside(frame *pF, int x, int y, ui_context *pctx)
{
    const int ix = SelectTargetRect(sTopBarActivationRect, 
                                    _countof(sTopBarActivationRect), x, y);
    //DebugPrintf("x:%d y:%d IX click: %d  \n", x, y, ix);
    
    switch (ix)
    {
        case 0:
        PushStdFrame(pctx, 2);
        break;

        case 2:
        PushStdFrame(pctx, 8);
        break;
    
        default:
        break;
    }
}
