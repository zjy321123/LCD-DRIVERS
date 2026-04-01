#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "FskComm.h"
#include "FskProcess.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
static uint8_t Scre29Cnt = 0;
static uint8_t Pct_Level = 0;
static uint8_t IsClick = 0;
static uint8_t CacheClick = 0;
void Screen_29_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG)
    {
        if (Scre29Cnt < 2)
            Scre29Cnt++;
    }
    else if (Keyid == KEYFORWARD_TRG)
    {
        if (Scre29Cnt > 0)
            Scre29Cnt--;
    }
    else if (Keyid == KEYLEFT_TRG)
    {

        if (Pct_Level > 0)
            Pct_Level -= 10;
        if (!IsClick)
            IsClick = 1;
    }
    else if (Keyid == KEYRIGHT_TRG)
    {
        if (Pct_Level < 100)
            Pct_Level += 10;
        if (!IsClick)
            IsClick = 1;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Scre29Cnt == 0)
        {
            if (!IsClick)
                IsClick = 1;
            if (g_MsgSave.RadioMode == RM_OOK)
                rfsend_SendPctData(PCT_0 + Pct_Level / 10);
            else
                Fsk_RadioPercentCmd(&TxBuf, Pct_Level / 10);
            g_SystermFlag.Sys.RfSendLast = 0;
        }
        else if (Scre29Cnt == 1)
        {
            lv_event_send(guider_ui.screen_29_btn_2, LV_EVENT_SHORT_CLICKED, NULL);
            current_screen = SCREEN_20;
            Screen20date_preloading();
            return;
        }
    }
    CacheClick = Keyid;
    Screen29date_preloading();
}

void Screen29date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_29_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_29_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_bar_set_value(guider_ui.screen_29_bar_1, Pct_Level, LV_ANIM_OFF);
    lv_label_set_text_fmt(guider_ui.screen_29_label_5, "%d%%", Pct_Level);

    lv_event_send(guider_ui.screen_29_label_6, LV_EVENT_SHORT_CLICKED, NULL);
    lv_event_send(guider_ui.screen_29_label_7, LV_EVENT_SHORT_CLICKED, NULL);
    if (Scre29Cnt == 0)
        lv_event_send(guider_ui.screen_29_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Scre29Cnt == 1)
        lv_event_send(guider_ui.screen_29_btn_2, LV_EVENT_CLICKED, NULL);
    if (CacheClick == KEYLEFT_TRG)
    {
        lv_event_send(guider_ui.screen_29_label_6, LV_EVENT_CLICKED, NULL);
        // return;
    }
    else if (CacheClick == KEYRIGHT_TRG)
    {
        lv_event_send(guider_ui.screen_29_label_7, LV_EVENT_CLICKED, NULL);
        // return;
    }
    else if ((CacheClick == KEYENTER_TRG) && (Scre29Cnt == 0))
    {
        lv_event_send(guider_ui.screen_29_btn_1, LV_EVENT_SHORT_CLICKED, NULL);
        // return;
    }
}
void Screen29click_show(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_29_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_29_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_bar_set_value(guider_ui.screen_29_bar_1, Pct_Level, LV_ANIM_OFF);
    lv_label_set_text_fmt(guider_ui.screen_29_label_5, "%d%%", Pct_Level);
    lv_event_send(guider_ui.screen_29_label_6, LV_EVENT_SHORT_CLICKED, NULL);
    lv_event_send(guider_ui.screen_29_label_7, LV_EVENT_SHORT_CLICKED, NULL);
    if (Scre29Cnt == 0)
        lv_event_send(guider_ui.screen_29_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Scre29Cnt == 1)
        lv_event_send(guider_ui.screen_29_btn_2, LV_EVENT_CLICKED, NULL);
}
// 回调函数：在 LVGL 主线程里执行 UI 切换
static void switch_to_screen29_cb(void *param)
{
    LV_UNUSED(param);
    Screen29click_show();
    CacheClick = 0;
    lv_obj_invalidate(lv_scr_act());
}

void Screen29_AutoLoadProcess(void)
{
    if ((current_screen == SCREEN_29) && IsClick)
    {
        Autotick++;
        if (Autotick == SCREEN_29AutoLoadtime)
        {
            IsClick = Autotick = 0;
            // 用异步回调保证 UI 操作在 LVGL 主循环执行
            lv_async_call(switch_to_screen29_cb, NULL);
            return;
        }
    }
}



