#include <frame.h>
#include <ui_context.h>
#include <ui_protos.h>
#include "widgets_utility.h"

void TopBarDraw(frame *pF, ui_context *pctx);

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
        break;

        case kEventClickOutside:
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
                                    8 * pF->mRegion.mWidth);

    DrawTextBBox(&pctx->mScreenCtl, &pF->mRegion, 0);

    for(int i = 0, x_draw = kGap; i < _countof(kTitleLength); ++i)
    {
        const char *pith_title = GetUIItem(2 + i)->mpTitle;
        const uint16_t y_top = pF->mRegion.mTly << 3;
        const uint16_t xleft = x_draw;

        TftPutTextLabel(&pctx->mScreenCtl, pith_title, x_draw, y_top + 4, 1);
        x_draw += kGap + kTitleLength[i];

        TftPutLine(&pctx->mScreenCtl, x_draw, y_top, x_draw, 
                   y_top + (pF->mRegion.mHeight<<3) - 1);

        /* Assign activation rects. */
        //AssignRect2Atom(&(ptopbar->mpChildren + i)->mClickRegion,
        //        xleft, y_top, x_draw - xleft, ptopbar->mRegion.mHeight<<3);

        x_draw += kGap + 1;
    }

    SetRegionColor(&pctx->mScreenCtl, &pF->mRegion, pF->mPaper, pF->mInk);

}
