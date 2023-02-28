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
//      Implements ADC initialization as well as ADC ISR with UI tick call.
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

void __not_in_flash_func (ADCrxISR)(void)
{
    while(!adc_fifo_is_empty())
    {
        const int32_t val = adc_fifo_get();
    }

    UItick(GetUIContext(), 20000L);    /* Call UI service [20 ms events proc.] */
}

void InitADC(void)
{
    adc_init();
    adc_gpio_init(ADC_PIN); /* Make sure GPIO is high-impedance,no pullups.*/
    adc_select_input(ANALOG_RECEIVER_INPUT);/* Select ADC input 0 (GPIO26) */
    adc_set_clkdiv(ADC_CLKDIV);                            /* Sample rate. */

    adc_fifo_setup(true,      /* Write each conversion to the sample FIFO. */
                   false,              /* Disable DMA data request (DREQ). */
                   3,/* IRQ will be asserted when at least 3 smpl present. */
                   false,            /* We won't see the ERR bit; disable. */
                   false /* No shift each sample to 8 bits. Use full 12 b. */
    );

    irq_set_exclusive_handler(ADC_IRQ_FIFO, ADCrxISR);/* Only one handler. */
    adc_irq_set_enabled(true);
    irq_set_enabled(ADC_IRQ_FIFO, true);
    irq_set_priority(ADC_IRQ_FIFO, PICO_DEFAULT_IRQ_PRIORITY);

    //CB_Init(&gcbufADC, ADC_BUF_SIZE);   /* init ADC input circular buffer. */

    adc_run(true);                     /* Enable free running sample mode. */
}
