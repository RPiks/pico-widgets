#include <frame.h>
#include <ui_context.h>
#include <ui_protos.h>
#include "widgets_utility.h"

#include <tftlogger.h>

static frame_rect sSettingsListActivationRect[2] = {0};

void SettingsListDraw(frame *pF, ui_context *pctx);
void SettingsListClickInside(frame *pF, int x, int y, ui_context *pctx);

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

void SettingsListDraw(frame *pF, ui_context *pctx)
{
    DrawBBox(&pctx->mScreenCtl, &pF->mRegion, 0);
    //DebugPrintf("SettingsListDraw\n");
}
void SettingsListClickInside(frame *pF, int x, int y, ui_context *pctx)
{
    DebugPrintf("SettingsList - ClickInside!\n");
}
