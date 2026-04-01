#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "FskComm.h"
extern current_screen_t current_screen, Pre_screen;
static uint8_t Scre23Cnt = 0;

void Screen_23_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre23Cnt < 2)
            Scre23Cnt++;
    }
    else if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre23Cnt > 0)
            Scre23Cnt--;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre23Cnt == 0)
        {
            lv_event_send(guider_ui.screen_23_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
            current_screen = SCREEN_20;
            Screen20date_preloading();
            return;
        }
        else if (Scre23Cnt == 1)
        {
            lv_event_send(guider_ui.screen_23_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_22
            current_screen = SCREEN_22;
            return;
        }
        else if (Scre23Cnt == 2)
        {
            lv_event_send(guider_ui.screen_23_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
            current_screen = SCREEN_20;
            Screen20date_preloading();
            return;
        }
    }
    Screen23date_preloading();
}

void Screen23date_preloading(void)
{
    lv_label_set_text_fmt(guider_ui.screen_23_label_4, "Position:  %d", ReadMotorMsg.CurrentPct);
    lv_obj_set_style_bg_color(guider_ui.screen_23_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_23_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_23_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre23Cnt == 0)
        lv_event_send(guider_ui.screen_23_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Scre23Cnt == 1)
        lv_event_send(guider_ui.screen_23_btn_2, LV_EVENT_CLICKED, NULL);
    else if (Scre23Cnt == 2)
        lv_event_send(guider_ui.screen_23_btn_3, LV_EVENT_CLICKED, NULL);
}
