#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
extern current_screen_t current_screen;
static uint8_t Scre5Cnt = 0;
void Screen_5_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre5Cnt == 0)
            Scre5Cnt = 1;
        else if (Scre5Cnt == 1)
            Scre5Cnt = 0;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre5Cnt == 0)
            Scre5Cnt = 1;
        else if (Scre5Cnt == 1)
            Scre5Cnt = 0;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_5_label_2, LV_EVENT_CLICKED, NULL); // 加载界面screen_4
        current_screen = SCREEN_4;
        Screen4date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre5Cnt == 0) // 待加载界面6
        {
            lv_event_send(guider_ui.screen_5_btn_6, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_6
            current_screen = SCREEN_6;
            Screen6date_preloading();
            return;
        }
        else if (Scre5Cnt == 1)
        {
            lv_event_send(guider_ui.screen_5_btn_5, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_4
            current_screen = SCREEN_4;
            Screen4date_preloading();
            return;
        }
    }
    Screen5date_preloading();
}

void Screen5date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_5_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_5_btn_6, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre5Cnt == 0)
        lv_event_send(guider_ui.screen_5_btn_6, LV_EVENT_CLICKED, NULL);
    else if (Scre5Cnt == 1)
        lv_event_send(guider_ui.screen_5_btn_5, LV_EVENT_CLICKED, NULL);
}



