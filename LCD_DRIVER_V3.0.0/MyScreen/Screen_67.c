#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Customdate.h"
#include "Calendar.h"
extern current_screen_t current_screen, Pre_screen;
static uint8_t Index_Scr67 = 0;
extern uint8_t g_TimerPreIndex;
void Screen_67_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Index_Scr67 == 0)
            Index_Scr67 = 1;
        else if (Index_Scr67 == 1)
            Index_Scr67 = 0;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Index_Scr67 == 0)
            Index_Scr67 = 1;
        else if (Index_Scr67 == 1)
            Index_Scr67 = 0;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Index_Scr67 == 0) // Confirm 删除
        {
            SetCustomTimerData(g_TimerPreIndex, NOT_USED, FORMAT_12HOUSRS,
                               0 OPEN_HOUR, 0 OPEN_MINUTE, 0 OPEN_SECOND, DefaultType, 0 PCT,
                               0 CLOSE_HOUR, 0 CLOSE_MINUTE, 0 CLOSE_SECOND, DefaultType, 0 PCT,
                               NOWEEK, TIMER_DISABLE, NoShade, NOT_USED, NULL);
            S_TIMER_EXECUTE_INFO nextTimer;
            RTC_Set_Next_AlarmTimer(&nextTimer);
            SaveDataToFlash();
            lv_event_send(guider_ui.screen_67_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_62
            current_screen = SCREEN_61;
            Screen61date_preloading();
            return;
        }
        else if (Index_Scr67 == 1) // 取消
        {
            lv_event_send(guider_ui.screen_67_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_62
            current_screen = SCREEN_62;
            Screen62date_preloading();
            return;
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_67_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_62
        current_screen = SCREEN_62;
        Screen62date_preloading();
        return;
    }
    Screen67date_preloading();
}

void Screen67date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_67_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_67_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Index_Scr67 == 0)
        lv_event_send(guider_ui.screen_67_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr67 == 1)
        lv_event_send(guider_ui.screen_67_btn_2, LV_EVENT_CLICKED, NULL);
}
