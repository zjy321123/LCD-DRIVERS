#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
#include "Customdate.h"
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
extern U_SYS g_SystermFlag;
extern uint8_t current_index, current_mode,Cachecurrent_index, Cachecurrent_mode;;
static uint8_t Scre6Cnt = 0;
void Screen_6_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre6Cnt == 0)
            Scre6Cnt = 1;
        else if (Scre6Cnt == 1)
            Scre6Cnt = 0;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre6Cnt == 0)
            Scre6Cnt = 1;
        else if (Scre6Cnt == 1)
            Scre6Cnt = 0;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_6_label_2, LV_EVENT_CLICKED, NULL); // 加载界面screen_5
        current_screen = SCREEN_5;
        Screen5date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre6Cnt == 0)
        {
            lv_event_send(guider_ui.screen_6_btn_6, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_6;
            return;
        }
        else if (Scre6Cnt == 1)
        {
            lv_event_send(guider_ui.screen_6_btn_5, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_5
            current_screen = SCREEN_5;
            Screen5date_preloading();
            return;
        }
    }
    Screen6date_preloading();
}

void Screen6date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_6_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_6_btn_6, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre6Cnt == 0)
        lv_event_send(guider_ui.screen_6_btn_6, LV_EVENT_CLICKED, NULL);
    else if (Scre6Cnt == 1)
        lv_event_send(guider_ui.screen_6_btn_5, LV_EVENT_CLICKED, NULL);
}

// 回调函数：在 LVGL 主线程里执行 UI 切换
static void switch_to_screen6_cb(void *param)
{
    LV_UNUSED(param);

    lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_CLICKED, NULL); // 触发事件
    current_screen = SCREEN_8;
    Screen8date_preloading(); // 界面数据预加载

    // 强制刷新
    lv_obj_invalidate(lv_scr_act());
}

void Screen6_AutoLoadProcess(void)
{
    if (Pre_screen == SCREEN_6)
    {
        Autotick++;
        if (Autotick == (SCREEN_6AutoLoadtime / 3))
        {
            keyboard_SendCmdTime(CMD_PRG, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 对码
            Cachecurrent_index = current_index;
            Cachecurrent_mode = current_mode;
            current_index = FindShadeEmptyIndex();
            current_mode = 0;
            g_SystermFlag.Sys.RfSendLast = 0;
        }
        if (Autotick > SCREEN_6AutoLoadtime)
        {
            Pre_screen = SCREEN_NUll;
            Autotick = 0;
            // 用异步回调保证 UI 操作在 LVGL 主循环执行
            lv_async_call(switch_to_screen6_cb, NULL);
            return;
        }
    }
}
