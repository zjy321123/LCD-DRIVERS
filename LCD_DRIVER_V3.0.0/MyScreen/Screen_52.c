#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen, Pre_screen;
uint8_t Index_Scr52 = 0, Scre52NOasync;
extern uint16_t Autotick;
void Screen_52_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG)
    {
        if (Index_Scr52 < 4)
            Index_Scr52++;
    }
    else if (Keyid == KEYFORWARD_TRG)
    {
        if (Index_Scr52 > 0)
            Index_Scr52--;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_52_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_51
        current_screen = SCREEN_51;
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Index_Scr52 == 0)
        {
            lv_event_send(guider_ui.screen_52_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_52;
            return;
        }
        else if (Index_Scr52 == 1)
        {
            lv_event_send(guider_ui.screen_52_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_52;
            return;
        }
        else if (Index_Scr52 == 2)
        {
            lv_event_send(guider_ui.screen_52_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_52;
            return;
        }
        else if (Index_Scr52 == 3)
        {
            lv_event_send(guider_ui.screen_52_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_52;
            return;
        }
        else if (Index_Scr52 == 4)
        {
            lv_event_send(guider_ui.screen_52_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_52;
            return;
        }
    }
    Screen52date_preloading();
}

void Screen52date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_52_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_52_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_52_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_52_btn_4, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_52_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Index_Scr52 == 0)
        lv_event_send(guider_ui.screen_52_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr52 == 1)
        lv_event_send(guider_ui.screen_52_btn_2, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr52 == 2)
        lv_event_send(guider_ui.screen_52_btn_3, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr52 == 3)
        lv_event_send(guider_ui.screen_52_btn_4, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr52 == 4)
        lv_event_send(guider_ui.screen_52_btn_5, LV_EVENT_CLICKED, NULL);
}

// 回调函数：在 LVGL 主线程里执行 UI 切换
static void switch_to_screen52_cb(void *param)
{
    LV_UNUSED(param);
    lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_KEY, NULL); // 加载界面screen_53
    current_screen = SCREEN_53;
    Screen53date_preloading(); // 界面数据预加载

    // 强制刷新
    lv_obj_invalidate(lv_scr_act());
}

void Screen52_AutoLoadProcess(void)
{
    if (Pre_screen == SCREEN_52)
    {
        Autotick++;
        if (Autotick == (SCREEN_52AutoLoadtime / 3 * 2 + 5))
        {
            if (Index_Scr52 == 0)
                keyboard_SendCmdTime(CMD_ONSTACLE_ONE, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻一档灵敏度
            else if (Index_Scr52 == 1)
                keyboard_SendCmdTime(CMD_ONSTACLE_TWO, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻二档灵敏度
            else if (Index_Scr52 == 2)
                keyboard_SendCmdTime(CMD_ONSTACLE_THREE, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻三档灵敏度
            else if (Index_Scr52 == 3)
                keyboard_SendCmdTime(CMD_ONSTACLE_FOUR, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻四档灵敏度
            else if (Index_Scr52 == 4)
                keyboard_SendCmdTime(CMD_ONSTACLE_FIVE, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻五档灵敏度
            g_SystermFlag.Sys.RfSendLast = 0;
        }
        if ((Autotick > SCREEN_52AutoLoadtime) && (Scre52NOasync == 0))
        {
            Pre_screen = SCREEN_NUll;
            Autotick = 0;
            // 用异步回调保证 UI 操作在 LVGL 主循环执行
            lv_async_call(switch_to_screen52_cb, NULL);
            return;
        }
    }
}
