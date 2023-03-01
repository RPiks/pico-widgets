///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  TopBarEventProc.c - Top Bar widget functions.
// 
//
//  DESCRIPTION
//
//      Implements the Top Bar widget of pico-widgets UI library.
//
//  PLATFORM
//      Hardware: Raspberry Pi Pico.
//      Software: https://github.com/RPiks/pico-touchscr-sdk
//
//  REVISION HISTORY
// 
//      Rev 0.5   25 Feb 2023
//  Initial release.
//
//  LICENCE
//      MIT License (http://www.opensource.org/licenses/mit-license.php)
//
//  Copyright (c) 2023 by Roman Piksaykin
//  
//  Permission is hereby granted, free of charge,to any person obtaining a copy
//  of this software and associated documentation files (the Software), to deal
//  in the Software without restriction,including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY,WHETHER IN AN ACTION OF CONTRACT,TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////
#include <frame.h>
#include <ui_context.h>
#include <ui_protos.h>
#include "widgets_utility.h"

#include <tftlogger.h>

static frame_rect sTopBarActivationRect[4] = {0};

void TopBarDraw(frame *pF, ui_context *pctx);
void TopBarClickInside(frame *pF, int x, int y, ui_context *pctx);

/// @brief Event procssor of the Top Bar widget.
/// @param pF The widget.
/// @param fE Incoming event type.
/// @param x Touch X coord.
/// @param y Touch Y coord.
/// @param pctx User interface context.
/// @return 0.
int TopBarEventProc(frame *pF, frame_event fE, int x, int y, void *pctx)
{
    ASSERT(pF);
    ASSERT(pctx);

    ui_context *pui = (ui_context *)pctx;

    switch (fE)
    {
        case kEventDraw:
        TopBarDraw(pF, pui);
        break;

        case kEventClickInside:
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        TopBarClickInside(pF, x, y, pui);
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

/// @brief Draws the Top Bar widget.
/// @param pF The widget.
/// @param pctx User interface context.
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

/// @brief Defines what to do when click inside the widget.
/// @param pF The widget.
/// @param x Click X coord.
/// @param y Click Y coord.
/// @param pctx User interface context.
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
