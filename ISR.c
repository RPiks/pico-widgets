///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  ISR.c - Testing interrupt service routine where it is supposed to invoke
//      UI tick function (UI job).
//
//  DESCRIPTION
//
//      Implements an 1 kHz oscillator in order to process the data.
//
//  PLATFORM
//      Hardware: Raspberry Pi Pico.
//      Software: https://github.com/RPiks/pico-touchscr-sdk
//
//  REVISION HISTORY
// 
//      Rev 0.5   23 Feb 2023
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
#include "ISR.h"

static uint64_t sFutureTm;

/// @brief 1 kHz oscillator.
void __not_in_flash_func (tmISR)(void)
{
    static int32_t sTick = 0;
    gpio_put(PICO_DEFAULT_LED_PIN, (++sTick)&1);

    if(0b11 == (sTick & 0b11))
    {
        UItick(GetUIContext());
    }

    sFutureTm += OSC_CLK_INTERVAL;            /* Increment oscillator phase. */

    hw_clear_bits(&timer_hw->intr, 1U<<TIMER_ALARM_NUM); /* Stop protection. */
    timer_hw->alarm[TIMER_ALARM_NUM] = (uint32_t)sFutureTm;     /* Set next. */
}

/// @brief Inits the timer, the Pico's interrupt system.
void InitISR(void)
{
    hw_set_bits(&timer_hw->inte, 1U<<TIMER_ALARM_NUM);       /* Set tm. int. */
    irq_set_exclusive_handler(TIMER_ALARM_IRQ, tmISR);
    irq_set_priority(TIMER_ALARM_IRQ, 0x00);            /* highest priority. */
    irq_set_enabled(TIMER_ALARM_IRQ, true);                   /* Enable ISR. */

    sFutureTm = timer_hw->timerawl + 500;        /* Postpone first ISR call. */
    timer_hw->alarm[TIMER_ALARM_NUM] 
                    = (uint32_t)sFutureTm;     /* Set first ISR launch time. */
}
