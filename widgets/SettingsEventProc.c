///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  SettingsEventProc.c - Settings widget functions.
// 
//
//  DESCRIPTION
//
//      Implements the basic `list of options` widget of pico-widgets UI library.
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

static frame_rect sSettingsListActivationRect[2] = {0};

void SettingsListDraw(frame *pF, ui_context *pctx);
void SettingsListClickInside(frame *pF, int x, int y, ui_context *pctx);

/// @brief Event procssor of the Settings widget.
/// @param pF The widget.
/// @param fE Incoming event type.
/// @param x Touch X coord.
/// @param y Touch Y coord.
/// @param pctx User interface context.
/// @return 0.
int SettingsEventProc(frame *pF, frame_event fE, int x, int y, void *pctx)
{
    ASSERT(pF);
    ASSERT(pctx);

    ui_context *pui = pctx;

    switch (fE)
    {
        case kEventDraw:
        SettingsListDraw(pF, pctx);
        break;

        case kEventClickInside:
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        SettingsListClickInside(pF, x, y, pui);
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

/// @brief Draws the widget.
/// @param pF The widget.
/// @param pctx UI context.
void SettingsListDraw(frame *pF, ui_context *pctx)
{
    DrawBBox(&pctx->mScreenCtl, &pF->mRegion, 0);
    //DebugPrintf("SettingsListDraw\n");
}

/// @brief Defines what to do when click inside the widget.
/// @param pF The widget.
/// @param x Click X coord.
/// @param y Click Y coord.
/// @param pctx User interface context.
void SettingsListClickInside(frame *pF, int x, int y, ui_context *pctx)
{
    DebugPrintf("SettingsList - ClickInside!\n");
}
