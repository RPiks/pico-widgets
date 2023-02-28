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
       reference points had been pressed. */
    const int16_t smplpoints[] =
    {
        10,  120,   //  0,   0.
        119, 119,   //  240, 0.
        9,   11,    //  ..
        118, 12     //  ..
    };

    CalculateCalibrationMat(refpoints, smplpoints, 4, &pUI->mTouchCalMat);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    InitADC();
   
    for(int tick = 0;;++tick)
    {
        tight_loop_contents();       

        if(((1<<12) - 1) == (tick & ((1<<12) - 1)))
        {
            TftFullScreenSelectiveWrite(pScrCtl, 1);
        }
    }
}
