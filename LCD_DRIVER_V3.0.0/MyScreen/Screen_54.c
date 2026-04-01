#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Customdate.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen, Pre_screen;
extern uint8_t ShadePreIndex;
extern uint16_t Autotick;
static uint8_t Index_Scr54 = 0;
void Screen_54_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Index_Scr54 == 0)
            Index_Scr54 = 1;
        else if (Index_Scr54 == 1)
            Index_Scr54 = 0;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Index_Scr54 == 0)
            Index_Scr54 = 1;
        else if (Index_Scr54 == 1)
            Index_Scr54 = 0;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Index_Scr54 == 1)
        {
            lv_event_send(guider_ui.screen_54_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
            current_screen = SCREEN_20;
            Screen20date_preloading();
            return;
        }
    }
    else if (Keyid == KEYENTER_6S)
    {
        if (Index_Scr54 == 0)
        {
            lv_event_send(guider_ui.screen_54_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_7
            current_screen = SCREEN_7;
            Pre_screen = SCREEN_54;
            return;
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_54_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
        current_screen = SCREEN_20;
        Screen20date_preloading();
        return;
    }
    Screen54date_preloading();
}

void Screen54date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_54_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_54_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Index_Scr54 == 0)
        lv_event_send(guider_ui.screen_54_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr54 == 1)
        lv_event_send(guider_ui.screen_54_btn_2, LV_EVENT_CLICKED, NULL);

    char display_text[21]; // 19字节最大需求 + 1
    char *current_data = CustomShadeDates[ShadePreIndex].DataArray;

    strcpy(display_text, "Display ");
    if (strlen(current_data) > 10)
    {
        strncat(display_text, current_data, 10);
        strcat(display_text, "...");
    }
    else
    {
        strcat(display_text, current_data);
    }
    lv_label_set_text(guider_ui.screen_54_label_3, display_text);
}

// 回调函数：在 LVGL 主线程里执行 UI 切换
static void switch_to_screen54_cb(void *param)
{
    LV_UNUSED(param);

    lv_event_send(guider_ui.screen_7_label_6, LV_EVENT_CLICKED, NULL); // 触发事件
    current_screen = SCREEN_55;
    Screen55date_preloading(); // 界面数据预加载

    // 强制刷新
    lv_obj_invalidate(lv_scr_act());
}

void Screen54_AutoLoadProcess(void)
{
    if (Pre_screen == SCREEN_54)
    {
        Autotick++;
        if (Autotick == (SCREEN_54AutoLoadtime / 3))
        {
            keyboard_SendCmdTime(CMD_RESET_ALL, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 对码
            g_SystermFlag.Sys.RfSendLast = 0;
        }
        if (Autotick > SCREEN_54AutoLoadtime)
        {
            Pre_screen = SCREEN_NUll;
            Autotick = 0;
            // 用异步回调保证 UI 操作在 LVGL 主循环执行
            lv_async_call(switch_to_screen54_cb, NULL);
            return;
        }
    }
}
