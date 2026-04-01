#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
extern current_screen_t current_screen, Mem_Screen;
void Screen_3_Movie(unsigned int Keyid)
{
    if (Keyid == KEYRETURN_TRG)
    {
        if (Mem_Screen == SCREEN_20)
        {
            lv_event_send(guider_ui.screen_3_label_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
            Screen20date_preloading();
            current_screen = SCREEN_20;
            return;
        }
        else
        {
            lv_event_send(guider_ui.screen_3_label_2, LV_EVENT_CLICKED, NULL); // 加载界面screen_2
            Screen2date_preloading();
            current_screen = SCREEN_2;
            return;
        }
    }
    else if (Keyid == KEYENTER_TRG)
    {

        lv_event_send(guider_ui.screen_3_btn_5, LV_EVENT_CLICKED, NULL); // 加载界面screen_4
        Screen4date_preloading();
        current_screen = SCREEN_4;
        return;
    }
}
