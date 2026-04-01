#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
extern U_SYS g_SystermFlag;
extern uint8_t Index_Scr81;
static uint8_t Scre13Cnt = 0;
void Screen_13_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre13Cnt == 0)
            Scre13Cnt = 1;
        else if (Scre13Cnt == 1)
            Scre13Cnt = 0;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre13Cnt == 0)
            Scre13Cnt = 1;
        else if (Scre13Cnt == 1)
            Scre13Cnt = 0;
    }
    else if (Keyid == KEYENTER_TRG)
    {

        if (Scre13Cnt == 0)
        {
            lv_event_send(guider_ui.screen_13_btn_2, LV_EVENT_PRESSED, NULL); // 加载界面screen_81
            current_screen = SCREEN_81;
            Index_Scr81 = 0;
            Screen81date_preloading();
            return;
        }
        else if (Scre13Cnt == 1)
        {
            lv_event_send(guider_ui.screen_13_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7(加载界面)
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_13;
            return;
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_13_btn_3, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_80
        current_screen = SCREEN_80;
        return;
    }
    Screen13date_preloading();
}

void Screen13date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_13_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_13_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre13Cnt == 0)
        lv_event_send(guider_ui.screen_13_btn_2, LV_EVENT_CLICKED, NULL);
    else if (Scre13Cnt == 1)
        lv_event_send(guider_ui.screen_13_btn_3, LV_EVENT_CLICKED, NULL);
}
// 回调函数：在 LVGL 主线程里执行 UI 切换
static void switch_to_screen13_cb(void *param)
{
    LV_UNUSED(param);
    lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_SHORT_CLICKED, NULL); // 触发事件
    current_screen = SCREEN_11;
    // 强制刷新
    lv_obj_invalidate(lv_scr_act());
}
void Screen13_AutoLoadProcess(void)
{

    if (Pre_screen == SCREEN_13)
    {
        Autotick++;
        if (Autotick == (SCREEN_13AutoLoadtime / 3))
        {
            if (Scre13Cnt == 0)
                ; // 有行程换向 条到Screen81操作
            else if (Scre13Cnt == 1)
                keyboard_SendCmdTime(CMD_SET_STOP_2SEC, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 无行程换向
            g_SystermFlag.Sys.RfSendLast = 0;
        }
        if (Autotick > SCREEN_13AutoLoadtime)
        {
            Pre_screen = SCREEN_NUll;
            Autotick = 0;
            lv_async_call(switch_to_screen13_cb, NULL); // 切换回界面11

            return;
        }
    }
}
