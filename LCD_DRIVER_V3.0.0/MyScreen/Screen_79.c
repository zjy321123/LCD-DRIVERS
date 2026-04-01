#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen, Pre_screen, Mem_Screen;
extern uint16_t Autotick;
uint8_t Scre79Cnt = 0;
void Screen_79_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre79Cnt == 0)
            Scre79Cnt = 1;
        else if (Scre79Cnt == 1)
            Scre79Cnt = 0;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre79Cnt == 0)
            Scre79Cnt = 1;
        else if (Scre79Cnt == 1)
            Scre79Cnt = 0;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_79_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_78
        current_screen = SCREEN_78;
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre79Cnt == 0)
        {
            lv_event_send(guider_ui.screen_79_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7(加载界面)
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_79;
            Mem_Screen = SCREEN_79; // 有行程则删除后回到，这里重新去设置行程
            return;
        }
        else if (Scre79Cnt == 1)
        {
            lv_event_send(guider_ui.screen_79_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7(加载界面)
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_79;
            return;
        }
    }
    Screen79date_preloading();
}

void Screen79date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_79_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_79_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre79Cnt == 0)
        lv_event_send(guider_ui.screen_79_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Scre79Cnt == 1)
        lv_event_send(guider_ui.screen_79_btn_2, LV_EVENT_CLICKED, NULL);
}

// 回调函数：在 LVGL 主线程里执行 UI 切换
static void switch_to_screen79_cb(void *param)
{
    LV_UNUSED(param);
    lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_PRESSING, NULL); // 触发事件
    current_screen = SCREEN_17;
    Screen17date_preloading();
    // 强制刷新
    lv_obj_invalidate(lv_scr_act());
}
void Screen79_AutoLoadProcess(void)
{

    if (Pre_screen == SCREEN_79)
    {
        Autotick++;
        if (Autotick == (SCREEN_79AutoLoadtime / 3))
        {
            if (Scre79Cnt == 0)
                keyboard_SendCmdTime(CMD_SET_DOWN_6SEC, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 删除下行程
            else if (Scre79Cnt == 1)
                keyboard_SendCmdTime(CMD_SET_DOWN_2SEC, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 设置下行程
            g_SystermFlag.Sys.RfSendLast = 0;
        }
        if (Autotick > SCREEN_79AutoLoadtime)
        {
            Pre_screen = SCREEN_NUll;
            Autotick = 0;
            lv_async_call(switch_to_screen79_cb, NULL); // 切换回界面17
            return;
        }
    }
}
