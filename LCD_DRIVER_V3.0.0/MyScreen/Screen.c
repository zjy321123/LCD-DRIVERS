#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
#include "Customdate.h"
extern lv_indev_t *indev_keypad;
extern lv_ui guider_ui; // 声明 界面对象
uint8_t Shades = 1;
extern unsigned char g_Channel;
extern uint8_t MenuCnt;
extern current_screen_t current_screen;
extern uint16_t Autotick;
// 添加一些全局变量来跟踪当前显示状态
uint8_t current_mode = 0;       // 0: CustomShadeDates, 1: CustomGroupDates
uint8_t current_index = 0;      // 当前显示的项目索引
uint8_t Cachecurrent_mode = 0;  //
uint8_t Cachecurrent_index = 0; //
static uint8_t Keysent_Event = 0;
typedef enum
{
    NUll_EVENT,
    FORWARD_EVENT,
    ENTER_EVENT,
    BEHIND_EVENT,
    RETURN_EVENT,
    FORWARD2S_EVENT,
    BEHIND2S_EVENT,
    RETURN2S_EVENT,
} Key_Event;

void Screen_init(void)
{
    Screendate_preloading();
}
void Screen_Movie(unsigned int Keyid)
{
    uint8_t shade_count = 0; // CustomShadeDates中的项目数
    uint8_t group_count = 0; // CustomGroupDates中的项目数

    // 计算有效项目数量
    for (uint8_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (CustomShadeDates[i].ThemeShow)
        {
            shade_count++;
        }
        if (CustomGroupDates[i].ThemeShow)
        {
            group_count++;
        }
    }

    if (Keyid == KEYRIGHT_TRG)
    {
        // 向右切换到下一个项目
        if (current_mode == 0)
        {
            // 当前显示CustomShadeDates
            current_index++;
            if (current_index >= shade_count)
            {
                // 如果到达CustomShadeDates末尾，切换到CustomGroupDates
                current_mode = 1;
                current_index = 0;
            }
        }
        else
        {
            // 当前显示CustomGroupDates
            current_index++;
            if (current_index >= group_count)
            {
                // 如果到达CustomGroupDates末尾，切换回CustomShadeDates
                current_mode = 0;
                current_index = 0;
            }
        }
        Keysent_Event = NUll_EVENT;
    }
    else if (Keyid == KEYLEFT_TRG)
    {
        // 向左切换到上一个项目
        if (current_mode == 0)
        {
            // 当前显示CustomShadeDates
            if (current_index > 0)
            {
                current_index--;
            }
            else
            {
                // 如果在CustomShadeDates开头，切换到CustomGroupDates末尾
                current_mode = 1;
                current_index = (group_count > 0) ? group_count - 1 : 0;
            }
        }
        else
        {
            // 当前显示CustomGroupDates
            if (current_index > 0)
            {
                current_index--;
            }
            else
            {
                // 如果在CustomGroupDates开头，切换到CustomShadeDates末尾
                current_mode = 0;
                current_index = (shade_count > 0) ? shade_count - 1 : 0;
            }
        }
        Keysent_Event = NUll_EVENT;
    }
    else if (Keyid == KEYFORWARD_TRG)
    {
        keyboard_SendCmdTime(CMD_UP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        Keysent_Event = FORWARD_EVENT;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        keyboard_SendCmdTime(CMD_DOWN, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        Keysent_Event = BEHIND_EVENT;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        keyboard_SendCmdTime(CMD_STOP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        Keysent_Event = ENTER_EVENT;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        keyboard_SendCmdTime(CMD_PRG, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        Keysent_Event = RETURN_EVENT;
    }
    else if (Keyid == RETURN_2S)
    {
        keyboard_SendCmdTime(CMD_SET, KEY_DELAY_4SEC, E_KEY_DELAY_START_6S);
        Keysent_Event = RETURN2S_EVENT;
    }
    else if (Keyid == KEYFORWARD_2S)
    {
        keyboard_SendCmdTime(CMD_UP_2SEC, KEY_DELAY_4SEC, E_KEY_DELAY_START_6S);
        Keysent_Event = FORWARD2S_EVENT;
    }
    else if (Keyid == KEYBEHIND_2S)
    {
        keyboard_SendCmdTime(CMD_DOWN_2SEC, KEY_DELAY_4SEC, E_KEY_DELAY_START_6S);
        Keysent_Event = BEHIND2S_EVENT;
    }
    else if (Keyid == KEYMENU_TRG)
    {
        Keysent_Event = NUll_EVENT;
        lv_event_send(guider_ui.screen_label_6, LV_EVENT_CLICKED, NULL); // 加载界面screen_1
        current_screen = SCREEN_1;
        Screen1date_preloading();
        return;
    }

    Screendate_preloading();
}

void Screendate_preloading(void)
{
    uint8_t real_index = 0;
    char display_text[STRING_SIZE + 10] = {0}; // 足够存放文本和额外信息

    if (current_mode == 0)
    {
        // 显示CustomShadeDates
        // 找到第current_index个ThemeShow为1的项目
        uint8_t count = 0;
        for (uint8_t i = 0; i < ARRAY_SIZE; i++)
        {
            if (CustomShadeDates[i].ThemeShow)
            {
                if (count == current_index)
                {
                    real_index = i;
                    break;
                }
                count++;
            }
        }

        // 显示通道信息
        // g_Channel = real_index; // 更新当前通道
        strncpy(display_text, (char *)CustomShadeDates[real_index].DataArray, STRING_SIZE);
        lv_label_set_text(guider_ui.screen_label_2, display_text);

        // 计算总数并显示
        uint8_t total_shades = 0;
        for (uint8_t i = 0; i < ARRAY_SIZE; i++)
        {
            if (CustomShadeDates[i].ThemeShow)
            {
                total_shades++;
            }
        }
        lv_label_set_text_fmt(guider_ui.screen_label_5, "Ch %d /%d", current_index + 1, total_shades);
        lv_label_set_text(guider_ui.screen_label_1, "Shades");
        lv_led_set_color(guider_ui.screen_led_1, lv_color_hex(0x71FF9B));
    }
    else
    {
        // 显示CustomGroupDates
        // 找到第current_index个ThemeShow为1的项目
        uint8_t count = 0;
        for (uint8_t i = 0; i < ARRAY_SIZE; i++)
        {
            if (CustomGroupDates[i].ThemeShow)
            {
                if (count == current_index)
                {
                    real_index = i;
                    break;
                }
                count++;
            }
        }

        // 显示群组信息
        // g_Channel = real_index; // 更新当前通道为群组索引
        strncpy(display_text, (char *)CustomGroupDates[real_index].DataArray, STRING_SIZE);
        lv_label_set_text(guider_ui.screen_label_2, display_text);

        // 计算总数并显示
        uint8_t total_groups = 0;
        for (uint8_t i = 0; i < ARRAY_SIZE; i++)
        {
            if (CustomGroupDates[i].ThemeShow)
            {
                total_groups++;
            }
        }
        lv_label_set_text_fmt(guider_ui.screen_label_5, "Grp %d/%d", current_index + 1, total_groups);
        lv_label_set_text(guider_ui.screen_label_1, "Groups");
        lv_led_set_color(guider_ui.screen_led_1, lv_color_hex(0xB06016));
    }
    if (Keysent_Event == NUll_EVENT)
    {
        lv_obj_add_flag(guider_ui.screen_label_31, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_spinner_1, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        Autotick = 0;
        lv_obj_clear_flag(guider_ui.screen_label_31, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_spinner_1, LV_OBJ_FLAG_HIDDEN);
        switch (Keysent_Event)
        {
        case FORWARD_EVENT:
            lv_label_set_text(guider_ui.screen_label_31, "Opening...");
            break;
        case ENTER_EVENT:
            lv_label_set_text(guider_ui.screen_label_31, "Stoping...");
            break;
        case BEHIND_EVENT:
            lv_label_set_text(guider_ui.screen_label_31, "Closeing...");
            break;
        case RETURN_EVENT:
            lv_label_set_text(guider_ui.screen_label_31, "Pairing...");
            break;
        case FORWARD2S_EVENT:
            lv_label_set_text(guider_ui.screen_label_31, "HoldOpening...");
            break;
        case BEHIND2S_EVENT:
            lv_label_set_text(guider_ui.screen_label_31, "HoldClosing...");
            break;
        case RETURN2S_EVENT:
            lv_label_set_text(guider_ui.screen_label_31, "Faving...");
            break;
        default:
            break;
        }
    }
}
void Screen_KeyRelease(unsigned int Keyid)
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
    case KEYRETURN:
        g_RfSendCnt = 0;
        g_Keyboard.KeyHoldDelay = 0;
        g_SystermFlag.Sys.RfSend = 1;
        if (g_Keyboard.KeyHoldFlag == E_KEY_DELAY_START_6S)
            g_RfData.CtrlCmd = CMD_REL_SET;
        // else if (g_Keyboard.KeyHoldFlag == E_KEY_DELAY_START_6S)
        //     g_RfData.CtrlCmd = CMD_REL_DOWN_2SEC;
        // else if (g_Keyboard.KeyHoldFlag == E_KEY_DELAY_START_10S)
        //     g_RfData.CtrlCmd = CMD_REL_DOWN_6SEC;
        else
            g_SystermFlag.Sys.RfSend = 0;
        break;
    case KEYENTER:
        break;
    default:
        break;
    }
}
// 定义一个回调函数，用于隐藏控件
static void hide_objs_cb(void *param)
{
    LV_UNUSED(param);

    if (guider_ui.screen_label_31 != NULL)
        lv_obj_add_flag(guider_ui.screen_label_31, LV_OBJ_FLAG_HIDDEN);

    if (guider_ui.screen_spinner_1 != NULL)
        lv_obj_add_flag(guider_ui.screen_spinner_1, LV_OBJ_FLAG_HIDDEN);

    // 强制刷新当前屏幕，确保立即生效
    lv_obj_invalidate(lv_scr_act());
}
void Screen_AutoLoadProcess(void)
{
    if (current_screen == SCREEN)
    {

        if (Keysent_Event == NUll_EVENT)
            return;
        if (Autotick < SCREEN_AutoLoadtime)
        {
            Autotick++;
            if (Autotick == SCREEN_AutoLoadtime - 10)
            {
                Autotick = SCREEN_AutoLoadtime + 1;
                Keysent_Event = NUll_EVENT;
                // 使用异步回调方式，让 LVGL 主循环来隐藏控件
                lv_async_call(hide_objs_cb, NULL);
                return;
            }
        }
    }
}
