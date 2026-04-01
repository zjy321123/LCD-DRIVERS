#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen, Pre_screen;
uint8_t Scre49Cnt = 0,Scre49NOasync;
extern uint16_t Autotick;
void Screen_49_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre49Cnt < 2)
            Scre49Cnt++;
    }
    else if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre49Cnt > 0)
            Scre49Cnt--;
    }
    if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_49_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
        current_screen = SCREEN_48;
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre49Cnt == 0)
        {
            lv_event_send(guider_ui.screen_49_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_49;
            return;
        }
        else if (Scre49Cnt == 1)
        {
            lv_event_send(guider_ui.screen_49_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_49;
            return;
        }
        else if (Scre49Cnt == 2)
        {
            lv_event_send(guider_ui.screen_49_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_49;
            return;
        }
    }
    Screen49date_preloading();
}

void Screen49date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_49_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_49_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_49_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre49Cnt == 0)
        lv_event_send(guider_ui.screen_49_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Scre49Cnt == 1)
        lv_event_send(guider_ui.screen_49_btn_2, LV_EVENT_CLICKED, NULL);
    else if (Scre49Cnt == 2)
        lv_event_send(guider_ui.screen_49_btn_3, LV_EVENT_CLICKED, NULL);
}

// 回调函数：在 LVGL 主线程里执行 UI 切换
static void switch_to_screen49_cb(void *param)
{
    LV_UNUSED(param);

    lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_HIT_TEST, NULL); // 加载界面screen_50
    current_screen = SCREEN_50;
    Screen50date_preloading(); // 界面数据预加载

    // 强制刷新
    lv_obj_invalidate(lv_scr_act());
}

void Screen49_AutoLoadProcess(void)
{
    if (Pre_screen == SCREEN_49)
    {
        Autotick++;
        if (Autotick ==(SCREEN_49AutoLoadtime / 3*2+5))
        {
            if (Scre49Cnt == 0)
                keyboard_SendCmdTime(CMD_ONSTACLE_STOPMODE, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻停
            else if (Scre49Cnt == 1)
                keyboard_SendCmdTime(CMD_ONSTACLE_REBOUNDMODE, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻回弹
            else if (Scre49Cnt == 2)
                keyboard_SendCmdTime(CMD_ONSTACLE_UPMODE, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻上升
            g_SystermFlag.Sys.RfSendLast = 0;
        }
        if ((Autotick > SCREEN_46AutoLoadtime)&&(Scre49NOasync == 0))
        {
            Pre_screen = SCREEN_NUll;
            Autotick = 0;
            // 用异步回调保证 UI 操作在 LVGL 主循环执行
            lv_async_call(switch_to_screen49_cb, NULL);
            return;
        }
    }
}
