#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen, Pre_screen, Mem_Screen, Mem_Screen2;
extern uint16_t Autotick;
extern U_SYS g_SystermFlag;
void Screen_14_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
    }

    else if (Keyid == KEYENTER_TRG)
    {
        lv_event_send(guider_ui.screen_14_btn_2, LV_EVENT_CLICKED, NULL); // 加载界面screen_76
        current_screen = SCREEN_76;
        return;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        if (Mem_Screen2 == SCREEN_20)
        {
            lv_event_send(guider_ui.screen_14_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
            current_screen = SCREEN_20;
            Screen20date_preloading();
            return;
        }
        else
        {
            lv_event_send(guider_ui.screen_14_label_3, LV_EVENT_CLICKED, NULL); // 加载界面screen_12
            current_screen = SCREEN_12;
            Screen12date_preloading();
            return;
        }
    }
}
