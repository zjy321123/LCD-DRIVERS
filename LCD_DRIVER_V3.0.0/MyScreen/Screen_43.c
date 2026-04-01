#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
uint8_t Scre43Cnt = 0, Scre43NOasync;
void Screen_43_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre43Cnt == 0)
            Scre43Cnt = 1;
        else if (Scre43Cnt == 1)
            Scre43Cnt = 0;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre43Cnt == 0)
            Scre43Cnt = 1;
        else if (Scre43Cnt == 1)
            Scre43Cnt = 0;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre43Cnt == 0)
        {
            lv_event_send(guider_ui.screen_43_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_43;
            return;
        }
        else if (Scre43Cnt == 1)
        {
            lv_event_send(guider_ui.screen_43_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_43;
            return;
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {

        lv_event_send(guider_ui.screen_43_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_42
        current_screen = SCREEN_42;
        return;
    }
    Screen43date_preloading();
}

void Screen43date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_43_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_43_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre43Cnt == 0)
        lv_event_send(guider_ui.screen_43_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Scre43Cnt == 1)
        lv_event_send(guider_ui.screen_43_btn_2, LV_EVENT_CLICKED, NULL);
}

// 回调函数：在 LVGL 主线程里执行 UI 切换
static void switch_to_screen43_cb(void *param)
{
    LV_UNUSED(param);

    lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_DEFOCUSED, NULL); // 触发事件
    current_screen = SCREEN_44;
    Screen44date_preloading(); // 界面数据预加载

    // 强制刷新
    lv_obj_invalidate(lv_scr_act());
}

void Screen43_AutoLoadProcess(void)
{
    if (Pre_screen == SCREEN_43)
    {
        Autotick++;
        if (Autotick ==(SCREEN_43AutoLoadtime / 3*2+5))
        {
            if (Scre43Cnt == 0)
                keyboard_SendCmdTime(CMD_TURNON_LED, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 开灯
            else if (Scre43Cnt == 1)
                keyboard_SendCmdTime(CMD_TURNOFF_LED, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 关灯
            g_SystermFlag.Sys.RfSendLast = 0;
        }
        if ((Autotick > SCREEN_43AutoLoadtime) && (Scre43NOasync == 0))
        {
            Pre_screen = SCREEN_NUll;
            Autotick = 0;
            // 用异步回调保证 UI 操作在 LVGL 主循环执行
            lv_async_call(switch_to_screen43_cb, NULL);
            return;
        }
    }
}
