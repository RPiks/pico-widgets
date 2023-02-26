///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  ui_init.c - User interface initialization.
// 
//
//  DESCRIPTION
//
//      Implements functions of UI init.
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
#ifndef UI_STRUCTURE_H_
#define UI_STRUCTURE_H_

#include <frame.h>
#include "ui_context.h"
#include "ui_protos.h"

static ui_context sUIcontext = {0};

static const frame skUI[] =
{
    { kFrameRoot, "root", {0, 0, TEXT_WIDTH, TEXT_HEIGHT}, kBlack, kWhite, NULL, NULL },        // [0]
    { kFrameBBox, "TopBar", {0, 0, 240, 16}, kRed, kWhite, TopBarEventProc, NULL },             // [1]
    { kFrameBBox, "Settings", {32, 32, 64, 72}, kBlue, kCyan, SettingsEventProc, NULL },        // [2]
    { kFrameBBox, "APRS", {32, 32, 0, 72}, kBlue, kCyan, APRSEventProc, NULL },                 // [3]
    { kFrameBBox, "PSK", {32, 32, 0, 72}, kBlue, kCyan, PSKEventProc, NULL },                   // [4]
    { kFrameBBox, "Phone", {32, 32, 0, 72}, kBlue, kCyan, PhoneEventProc, NULL },               // [5]
    { kFrameBBox, "Callsign", {32, 32, 0, 72}, kBlue, kYellow, CallsignEventProc, NULL },       // [6]
    { kFrameBBox, "Calibration", {32, 32, 0, 72}, kBlue, kYellow, CalibrationEventProc, NULL }, // [7]
    { kFrameBBox, "Terminal", {32, 32, 0, 72}, kBlack, kGreen, TerminalEventProc, NULL },       // [8]
};

ui_context* InitUI(void)
{
    PushStdFrame(&sUIcontext, 0);
    PushStdFrame(&sUIcontext, 1);

    return &sUIcontext;
}

ui_context* GetUI(void)
{
    return &sUIcontext;
}

void PushStdFrame(ui_context *pcntx, int ix)
{
    ASSERT(pcntx);
    pcntx->mpFrameStack[pcntx->mFrameActiveIX++] = &skUI[ix];
}

const frame* GetUIItem(int n)
{
    return &skUI[n];
}

#endif
