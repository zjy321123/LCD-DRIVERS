#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "Customdate.h"
#include "Si446x.h"
#include "FskComm.h"
#include "ProtocolCmd.h"
#include "FskProcess.h"
extern lv_indev_t *indev_keypad;
extern lv_ui guider_ui; // 声明 界面对象
static uint8_t Index_Scrlabtest = 0;
extern uint8_t g_CacheCmd;
extern current_screen_t current_screen, Mem_Screen;
static uint8_t OOK_High_Flag = 0, OOK_Sent_falg = 0, FSK_Sent_flag = 0, ColorTest_Flag = 0;
uint8_t S_default_mode = RM_OOK;
void Screen_LabTest_Movie(unsigned int Keyid)
{

    if (Keyid == KEYFORWARD_TRG)
    {
        if (Index_Scrlabtest == 0)
            Index_Scrlabtest = 4;
        else
            Index_Scrlabtest--;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Index_Scrlabtest == 4)
            Index_Scrlabtest = 0;
        else
            Index_Scrlabtest++;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        g_MsgSave.RadioMode = S_default_mode;
        lv_event_send(guider_ui.lab_test_label_7, LV_EVENT_CLICKED, NULL); // 加载界面Screen
        current_screen = SCREEN;
        Screendate_preloading();
        return;
    }

    else if (Keyid == KEYENTER_TRG)
    {

        // 5个选项互斥：当前项可在 0/1 间切换；若切到 1，其它项全部置 0
        switch (Index_Scrlabtest)
        {
        case 0:
            // OOK_High_Flag = (OOK_High_Flag == 0) ? 1 : 0;
            // if (OOK_High_Flag == 1)
            // {
            //     OOK_Sent_falg = 0;
            //     FSK_Sent_flag = 0;
            //     ColorTest_Flag = 0;
            //     g_MsgSave.RadioMode = RM_OOK;
            //     Radio_Rf_Init();
            //     IODATAHIGHT();
            // }
            break;
        case 1:
            OOK_Sent_falg = (OOK_Sent_falg == 0) ? 1 : 0;
            if (OOK_Sent_falg == 1)
            {
                OOK_High_Flag = 0;
                FSK_Sent_flag = 0;
                ColorTest_Flag = 0;
                g_MsgSave.RadioMode = RM_OOK;
                Radio_Rf_Init();
                keyboard_SendCmdTime(CMD_STOP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
            }

            break;
        case 2:
            FSK_Sent_flag = (FSK_Sent_flag == 0) ? 1 : 0;
            if (FSK_Sent_flag == 1)
            {
                OOK_High_Flag = 0;
                OOK_Sent_falg = 0;
                ColorTest_Flag = 0;
                g_MsgSave.RadioMode = RM_FSK;
                Radio_Rf_Init();
            }
            break;
        case 3:
            ColorTest_Flag = (ColorTest_Flag == 0) ? 1 : 0;
            if (ColorTest_Flag == 1)
            {
                OOK_High_Flag = 0;
                OOK_Sent_falg = 0;
                FSK_Sent_flag = 0;
            }
            break;
        case 4:
            // 第5项目前无功能：按下时等同于“全部关闭”
            OOK_High_Flag = 0;
            OOK_Sent_falg = 0;
            FSK_Sent_flag = 0;
            ColorTest_Flag = 0;
            break;
        default:
            break;
        }
    }
    Screen_LabTest_preloading();
}

