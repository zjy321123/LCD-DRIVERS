#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
void Screen_37_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        keyboard_SendCmdTime(CMD_UP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        keyboard_SendCmdTime(CMD_DOWN, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
    }
    else if (Keyid == KEYENTER_TRG)
    {
        keyboard_SendCmdTime(CMD_STOP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_37_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
        current_screen = SCREEN_20;
        Screen20date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_2S)
    {

        lv_event_send(guider_ui.screen_37_btn_2, LV_EVENT_CLICKED, NULL); // 加载界面screen_30
        current_screen = SCREEN_30;
        Screen30date_preloading();
        return;
    }
}
