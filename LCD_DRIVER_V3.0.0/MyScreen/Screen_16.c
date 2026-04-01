#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen, Pre_screen, Mem_Screen2;
extern uint16_t Autotick;
extern U_SYS g_SystermFlag;
void Screen_16_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
    }

    else if (Keyid == KEYENTER_TRG)
    {
        lv_event_send(guider_ui.screen_16_btn_2, LV_EVENT_CLICKED, NULL); // 加载界面screen_78
        current_screen = SCREEN_78;
        return;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        if (Mem_Screen2 == SCREEN_20)
        {
            lv_event_send(guider_ui.screen_16_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
            current_screen = SCREEN_20;
            Screen20date_preloading();
            return;
        }
        else
        {
            lv_event_send(guider_ui.screen_16_label_3, LV_EVENT_CLICKED, NULL); // 加载界面screen_15
            current_screen = SCREEN_15;
            Screen15date_preloading();
            return;
        }
    }
}
