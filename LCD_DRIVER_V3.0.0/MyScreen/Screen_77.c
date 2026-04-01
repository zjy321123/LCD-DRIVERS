#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen, Pre_screen, Mem_Screen;
extern uint16_t Autotick;
 uint8_t Scre77Cnt = 0;
void Screen_77_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre77Cnt == 0)
            Scre77Cnt = 1;
        else if (Scre77Cnt == 1)
            Scre77Cnt = 0;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre77Cnt == 0)
            Scre77Cnt = 1;
        else if (Scre77Cnt == 1)
            Scre77Cnt = 0;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_77_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_76
        current_screen = SCREEN_76;
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre77Cnt == 0)
        {
            lv_event_send(guider_ui.screen_77_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7(加载界面)
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_77;
            Mem_Screen = SCREEN_77; // 有行程则删除后回到，这里重新去设置行程
            return;
        }
        else if (Scre77Cnt == 1)
        {
            lv_event_send(guider_ui.screen_77_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7(加载界面)
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_77;
            return;
        }
    }
    Screen77date_preloading();
}

void Screen77date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_77_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_77_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Scre77Cnt == 0)
        lv_event_send(guider_ui.screen_77_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Scre77Cnt == 1)
        lv_event_send(guider_ui.screen_77_btn_2, LV_EVENT_CLICKED, NULL);
}

// 回调函数：在 LVGL 主线程里执行 UI 切换
static void switch_to_screen77_cb(void *param)
{
    LV_UNUSED(param);
    lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_PRESSED, NULL); // 触发事件
    current_screen = SCREEN_15;
    Screen15date_preloading();
    // 强制刷新
    lv_obj_invalidate(lv_scr_act());
}
void Screen77_AutoLoadProcess(void)
{

    if (Pre_screen == SCREEN_77)
    {
        Autotick++;
        if (Autotick == (SCREEN_77AutoLoadtime / 3))
        {
            if (Scre77Cnt == 0)
                keyboard_SendCmdTime(CMD_SET_UP_6SEC, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 删除上行程
            else if (Scre77Cnt == 1)
                keyboard_SendCmdTime(CMD_SET_UP_2SEC, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 设置上行程
            g_SystermFlag.Sys.RfSendLast = 0;
        }
        if (Autotick > SCREEN_77AutoLoadtime)
        {
            Pre_screen = SCREEN_NUll;
            Autotick = 0;
            lv_async_call(switch_to_screen77_cb, NULL); // 切换回界面11

            return;
        }
    }
}



