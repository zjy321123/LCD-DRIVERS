#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
extern current_screen_t current_screen;
static uint8_t Scre10Cnt = 0;
extern uint8_t current_index, current_mode, Cachecurrent_index, Cachecurrent_mode;
;
void Screen_10_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre10Cnt == 0)
            Scre10Cnt = 1;
        else if (Scre10Cnt == 1)
            Scre10Cnt = 0;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre10Cnt == 0)
            Scre10Cnt = 1;
        else if (Scre10Cnt == 1)
            Scre10Cnt = 0;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_10_btn_2, LV_EVENT_PRESSED, NULL); // 加载界面screen_20
        current_screen = SCREEN_20;
        Screen20date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre10Cnt == 0)
        {
            lv_event_send(guider_ui.screen_10_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_11
            current_screen = SCREEN_11;
            return;
        }
        else if (Scre10Cnt == 1)
        {
            current_index = Cachecurrent_index;
            current_mode = Cachecurrent_mode;
            Cachecurrent_mode = Cachecurrent_index=0;
            lv_event_send(guider_ui.screen_10_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_4
            current_screen = SCREEN_2;
            Screen2date_preloading();
            return;
        }
    }
    Screen10date_preloading();
}

void Screen10date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_10_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_10_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre10Cnt == 0)
        lv_event_send(guider_ui.screen_10_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Scre10Cnt == 1)
        lv_event_send(guider_ui.screen_10_btn_2, LV_EVENT_CLICKED, NULL);
}
