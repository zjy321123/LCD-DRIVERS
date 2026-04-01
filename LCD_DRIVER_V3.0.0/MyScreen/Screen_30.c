#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen, Pre_screen;
uint8_t Index_Scr30 = 0;
extern uint16_t Autotick;
void Screen_30_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG)
    {
        if (Index_Scr30 < 3)
            Index_Scr30++;
    }
    else if (Keyid == KEYFORWARD_TRG)
    {
        if (Index_Scr30 > 0)
            Index_Scr30--;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_30_btn_4, LV_EVENT_PRESSED, NULL); // 加载界面screen_37
        current_screen = SCREEN_37;
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Index_Scr30 == 0)
        {
            keyboard_SendCmdTime(CMD_PRG, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        }
        else if (Index_Scr30 == 1)
        {
            keyboard_SendCmdTime(CMD_STOP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        }
        else if (Index_Scr30 == 2)
        {
            keyboard_SendCmdTime(CMD_STOP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        }
        else if (Index_Scr30 == 3)
        {
            lv_event_send(guider_ui.screen_30_btn_4, LV_EVENT_PRESSED, NULL); // 加载界面screen_20
            Screen20date_preloading();
            current_screen = SCREEN_20;
            return;
        }
    }
    Screen30date_preloading();
}

void Screen30date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_30_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_30_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_30_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_30_btn_4, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Index_Scr30 == 0)
        lv_event_send(guider_ui.screen_30_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr30 == 1)
        lv_event_send(guider_ui.screen_30_btn_2, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr30 == 2)
        lv_event_send(guider_ui.screen_30_btn_3, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr30 == 3)
        lv_event_send(guider_ui.screen_30_btn_4, LV_EVENT_CLICKED, NULL);
}
