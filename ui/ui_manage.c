///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  ui_manage.c - User interface service functions.
// 
//
//  DESCRIPTION
//
//      Implements functions of UI.
//
//  PLATFORM
//      Hardware: Raspberry Pi Pico.
//      Software: https://github.com/RPiks/pico-touchscr-sdk
//
//  REVISION HISTORY
// 
//      Rev 0.5   24 Feb 2023
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
#ifndef UI_MANAGE_H_
#define UI_MANAGE_H_

#include <frame.h>
#include "ui_context.h"
#include "ui_protos.h"
#include <ili9341.h>
#include <msp2807_touch.h>

#include <tftlogger.h>

void PopFrame(ui_context* pcntx)
{
    ASSERT(pcntx);
    ASSERT(pcntx->mFrameActiveIX);
    --pcntx->mFrameActiveIX;
}

void PushCustomFrame(ui_context *pcntx, frame *pframe)
{
    ASSERT(pcntx);
    pcntx->mpFrameStack[pcntx->mFrameActiveIX++] = pframe;
}

frame *GetActiveFrame(ui_context *pcntx)
{
    ASSERT(pcntx);
    if(0 == pcntx->mFrameActiveIX)
    {
        return NULL;
    }
    return (frame *)pcntx->mpFrameStack[pcntx->mFrameActiveIX - 1];
}

void DrawFrame(ui_context *pcntx, frame *pkframe)
{
    ASSERT(pkframe);

    switch(pkframe->mFrameType)
    {
        case kFrameRoot:
        //TftClearScreenBuffer()
        break;

        case kFrameBBox:
        pkframe->mpfEventProc(pkframe, kEventDraw, 0, 0, pcntx);
        break;

        default:
        break;
    };
}

/// @brief Measures a current time.
/// @return A count of 1 us ticks from start.
uint64_t GetTime(void)
{
    const uint32_t ktlo = timer_hw->timelr;
    const uint32_t kthi = timer_hw->timehr;

    return ((uint64_t)kthi << 32) | ktlo;
}

void UItick(ui_context *pcntx)
{
    ASSERT(pcntx);
    
    //static uint64_t sLastTm = 0;
    //const uint64_t ktm64_now = GetTime();
    
    /* Check whether enough time has passed. */
    //if(ktm64_now - sleep_interval < sLastTm)
    //{
    //    return;
    //}
    //sLastTm = ktm64_now;

    const bool kbnew_touch = GetTouchData(pcntx, &pcntx->mLastX, 
                                          &pcntx->mLastY);
    if(kbnew_touch)
    {
        pcntx->mLastTouchTm = GetTime();
    }

    TftFullScreenSelectiveWrite(&pcntx->mScreenCtl, 8);
}

/// @brief Gets data from touchscreen device.
/// @param puic UI context.
/// @param x X coord of the touch point, if pressed.
/// @param y Y coord of the touch point, if pressed.
/// @return Whether the screen is pressed and {x,y} are valid.
bool GetTouchData(ui_context *puic, int32_t *x, int32_t *y)
{
    ASSERT(puic);

    const int res = CheckTouch(&puic->mTouchCtl);
    if(0 == res)
    {
        *x = (puic->mTouchCtl.mXf + 8) >> 4;
        *y = (puic->mTouchCtl.mYf + 8) >> 4;

        TouchTransformCoords(&puic->mTouchCalMat, x, y);

        puic->mLastReleaseTm = 0;

        return true;
    }
    else if(1 == res && !puic->mLastReleaseTm)
    {
        puic->mLastReleaseTm = GetTime();
    }
    
    return false;
}

/// @brief Check if {x,y} `graph mode` are inside rectange.
/// @param rct The rectangle.
/// @param x X coord.
/// @param y Y coord.
/// @return True if is inside.
bool IsInsideRect(const frame_rect *rct, int32_t x, int32_t y)
{
    if((x < rct->mTlx)
    || (y < rct->mTly)
    || (x > rct->mTlx + rct->mWidth)
    || (y > rct->mTly + rct->mHeight))
    {
        return false;
    }

    return true;
}

#endif
