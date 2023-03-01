///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  TerminalEventProc.c - Terminal widget functions.
// 
//
//  DESCRIPTION
//
//      Implements the Terminal widget of pico-widgets UI library.
//  The widget is a combo of rather basic stylus-activated keyboard & text
//  window. It requires scarce RAM due to the fact that keyboard is hard-coded 
//  in order to use with 240x320 TFT screen via pico-touchscr-sdk.
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

typedef struct
{
    uint16_t x;
    uint16_t y;
} point2d_t;

/// @brief Widget's context.
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

static const char skKeysCap[] = "1234567890-+QWERTYUIOPASDFGHJKL:\"^ZXCVBNM,./\x08\x0d ";
static const char skKeysLow[] = "1234567890-+qwertyuiopasdfghjkl:\"^zxcvbnm,./\x08\x0d ";

void KeyboardDraw(frame *pkbd, screen_control_t *pscr, 
                  color_t paper, color_t ink, int mode);
int LocateKeyTouch(int x, int y);
void Key16x16Draw(screen_control_t *pscr, point2d_t tl);
void BrickDraw(screen_control_t *pscr, point2d_t tl, point2d_t br, int over);

/// @brief Event procssor of the Terminal widget.
/// @param pF The widget.
/// @param fE Incoming event type.
/// @param x Touch X coord.
/// @param y Touch Y coord.
/// @param pctx User interface context.
/// @return 0.
int TerminalEventProc(frame *pF, frame_event fE, int x, int y, void *pctx)
{
    ASSERT(pF);
    ASSERT(pctx);

    static terminal_context_t sTermCtx = { 0 };

    ui_context *pui = pctx;

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

/// @brief Draws a keyboard.
/// @param pkbd The widget.
/// @param pscr Screen control structure.
/// @param paper Paper color (background).
/// @param ink Ink color (foreground).
/// @param mode Defines which symbol set to display:
/// @param mode mode=1 - capital letters, 0 - lowcase.
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

/// @brief Draws a common key as filled rectangle.
/// @param pscr Screen control sutucture.
/// @param tl Top-left point of the key (rect).
/// @param br Bot-right point of the key (rect).
/// @param over 1-positive drawing, 0-negative.
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

/// @brief Touch coords to key index converter.
/// @param x X coord of the touch.
/// @param y Y coord of the touch.
/// @return Index of key has been located. -1 if no.
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
