#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
extern current_screen_t current_screen, Pre_screen;
static uint8_t Scre18Cnt = 0;
void Screen_18_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre18Cnt < 2)
            Scre18Cnt++;
    }
    else if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre18Cnt > 0)
            Scre18Cnt--;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_18_label_3, LV_EVENT_CLICKED, NULL); // 加载界面screen_17
        current_screen = SCREEN_17;
        Screen17date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre18Cnt == 0)
        {
            lv_event_send(guider_ui.screen_18_btn_3, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_19
            current_screen = SCREEN_19;
            Screen19date_preloading();
            return;
        }
        else if (Scre18Cnt == 1)
        {
            lv_event_send(guider_ui.screen_18_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen
            current_screen = SCREEN;
            Screendate_preloading();
            return;
        }
    }
    Screen18date_preloading();
}

void Screen18date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_18_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_18_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre18Cnt == 0)
        lv_event_send(guider_ui.screen_18_btn_3, LV_EVENT_CLICKED, NULL);
    else if (Scre18Cnt == 1)
        lv_event_send(guider_ui.screen_18_btn_2, LV_EVENT_CLICKED, NULL);
}
