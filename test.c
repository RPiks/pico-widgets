#include <stdlib.h>

#include <ili9341.h>
#include <msp2807_touch.h>
#include <msp2807_calibration.h>
#include "ISR.h"

#include <ui_context.h>

ui_context gUIcontext;

int main()
{
    screen_control_t *pScrCtl = &gUIcontext.mScreenCtl;
    touch_control_t *pTouchCtl= &gUIcontext.mTouchCtl;

    static ili9341_config_t ili9341_hw_config;
    pScrCtl->mpHWConfig = &ili9341_hw_config;

    ILI9341_Init(pScrCtl->mpHWConfig, spi0, 90 * MHz, 4, 5, 6, 7, 8, 9);

    TftClearScreenBuffer(pScrCtl, kBlack, kYellow);
    pScrCtl->mCursorY = 20;
    pScrCtl->mCursorX = 0;
    //TftPrintf(pScrCtl, 3, 40, kBlack, kMagenta, "Pix Comm v.0.15 %s is starting\n", __TIME__);
    //TftFullScreenWrite(pScrCtl);

    touch_hwconfig_t touch_hwc;
    TouchInitHW(&touch_hwc, spi1, 1 * MHz, 12, 13, 10, 11, 15);
    TouchInitCtl(pTouchCtl, &touch_hwc, 1000, 50000, 5);

    //TftPrintf(pScrCtl, 3, 40, kBlack, kRed, "Touch screen init passed.\n");
    //TftFullScreenSelectiveWrite(pScrCtl, 10000);

    const int16_t refpoints[] =
    {
        0,   0,
        240, 0,
        0,   320,
        240, 320
    };
    const int16_t smplpoints[] =
    {
        10,  120,
        119, 119,
        9,   11,
        118, 12
    };

    CalculateCalibrationMat(refpoints, smplpoints, 4, &gUIcontext.mTouchCalMat);

    //TftPrintf(pScrCtl, 3, 40, kBlack, kCyan, "Touch screen calibration passed.\n");
    //TftFullScreenSelectiveWrite(pScrCtl, 10000);

    //gUIcontext.mpRoot = CreateUI();
    //gUIcontext.mpRoot->mpNext = NULL;
    //ActivateChildAtom(gUIcontext.mpRoot, 0);
    //TftPrintf(pScrCtl, 3, 40, kBlack, kYellow, "UI hierarchy is allocated.\n");
    //TftFullScreenSelectiveWrite(pScrCtl, 10000);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    InitADC();
    
    //TopBarDraw(gUIcontext.mpRoot->mpChildren, pScrCtl);
    //TftFullScreenSelectiveWrite(pScrCtl, 10000);

    int loop_tick = 0;
    for(;;)
    {
        tight_loop_contents();
        
        ++loop_tick;

        if(((1<<16) - 1) == (loop_tick & ((1<<16) - 1)))
        {
 
        }

        if(((1<<12) - 1) == (loop_tick & ((1<<12) - 1)))
        {
            TftFullScreenSelectiveWrite(pScrCtl, 1);
        }
    }
}
