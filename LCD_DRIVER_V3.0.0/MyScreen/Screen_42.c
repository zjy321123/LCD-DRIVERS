#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
void Screen_42_Movie(unsigned int Keyid)
{
    if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_42_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
        current_screen = SCREEN_20;
        Screen20date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_2S)
    {

        lv_event_send(guider_ui.screen_42_btn_2, LV_EVENT_CLICKED, NULL); // 加载界面screen_43
        current_screen = SCREEN_43;
        Screen43date_preloading();
        return;
    }
}
