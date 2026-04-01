#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "Si446x.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
void Screen_56_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG)
    {
        lv_event_send(guider_ui.screen_56_btn_3, LV_EVENT_CLICKED, NULL);
        keyboard_SendCmdTime(CMD_DOWN, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
    }
    else if (Keyid == KEYFORWARD_TRG)
    {
        lv_event_send(guider_ui.screen_56_btn_2, LV_EVENT_CLICKED, NULL);
        keyboard_SendCmdTime(CMD_UP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
    }
    else if (Keyid == KEYENTER_TRG)
    {
        lv_event_send(guider_ui.screen_56_btn_1, LV_EVENT_CLICKED, NULL);
        keyboard_SendCmdTime(CMD_STOP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
    }
    else if (Keyid == KEYLEFT_TRG)
    {
        lv_event_send(guider_ui.screen_56_btn_4, LV_EVENT_CLICKED, NULL);
        keyboard_SendCmdTime(CMD_PRG, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
    }
    else if (Keyid == KEYRIGHT_TRG)
    {
        lv_event_send(guider_ui.screen_56_btn_5, LV_EVENT_CLICKED, NULL);
        keyboard_SendCmdTime(CMD_SET, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_56_label_3, LV_EVENT_CLICKED, NULL); // 加载界面screen_20
        current_screen = SCREEN_20;
        Screen20date_preloading();
        return;
    }
    // creen56date_preloading();
}
void Screen56_AutoLoadProcess(void)
{
    if (current_screen == SCREEN_56)
    {
        Autotick++;
        if (Autotick > SCREEN_56AutoLoadtime)
        {
            Autotick = 0;
            Screen56date_preloading();
            return;
        }
    }
}
void Screen56date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_56_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_56_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_56_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_56_btn_4, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_56_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
}

void Screen_56_KeyRelease(unsigned int Keyid)
{
    switch (g_Keyboard.KeyRelease)
    {
    case KEYFORWARD:
        g_RfSendCnt = 0;
        g_Keyboard.KeyHoldDelay = 0;
        g_SystermFlag.Sys.RfSend = 1;
        if (g_Keyboard.KeyHoldFlag == E_KEY_DELAY_START_2S)
            g_RfData.CtrlCmd = CMD_REL_UP;
        else if (g_Keyboard.KeyHoldFlag == E_KEY_DELAY_START_6S)
            g_RfData.CtrlCmd = CMD_REL_UP_2SEC;
        else if (g_Keyboard.KeyHoldFlag == E_KEY_DELAY_START_10S)
            g_RfData.CtrlCmd = CMD_REL_UP_6SEC;
        else
            g_SystermFlag.Sys.RfSend = 0;
        break;
    case KEYBEHIND:
        g_RfSendCnt = 0;
        g_Keyboard.KeyHoldDelay = 0;
        g_SystermFlag.Sys.RfSend = 1;
        if (g_Keyboard.KeyHoldFlag == E_KEY_DELAY_START_2S)
            g_RfData.CtrlCmd = CMD_REL_DOWN;
        else if (g_Keyboard.KeyHoldFlag == E_KEY_DELAY_START_6S)
            g_RfData.CtrlCmd = CMD_REL_DOWN_2SEC;
        else if (g_Keyboard.KeyHoldFlag == E_KEY_DELAY_START_10S)
            g_RfData.CtrlCmd = CMD_REL_DOWN_6SEC;
        else
            g_SystermFlag.Sys.RfSend = 0;
        break;
    case KEYENTER:
        break;
    case KEYRIGHT:
        g_RfSendCnt = 0;
        g_Keyboard.KeyHoldDelay = 0;
        g_SystermFlag.Sys.RfSend = 1;
        if (g_Keyboard.KeyHoldFlag == E_KEY_DELAY_START_2S)
            g_RfData.CtrlCmd = CMD_REL_SET;
        else
            g_SystermFlag.Sys.RfSend = 0;
        break;
    case KEYLEFT:
        break;

    default:
        break;
    }
}


