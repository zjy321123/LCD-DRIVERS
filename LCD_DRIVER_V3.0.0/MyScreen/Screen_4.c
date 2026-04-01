#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
extern current_screen_t current_screen;
static uint8_t Scre4Cnt = 0;
void Screen_4_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre4Cnt == 0)
            Scre4Cnt = 1;
        else if (Scre4Cnt == 1)
            Scre4Cnt = 0;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre4Cnt == 0)
            Scre4Cnt = 1;
        else if (Scre4Cnt == 1)
            Scre4Cnt = 0;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_4_btn_5, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_3
        current_screen = SCREEN_3;
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre4Cnt == 0)
        {
            lv_event_send(guider_ui.screen_4_btn_6, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_5
            Screen5date_preloading();
            current_screen = SCREEN_5;
            return;
        }
        else if (Scre4Cnt == 1)
        {
            lv_event_send(guider_ui.screen_4_btn_5, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_3
            current_screen = SCREEN_3;
            return;
        }
    }
    Screen4date_preloading();
}

void Screen4date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_4_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_4_btn_6, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre4Cnt == 0)
        lv_event_send(guider_ui.screen_4_btn_6, LV_EVENT_CLICKED, NULL);
    else if (Scre4Cnt == 1)
        lv_event_send(guider_ui.screen_4_btn_5, LV_EVENT_CLICKED, NULL);
}


