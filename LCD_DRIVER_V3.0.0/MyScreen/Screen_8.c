#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
extern current_screen_t current_screen, Mem_Screen;
static uint8_t Scre8Cnt = 0;
void Screen_8_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre8Cnt == 0)
            Scre8Cnt = 1;
        else if (Scre8Cnt == 1)
            Scre8Cnt = 0;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre8Cnt == 0)
            Scre8Cnt = 1;
        else if (Scre8Cnt == 1)
            Scre8Cnt = 0;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre8Cnt == 0)
        {
            if (Mem_Screen == SCREEN_20)
            {
                lv_event_send(guider_ui.screen_8_btn_6, LV_EVENT_PRESSED, NULL); // 加载界面screen_20
                current_screen = SCREEN_20;
                Mem_Screen = SCREEN_NUll;
                Screen20date_preloading();
                return;
            }
            else
            {
                lv_event_send(guider_ui.screen_8_btn_6, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_9
                current_screen = SCREEN_9;
                Mem_Screen = SCREEN_8;
                Screen9date_preloading();
                return;
            }
        }
        else if (Scre8Cnt == 1)
        {
            lv_event_send(guider_ui.screen_8_btn_5, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_4
            current_screen = SCREEN_4;
            Screen4date_preloading();
            return;
        }
    }
    Screen8date_preloading();
}

void Screen8date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_8_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_8_btn_6, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre8Cnt == 0)
        lv_event_send(guider_ui.screen_8_btn_6, LV_EVENT_CLICKED, NULL);
    else if (Scre8Cnt == 1)
        lv_event_send(guider_ui.screen_8_btn_5, LV_EVENT_CLICKED, NULL);
}