void Screen_LabTest_preloading(void)
{
    if (ColorTest_Flag == 1)
    {
        // 白色主题
        lv_color_t white = lv_color_hex(0xFFFFFF);
        // lv_color_t white = lv_color_hex(0x000000);

        // Switch 全白（包含 checked 状态）
        lv_obj_t *sws[] = {guider_ui.lab_test_sw_1, guider_ui.lab_test_sw_2, guider_ui.lab_test_sw_3, guider_ui.lab_test_sw_4, guider_ui.lab_test_sw_5};
        for (int i = 0; i < 5; i++)
        {
            lv_obj_set_style_bg_color(sws[i], white, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(sws[i], white, LV_PART_INDICATOR | LV_STATE_CHECKED);
            lv_obj_set_style_bg_color(sws[i], white, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(sws[i], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(sws[i], 0, LV_PART_INDICATOR | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(sws[i], 0, LV_PART_KNOB | LV_STATE_DEFAULT);
        }

        // 屏幕背景
        lv_obj_set_style_bg_color(guider_ui.lab_test, white, LV_PART_MAIN | LV_STATE_DEFAULT);

        // 按钮
        lv_obj_t *btns[] = {guider_ui.lab_test_btn_1, guider_ui.lab_test_btn_2, guider_ui.lab_test_btn_3, guider_ui.lab_test_btn_4, guider_ui.lab_test_btn_5};
        for (int i = 0; i < 5; i++)
        {
            lv_obj_set_style_bg_color(btns[i], white, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(btns[i], white, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(btns[i], white, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(btns[i], 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        // 标签
        lv_obj_set_style_text_color(guider_ui.lab_test_Time_Lable, white, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.lab_test_label_2, white, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.lab_test_label_5, white, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.lab_test_label_7, white, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.lab_test_RF_MODE, white, LV_PART_MAIN | LV_STATE_DEFAULT);

        // 电池图标
        lv_obj_set_style_border_color(guider_ui.lab_test_cont_1, white, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.lab_test_cont_1, white, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_line_color(guider_ui.lab_test_line_1, white, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.lab_test_Batter_Percent, white, LV_PART_INDICATOR | LV_STATE_DEFAULT);

        // 分割线
        lv_obj_set_style_line_color(guider_ui.lab_test_line_2, white, LV_PART_MAIN | LV_STATE_DEFAULT);

				return;
    }
    else
    {
        // 恢复默认深色主题
        lv_obj_set_style_bg_color(guider_ui.lab_test, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

        // Switch 恢复默认
        lv_obj_t *sws[] = {guider_ui.lab_test_sw_1, guider_ui.lab_test_sw_2, guider_ui.lab_test_sw_3, guider_ui.lab_test_sw_4, guider_ui.lab_test_sw_5};
        for (int i = 0; i < 5; i++)
        {
            lv_obj_set_style_bg_color(sws[i], lv_color_hex(0x9BA0AA), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(sws[i], lv_color_hex(0x2FDA64), LV_PART_INDICATOR | LV_STATE_CHECKED);
            lv_obj_set_style_bg_color(sws[i], lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(sws[i], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(sws[i], 0, LV_PART_INDICATOR | LV_STATE_CHECKED);
            lv_obj_set_style_border_width(sws[i], 0, LV_PART_KNOB | LV_STATE_DEFAULT);
        }

        // 按钮
        lv_obj_t *btns[] = {guider_ui.lab_test_btn_1, guider_ui.lab_test_btn_2, guider_ui.lab_test_btn_3, guider_ui.lab_test_btn_4, guider_ui.lab_test_btn_5};
        for (int i = 0; i < 5; i++)
        {
            lv_obj_set_style_bg_color(btns[i], lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(btns[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(btns[i], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        // 标签
        lv_obj_set_style_text_color(guider_ui.lab_test_Time_Lable, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.lab_test_label_2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.lab_test_label_5, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT); // Note: This one was dark on dark
        lv_obj_set_style_text_color(guider_ui.lab_test_label_7, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(guider_ui.lab_test_RF_MODE, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

        // 电池图标
        lv_obj_set_style_border_color(guider_ui.lab_test_cont_1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.lab_test_cont_1, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_line_color(guider_ui.lab_test_line_1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(guider_ui.lab_test_Batter_Percent, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);

        // 分割线
        lv_obj_set_style_line_color(guider_ui.lab_test_line_2, lv_color_hex(0x757575), LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    lv_obj_clear_flag(guider_ui.lab_test_sw_1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(guider_ui.lab_test_sw_2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(guider_ui.lab_test_sw_3, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(guider_ui.lab_test_sw_4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(guider_ui.lab_test_sw_5, LV_OBJ_FLAG_HIDDEN);

    if (g_MsgSave.RadioMode == RM_OOK)
    {
        lv_label_set_text(guider_ui.lab_test_RF_MODE, "OOK");
    }
    else if (g_MsgSave.RadioMode == RM_FSK)
    {
        lv_label_set_text(guider_ui.lab_test_RF_MODE, "FSK");
    }
    if (OOK_High_Flag == 1)
    {
        lv_obj_add_state(guider_ui.lab_test_sw_1, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(guider_ui.lab_test_sw_1, LV_STATE_CHECKED);
    }

    if (OOK_Sent_falg == 1)
    {
        lv_obj_add_state(guider_ui.lab_test_sw_2, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(guider_ui.lab_test_sw_2, LV_STATE_CHECKED);
    }
    if (FSK_Sent_flag == 1)
    {
        lv_obj_add_state(guider_ui.lab_test_sw_3, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(guider_ui.lab_test_sw_3, LV_STATE_CHECKED);
    }
    if (ColorTest_Flag == 1)
    {
        lv_obj_add_state(guider_ui.lab_test_sw_4, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(guider_ui.lab_test_sw_4, LV_STATE_CHECKED);
    }
    // 高亮当前光标位置
    switch (Index_Scrlabtest + 1)
    {
    case 1:
        lv_event_send(guider_ui.lab_test_btn_1, LV_EVENT_CLICKED, NULL);
        break;
    case 2:
        lv_event_send(guider_ui.lab_test_btn_2, LV_EVENT_CLICKED, NULL);
        break;
    case 3:
        lv_event_send(guider_ui.lab_test_btn_3, LV_EVENT_CLICKED, NULL);
        break;
    case 4:
        lv_event_send(guider_ui.lab_test_btn_4, LV_EVENT_CLICKED, NULL);
        break;
    case 5:
        lv_event_send(guider_ui.lab_test_btn_5, LV_EVENT_CLICKED, NULL);
        break;
    default:
        break;
    }
}
void ScreenLabTest_AutoLoadProcess(void)
{
    if (current_screen != SCREEN_LAB_TEST)
    {
        return;
    }
    S_Waketime = SysWakeUpTick;
    if (OOK_Sent_falg)
    {
        g_RfSendCnt = 1;
    }

    if (FSK_Sent_flag == 1)
    {
        TxMsg.WakeupFlag = 0u;          // 发唤醒码
        CommFlag.RadioTxState = TxIdle; // 马上去发
        TxMsg.HoldTxCnt = 5u;
        Fsk_CmdSend(&TxBuf, DYE_PRESS_STOP, Is6Pkg);
    }
    if(ColorTest_Flag == 1)
    {
        extern void Power_HideChargeUI(void);
        Power_HideChargeUI();
    }
}


