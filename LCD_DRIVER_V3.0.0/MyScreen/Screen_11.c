#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen;
void Screen_11_Movie(unsigned int Keyid)
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
    else if (Keyid == KEYFORWARD_2S)
    {
        keyboard_SendCmdTime(CMD_UP_2SEC, KEY_DELAY_4SEC, E_KEY_DELAY_START_6S);
    }
    else if (Keyid == KEYBEHIND_2S)
    {
        keyboard_SendCmdTime(CMD_DOWN_2SEC, KEY_DELAY_4SEC, E_KEY_DELAY_START_6S);
    }
    else if (Keyid == KEYENTER_2S)
    {
        lv_event_send(guider_ui.screen_11_btn_2, LV_EVENT_CLICKED, NULL); // 加载界面screen_12
        current_screen = SCREEN_12;
        Screen12date_preloading();
        return;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_11_label_3, LV_EVENT_CLICKED, NULL); // 加载界面screen_10
        current_screen = SCREEN_10;
        Screen10date_preloading();
        return;
    }
}
void Screen_11_KeyRelease(unsigned int Keyid)
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
    default:
        break;
    }
}
