#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "Si446x.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
#include "Calendar.h"
extern lv_indev_t *indev_keypad;
extern lv_ui guider_ui; // 声明 界面对象
extern uint8_t g_CacheCmd;
extern current_screen_t current_screen;
static uint8_t MenuCnt = 0;
void Screen_1_Movie(unsigned int Keyid)
{

    if (MenuCnt == 0)
    {
        if (Keyid == KEYFORWARD_TRG)
            MenuCnt = 4;
        else if (Keyid == KEYBEHIND_TRG)
            MenuCnt += 2;
        else if (Keyid == KEYLEFT_TRG)
            MenuCnt = 5;
        else if (Keyid == KEYRIGHT_TRG)
            MenuCnt += 1;
    }
    else if (MenuCnt == 1)
    {
        if (Keyid == KEYFORWARD_TRG)
            MenuCnt = 5;
        else if (Keyid == KEYBEHIND_TRG)
            MenuCnt += 2;
        else if (Keyid == KEYLEFT_TRG)
            MenuCnt -= 1;
        else if (Keyid == KEYRIGHT_TRG)
            MenuCnt += 1;
    }
    else if (MenuCnt == 2)
    {
        if (Keyid == KEYFORWARD_TRG)
            MenuCnt -= 2;
        else if (Keyid == KEYBEHIND_TRG)
            MenuCnt += 2;
        else if (Keyid == KEYLEFT_TRG)
            MenuCnt -= 1;
        else if (Keyid == KEYRIGHT_TRG)
            MenuCnt += 1;
    }
    else if (MenuCnt == 3)
    {
        if (Keyid == KEYFORWARD_TRG)
            MenuCnt -= 2;
        else if (Keyid == KEYBEHIND_TRG)
            MenuCnt += 2;
        else if (Keyid == KEYLEFT_TRG)
            MenuCnt -= 1;
        else if (Keyid == KEYRIGHT_TRG)
            MenuCnt += 1;
    }
    else if (MenuCnt == 4)
    {
        if (Keyid == KEYFORWARD_TRG)
            MenuCnt -= 2;
        else if (Keyid == KEYBEHIND_TRG)
            MenuCnt = 0;
        else if (Keyid == KEYLEFT_TRG)
            MenuCnt -= 1;
        else if (Keyid == KEYRIGHT_TRG)
            MenuCnt += 1;
    }
    else if (MenuCnt == 5)
    {
        if (Keyid == KEYFORWARD_TRG)
            MenuCnt -= 2;
        else if (Keyid == KEYBEHIND_TRG)
            MenuCnt = 0;
        else if (Keyid == KEYLEFT_TRG)
            MenuCnt -= 1;
        else if (Keyid == KEYRIGHT_TRG)
            MenuCnt = 0;
    }
    if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_1_label_20, LV_EVENT_CLICKED, NULL); // 加载界面screen
        current_screen = SCREEN;
        Screendate_preloading();
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        switch (MenuCnt + 1)
        {
        case 1:
            lv_event_send(guider_ui.screen_1_label_2, LV_EVENT_CLICKED, NULL); // 加载界面screen_2
            current_screen = SCREEN_2;
            Screen2date_preloading();
            return;
        case 2:
            lv_event_send(guider_ui.screen_1_label_3, LV_EVENT_CLICKED, NULL); // 加载界面screen_57
            current_screen = SCREEN_57;
            Screen57date_preloading();
            return;
        case 3:
            lv_event_send(guider_ui.screen_1_label_5, LV_EVENT_CLICKED, NULL); // 加载界面screen_61
            current_screen = SCREEN_61;
            Screen61date_preloading();
            return;
        case 4:
            lv_event_send(guider_ui.screen_1_label_4, LV_EVENT_CLICKED, NULL); // 加载界面screen_68
            current_screen = SCREEN_68;
            Screen68date_preloading();
            return;
        case 5:
            lv_event_send(guider_ui.screen_1_label_7, LV_EVENT_CLICKED, NULL); // 加载界面screen_74
            current_screen = SCREEN_74;
            Screen74date_preloading();
            return;
        case 6:
            lv_event_send(guider_ui.screen_1_label_6, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_75
            current_screen = SCREEN_75;
            return;
        default:
            break;
        }
    }
    Screen1date_preloading();
}
void Screen1date_preloading(void)
{

    lv_obj_set_style_border_color(guider_ui.screen_1_label_9, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(guider_ui.screen_1_label_10, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(guider_ui.screen_1_label_11, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(guider_ui.screen_1_label_12, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(guider_ui.screen_1_label_13, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(guider_ui.screen_1_label_14, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    switch (MenuCnt + 1)
    {
    case 1:
        lv_event_send(guider_ui.screen_1_label_9, LV_EVENT_CLICKED, NULL);
        break;
    case 2:
        lv_event_send(guider_ui.screen_1_label_10, LV_EVENT_CLICKED, NULL);
        break;
    case 3:
        lv_event_send(guider_ui.screen_1_label_11, LV_EVENT_CLICKED, NULL);
        break;
    case 4:
        lv_event_send(guider_ui.screen_1_label_12, LV_EVENT_CLICKED, NULL);
        break;
    case 5:
        lv_event_send(guider_ui.screen_1_label_14, LV_EVENT_CLICKED, NULL);
        break;
    case 6:
        lv_event_send(guider_ui.screen_1_label_13, LV_EVENT_CLICKED, NULL);
        break;
    default:
        break;
    }
}

void Screen_1_KeyRelease(unsigned int Keyid)
{
    //    // lv_obj_t *current_screen = lv_scr_act();
    //     if (current_screen != guider_ui.screen_1)
    //         return;

    switch (g_Keyboard.KeyRelease)
    {
    case KEYFORWARD:

        break;
    case KEYBEHIND:

        break;
    case KEYENTER:
        // g_RfSendCnt = 0;
        // g_Keyboard.KeyHoldDelay = 0;
        // g_SystermFlag.Sys.RfSend = 1;
        // if (g_Keyboard.KeyHoldFlag == E_KEY_DELAY_START_2S)
        // {
        //     if (g_CacheCmd == CMD_SET_UP_2SEC)
        //         g_RfData.CtrlCmd = CMD_REL_UP_2SEC;
        //     else if (g_CacheCmd == CMD_SET_DOWN_2SEC)
        //         g_RfData.CtrlCmd = CMD_REL_DOWN_2SEC;
        //     else if (g_CacheCmd == CMD_SET)
        //         g_RfData.CtrlCmd = CMD_REL_SET;
        // }

        // else if (g_Keyboard.KeyHoldFlag == E_KEY_DELAY_START_6S)
        //     g_RfData.CtrlCmd = CMD_REL_DOWN_2SEC;
        // else if (g_Keyboard.KeyHoldFlag == E_KEY_DELAY_START_10S)
        //     g_RfData.CtrlCmd = CMD_REL_DOWN_6SEC;
        // else
        //     g_SystermFlag.Sys.RfSend = 0;
         break;
    default:
        break;
    }
}
