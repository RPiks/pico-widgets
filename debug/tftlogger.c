///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  tftlogger.c - Logging functions.
// 
//
//  DESCRIPTION
//
//      Implements logging functions for the library. The data is written to
//  the TFT screen onto its pre-defines location.
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
