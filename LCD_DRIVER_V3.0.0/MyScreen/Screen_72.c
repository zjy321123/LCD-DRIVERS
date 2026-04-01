#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "tim.h"
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
extern uint8_t BacklightLevel;
static uint8_t Index_Scr72 = 0;
void Screen_72_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Index_Scr72 == 0)
            Index_Scr72 = 1;
        else if (Index_Scr72 == 1)
            Index_Scr72 = 0;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Index_Scr72 == 0)
            Index_Scr72 = 1;
        else if (Index_Scr72 == 1)
            Index_Scr72 = 0;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Index_Scr72 == 1)
        {
            lv_event_send(guider_ui.screen_72_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_68
            current_screen = SCREEN_68;
            Screen68date_preloading();
            return;
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_72_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_68
        current_screen = SCREEN_68;
        Screen68date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_6S)
    {
        if (Index_Scr72 == 0)
        {
            FactortCustomDateInit();
            lv_event_send(guider_ui.screen_72_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_73
            current_screen = SCREEN_73;
            return;
        }
    }
    Screen72date_preloading();
}

void Screen72date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_72_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_72_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Index_Scr72 == 0)
        lv_event_send(guider_ui.screen_72_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr72 == 1)
        lv_event_send(guider_ui.screen_72_btn_2, LV_EVENT_CLICKED, NULL);
}
