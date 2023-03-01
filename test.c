///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  test.c - The main testing routine.
//
//
//  DESCRIPTION
//
//      Implements pretty basic demo of the pico-widgets library.
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
#include <stdlib.h>

#include <ili9341.h>
#include <msp2807_touch.h>
#include <msp2807_calibration.h>
#include "ISR.h"

#include <ui_context.h>
#include <ui_protos.h>

int main()
{
    ui_context *pUI = InitUI();

    screen_control_t *pScrCtl = &pUI->mScreenCtl;
    touch_control_t *pTouchCtl= &pUI->mTouchCtl;
    
    ili9341_config_t ili9341_hw_config;
    pScrCtl->mpHWConfig = &ili9341_hw_config;
    ILI9341_Init(pScrCtl->mpHWConfig, spi0, 90 * MHz, 4, 5, 6, 7, 8, 9);

    TftClearScreenBuffer(pScrCtl, kBlack, kYellow);
    TftSetCursor(pScrCtl, 0, 20);

    touch_hwconfig_t touch_hwc;
    TouchInitHW(&touch_hwc, spi1, 1 * MHz, 12, 13, 10, 11, 15);
    TouchInitCtl(pTouchCtl, &touch_hwc, 1000, 50000, 5);

    /* Set reference points of the screen as its corners. */
    const int16_t refpoints[] =
    {
        0,   0,
        240, 0,
        0,   320,
        240, 320
    };
    /* Set sampled points of the screen as outputs of touchscreen when 
       reference points had been pressed.
       Admittedly there should be a calibration procedure in your project
       in order to get these points asking user to click a number of points
       of the screen. */
    const int16_t smplpoints[] =
    {
        10,  120,   //  0,   0.
        119, 119,   //  240, 0.
        9,   11,    //  ..
        118, 12     //  ..
    };
    /* Calculate transformation matrix using a number of reference points
       and sampled ones. */
    CalculateCalibrationMat(refpoints, smplpoints, 4, &pUI->mTouchCalMat);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    InitISR();
   
    for(int tick = 0;;++tick)
    {
        tight_loop_contents();       
        
        {
            static uint64_t sLastTm = 0;
            const uint64_t ktm64_now = GetTime();
   
            /* Prevent flicker. */
            if(ktm64_now - 20000L < sLastTm)
            {
                continue;
            }
            sLastTm = ktm64_now;

            frame *pfActive = GetActiveFrame(pUI);
            static frame *spfActiveM1 = NULL;
            if(!pfActive)
            {
                continue;
            }

            if(pfActive != spfActiveM1)
            {
                pfActive->mpfEventProc(pfActive, kEventDraw, 0, 0, pUI);
                spfActiveM1 = pfActive;
                continue;
            }
            spfActiveM1 = pfActive;

            if(pUI->mLastReleaseTm > pUI->mLastTouchTm)
                if(pfActive->mpfEventProc)
                {
                    IsInsideRect(&pfActive->mRegion, pUI->mLastX, pUI->mLastY)
                        ? pfActive->mpfEventProc(pfActive, kEventClickInside, pUI->mLastX, pUI->mLastY, pUI)
                        : pfActive->mpfEventProc(pfActive, kEventClickOutside, pUI->mLastX, pUI->mLastY, pUI);
                }
            pUI->mLastTouchTm = ktm64_now;
        }
    }
}
