#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
void Screen_32_Movie(unsigned int Keyid)
{
    if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_32_btn_2,LV_EVENT_SHORT_CLICKED , NULL); // 加载界面screen_20
        current_screen = SCREEN_20;
        Screen20date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_2S)
    {

        lv_event_send(guider_ui.screen_32_btn_2, LV_EVENT_CLICKED, NULL); // 加载界面screen_7
        current_screen = SCREEN_7;
        Pre_screen = SCREEN_32;
        return;
    }
}
void Screen32_AutoLoadProcess(void)
{
    if (Pre_screen == SCREEN_32)
    {
        Autotick++;
        if (Autotick > SCREEN_32AutoLoadtime)
        {
            Pre_screen = SCREEN_NUll;
            Autotick = 0;
            lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_SCROLL, NULL); // 加载界面screen_33
            current_screen = SCREEN_33;
            Screen33date_preloading();
            return;
        }
    }
}
