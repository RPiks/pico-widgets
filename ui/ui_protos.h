///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  ui_protos.h - User interface functions prototypes.
// 
//
//  DESCRIPTION
//
//      Declares functions which do events processing.
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
#ifndef UI_PROTOS_H_
#define UI_PROTOS_H_

#include <frame.h>
#include "ui_context.h"

ui_context* InitUI(void);
ui_context* GetUIContext(void);
const frame* GetUIItem(int n);
void PushStdFrame(ui_context *pcntx, int ix);
void PushCustomFrame(ui_context *pcntx, frame *pframe);
void PopFrame(ui_context* pcntx);
frame *GetActiveFrame(ui_context *pcntx);
void UItick(ui_context *pcntx, int32_t sleep_interval);
bool GetTouchData(ui_context *puic, int32_t *x, int32_t *y);
bool IsInsideRect(const frame_rect *rct, int32_t x, int32_t y);
uint64_t GetTime(void);

int TopBarEventProc(frame *pF, frame_event fE, int x, int y, void *pctx);
int SettingsEventProc(frame *pF, frame_event fE, int x, int y, void *pctx);
int APRSEventProc(frame *pF, frame_event fE, int x, int y, void *pctx);
int PSKEventProc(frame *pF, frame_event fE, int x, int y, void *pctx);
int PhoneEventProc(frame *pF, frame_event fE, int x, int y, void *pctx);
int CallsignEventProc(frame *pF, frame_event fE, int x, int y, void *pctx);
int CalibrationEventProc(frame *pF, frame_event fE, int x, int y, void *pctx);
int TerminalEventProc(frame *pF, frame_event fE, int x, int y, void *pctx);

#endif
