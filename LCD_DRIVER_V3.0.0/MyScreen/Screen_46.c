#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
uint8_t Scre46NOasync;
void Screen_46_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {

    }
    else if (Keyid == KEYBEHIND_TRG)
    {

    }
    else if (Keyid == KEYENTER_TRG)
    {
        lv_event_send(guider_ui.screen_46_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7
        current_screen = SCREEN_7;
        Pre_screen = SCREEN_46;
        return;
    }
    else if (Keyid == KEYRETURN_TRG)
    {

        lv_event_send(guider_ui.screen_46_btn_1, LV_EVENT_PRESSED, NULL); // 加载界面screen_45
        current_screen = SCREEN_45;
        return;
    }
}



// 回调函数：在 LVGL 主线程里执行 UI 切换
static void switch_to_screen46_cb(void *param)
{
    LV_UNUSED(param);

    lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_LEAVE, NULL); // 触发事件
    current_screen = SCREEN_47;
    Screen47date_preloading(); // 界面数据预加载

    // 强制刷新
    lv_obj_invalidate(lv_scr_act());
}

void Screen46_AutoLoadProcess(void)
{
    if (Pre_screen == SCREEN_46)
    {
        Autotick++;
        if (Autotick == (SCREEN_46AutoLoadtime / 3))
        {
            keyboard_SendCmdTime(CMD_ONSTACLE_SWITCH, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻开关
            g_SystermFlag.Sys.RfSendLast = 0;
        }
        if ((Autotick > SCREEN_46AutoLoadtime)&& (Scre46NOasync ==0))
        {
            Pre_screen = SCREEN_NUll;
            Autotick = 0;
            // 用异步回调保证 UI 操作在 LVGL 主循环执行
            lv_async_call(switch_to_screen46_cb, NULL);
            return;
        }
    }
}
