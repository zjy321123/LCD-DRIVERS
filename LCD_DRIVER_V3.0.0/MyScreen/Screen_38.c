#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
static uint8_t Scre38Cnt = 0;
void Screen_38_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre38Cnt == 0)
            Scre38Cnt = 1;
        else if (Scre38Cnt == 1)
            Scre38Cnt = 0;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre38Cnt == 0)
            Scre38Cnt = 1;
        else if (Scre38Cnt == 1)
            Scre38Cnt = 0;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre38Cnt == 0)
        {
            lv_event_send(guider_ui.screen_38_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
            current_screen = SCREEN_20;
             Screen20date_preloading();
            return;
        }
        else if (Scre38Cnt == 1)
        {
            lv_event_send(guider_ui.screen_38_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_37
            current_screen = SCREEN_37;
            return;
        }
    }
    Screen38date_preloading();
}

void Screen38date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_38_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_38_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre38Cnt == 0)
        lv_event_send(guider_ui.screen_38_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Scre38Cnt == 1)
        lv_event_send(guider_ui.screen_38_btn_2, LV_EVENT_CLICKED, NULL);
}

