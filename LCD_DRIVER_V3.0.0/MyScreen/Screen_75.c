#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
extern current_screen_t current_screen, Pre_screen;
void Screen_75_Movie(unsigned int Keyid)
{
    if (Keyid == KEYENTER_TRG)
    {
        lv_event_send(guider_ui.screen_75_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_1
        current_screen = SCREEN_1;
        Screen1date_preloading();
        return;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_75_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_1
        current_screen = SCREEN_1;
        Screen1date_preloading();
        return;
    }
}
