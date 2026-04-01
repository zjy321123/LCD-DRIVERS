#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
extern current_screen_t current_screen, Pre_screen;
static uint8_t Scre47Cnt = 0;

void Screen_47_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre47Cnt < 2)
            Scre47Cnt++;
    }
    else if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre47Cnt > 0)
            Scre47Cnt--;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre47Cnt == 0)
        {
            lv_event_send(guider_ui.screen_47_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
            current_screen = SCREEN_20;
            Screen20date_preloading();
            return;
        }
        else if (Scre47Cnt == 1)
        {
            lv_event_send(guider_ui.screen_47_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_45
            current_screen = SCREEN_45;
            return;
        }
        else if (Scre47Cnt == 2)
        {
            lv_event_send(guider_ui.screen_47_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
            current_screen = SCREEN_20;
            Screen20date_preloading();
            return;
        }
    }
    Screen47date_preloading();
}

void Screen47date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_47_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_47_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_47_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre47Cnt == 0)
        lv_event_send(guider_ui.screen_47_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Scre47Cnt == 1)
        lv_event_send(guider_ui.screen_47_btn_2, LV_EVENT_CLICKED, NULL);
    else if (Scre47Cnt == 2)
        lv_event_send(guider_ui.screen_47_btn_3, LV_EVENT_CLICKED, NULL);
}
