#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
extern current_screen_t current_screen, Pre_screen;
static uint8_t Scre41Cnt = 0;

void Screen_41_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre41Cnt < 2)
            Scre41Cnt++;
    }
    else if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre41Cnt > 0)
            Scre41Cnt--;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre41Cnt == 0)
        {
            lv_event_send(guider_ui.screen_41_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
            current_screen = SCREEN_20;
            Screen20date_preloading();
            return;
        }
        else if (Scre41Cnt == 1)
        {
            lv_event_send(guider_ui.screen_41_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_39
            current_screen = SCREEN_39;
            return;
        }
        else if (Scre41Cnt == 2)
        {
            lv_event_send(guider_ui.screen_41_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
            current_screen = SCREEN_20;
            Screen20date_preloading();
            return;
        }
    }
    Screen41date_preloading();
}

void Screen41date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_41_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_41_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_41_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre41Cnt == 0)
        lv_event_send(guider_ui.screen_41_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Scre41Cnt == 1)
        lv_event_send(guider_ui.screen_41_btn_2, LV_EVENT_CLICKED, NULL);
    else if (Scre41Cnt == 2)
        lv_event_send(guider_ui.screen_41_btn_3, LV_EVENT_CLICKED, NULL);
}
