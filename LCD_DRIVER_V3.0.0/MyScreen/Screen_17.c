#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
extern current_screen_t current_screen, Pre_screen, Mem_Screen, Mem_Screen2;
static uint8_t Scre17Cnt = 0;
void Screen_17_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre17Cnt < 2)
            Scre17Cnt++;
    }
    else if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre17Cnt > 0)
            Scre17Cnt--;
    }
    else if (Keyid == KEYENTER_TRG)
    {

        if (Scre17Cnt == 0)
        {
            // 分情况，之前有行程，则删除后重新回到screen_76，没有就值接设置完成，继续设置下行程。
            if (Mem_Screen == SCREEN_79)
            {
                lv_event_send(guider_ui.screen_17_btn_3, LV_EVENT_PRESSED, NULL); // 加载界面screen_78
                current_screen = SCREEN_78;
                Mem_Screen = SCREEN_NUll;
            }
            else
            {
                if (Mem_Screen2 == SCREEN_20)
                {
                    Mem_Screen2 = SCREEN_NUll;
                    lv_event_send(guider_ui.screen_17_btn_3, LV_EVENT_PRESSING, NULL); // 加载界面screen_20
                    current_screen = SCREEN_20;
                    Screen20date_preloading();
                    return;
                }
                else
                {
                    lv_event_send(guider_ui.screen_17_btn_3, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_18
                    current_screen = SCREEN_18;
                    Mem_Screen = SCREEN_NUll;
                    Screen18date_preloading();
                }
            }
            return;
        }
        else if (Scre17Cnt == 1)
        {
            lv_event_send(guider_ui.screen_17_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_16
            current_screen = SCREEN_16;
            return;
        }
        else if (Scre17Cnt == 2)
        {
            if (Mem_Screen2 == SCREEN_20)
            {
                Mem_Screen2 = SCREEN_NUll;
                lv_event_send(guider_ui.screen_17_btn_3, LV_EVENT_PRESSING, NULL); // 加载界面screen_20
                current_screen = SCREEN_20;
                Screen20date_preloading();
                return;
            }
            else
            {
                lv_event_send(guider_ui.screen_17_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen
                current_screen = SCREEN;
                Screendate_preloading();
                return;
            }
        }
        else if (Keyid == KEYRETURN_TRG)
        {
            lv_event_send(guider_ui.screen_15_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_14
            current_screen = SCREEN_14;
            return;
        }
    }
    Screen17date_preloading();
}

void Screen17date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_17_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_17_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_17_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre17Cnt == 0)
        lv_event_send(guider_ui.screen_17_btn_3, LV_EVENT_CLICKED, NULL);
    else if (Scre17Cnt == 1)
        lv_event_send(guider_ui.screen_17_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Scre17Cnt == 2)
        lv_event_send(guider_ui.screen_17_btn_2, LV_EVENT_CLICKED, NULL);
}
