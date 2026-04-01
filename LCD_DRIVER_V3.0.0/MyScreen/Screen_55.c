#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
extern current_screen_t current_screen, Pre_screen;
static uint8_t Index_Scr55 = 0;

void Screen_55_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG)
    {
        if (Index_Scr55 < 1)
            Index_Scr55++;
    }
    else if (Keyid == KEYFORWARD_TRG)
    {
        if (Index_Scr55 > 0)
            Index_Scr55--;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Index_Scr55 == 0)
        {
            lv_event_send(guider_ui.screen_55_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
            current_screen = SCREEN_20;
            Screen20date_preloading();
            return;
        }
        else if (Index_Scr55 == 1)
        {
            lv_event_send(guider_ui.screen_55_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_54
            current_screen = SCREEN_54;
            Screen54date_preloading();
            return;
        }
    }
    Screen55date_preloading();
}

void Screen55date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_55_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_55_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Index_Scr55 == 0)
        lv_event_send(guider_ui.screen_55_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr55 == 1)
        lv_event_send(guider_ui.screen_55_btn_2, LV_EVENT_CLICKED, NULL);
}
