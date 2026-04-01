#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen, Pre_screen;
static uint8_t Scre35Cnt = 0;
extern uint16_t Autotick;
void Screen_35_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre35Cnt < 4)
            Scre35Cnt++;
    }
    else if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre35Cnt > 0)
            Scre35Cnt--;
    }
    else if (Keyid == KEYENTER_2S)
    {
        if (Scre35Cnt == 4)
        {
            lv_event_send(guider_ui.screen_35_btn_5, LV_EVENT_PRESSED, NULL); // 加载界面screen_36
            current_screen = SCREEN_36;
            Screen36date_preloading();
            return;
        }
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre35Cnt == 0)
        {
            keyboard_SendCmdTime(CMD_PRG, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        }
        else if (Scre35Cnt == 1)
        {
            keyboard_SendCmdTime(CMD_STOP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        }
        else if (Scre35Cnt == 2)
        {
            keyboard_SendCmdTime(CMD_UP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        }
        else if (Scre35Cnt == 3)
        {
            keyboard_SendCmdTime(CMD_DOWN, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_35_btn_5, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_34
        current_screen = SCREEN_34;
        return;
    }
    Screen35date_preloading();
}

void Screen35date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_35_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_35_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_35_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_35_btn_4, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_35_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre35Cnt == 0)
        lv_event_send(guider_ui.screen_35_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Scre35Cnt == 1)
        lv_event_send(guider_ui.screen_35_btn_2, LV_EVENT_CLICKED, NULL);
    else if (Scre35Cnt == 2)
        lv_event_send(guider_ui.screen_35_btn_3, LV_EVENT_CLICKED, NULL);
    else if (Scre35Cnt == 3)
        lv_event_send(guider_ui.screen_35_btn_4, LV_EVENT_CLICKED, NULL);
    else if (Scre35Cnt == 4)
        lv_event_send(guider_ui.screen_35_btn_5, LV_EVENT_CLICKED, NULL);
}
