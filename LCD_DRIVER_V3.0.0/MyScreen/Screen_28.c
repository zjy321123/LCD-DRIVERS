#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
static uint8_t Scre28Cnt = 0;
void Screen_28_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre28Cnt == 0)
            Scre28Cnt = 1;
        else if (Scre28Cnt == 1)
            Scre28Cnt = 0;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre28Cnt == 0)
            Scre28Cnt = 1;
        else if (Scre28Cnt == 1)
            Scre28Cnt = 0;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_28_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
        current_screen = SCREEN_20;
        Screen20date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre28Cnt == 0)
        {
            lv_event_send(guider_ui.screen_28_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_29
            current_screen = SCREEN_29;
            Screen29date_preloading();
            return;
        }
        else if (Scre28Cnt == 1)
        {
            lv_event_send(guider_ui.screen_28_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
            current_screen = SCREEN_20;
            Screen20date_preloading();
            return;
        }
    }
    Screen28date_preloading();
}

void Screen28date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_28_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_28_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre28Cnt == 0)
        lv_event_send(guider_ui.screen_28_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Scre28Cnt == 1)
        lv_event_send(guider_ui.screen_28_btn_2, LV_EVENT_CLICKED, NULL);
}
