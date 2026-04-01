#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "Customdate.h"
extern lv_indev_t *indev_keypad;
extern lv_ui guider_ui; // 声明 界面对象
static uint8_t Index_Scr65 = 0;
extern uint8_t g_CacheCmd;
extern uint8_t g_TimerSentType;
extern uint8_t g_TimerPreIndex;
extern current_screen_t current_screen, Mem_Screen;
void Screen_65_Movie(unsigned int Keyid)
{

    if (Keyid == KEYFORWARD_TRG)
    {
        if (Index_Scr65 == 0)
            Index_Scr65 = 4;
        else
            Index_Scr65--;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Index_Scr65 == 4)
            Index_Scr65 = 0;
        else
            Index_Scr65++;
    }
    else if (Keyid == KEYLEFT_TRG)
    {
        // 只在PCT选项时响应左键（减小百分比）
        if (Index_Scr65 == 4) // 第5个选项（索引4）是PCT
        {
            if (g_TimerSentType == TIMER1TYPE)
            {
                if (CustonmTimers[g_TimerPreIndex].TypePct1 > 0)
                    CustonmTimers[g_TimerPreIndex].TypePct1 -= 10;
            }
            else if (g_TimerSentType == TIMER2TYPE)
            {
                if (CustonmTimers[g_TimerPreIndex].TypePct2 > 0)
                    CustonmTimers[g_TimerPreIndex].TypePct2 -= 10;
            }
        }
    }
    else if (Keyid == KEYRIGHT_TRG)
    {
        // 只在PCT选项时响应右键（增加百分比）
        if (Index_Scr65 == 4) // 第5个选项（索引4）是PCT
        {
            if (g_TimerSentType == TIMER1TYPE)
            {
                if (CustonmTimers[g_TimerPreIndex].TypePct1 < 100)
                    CustonmTimers[g_TimerPreIndex].TypePct1 += 10;
            }
            else if (g_TimerSentType == TIMER2TYPE)
            {
                if (CustonmTimers[g_TimerPreIndex].TypePct2 < 100)
                    CustonmTimers[g_TimerPreIndex].TypePct2 += 10;
            }
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_65_label_12, LV_EVENT_CLICKED, NULL); // 加载界面Screen_62
        current_screen = SCREEN_62;
        g_TimerSentType = TIMERNULLTYPE;
        Screen62date_preloading();
        return;
    }
    else if (Keyid == KEYMENU_TRG)
    {
        S_TIMER_EXECUTE_INFO nextTimer;
        RTC_Set_Next_AlarmTimer(&nextTimer);
        // 保存数据到Flash（类型值已在ENTER时修改）
        SaveDataToFlash();
        lv_event_send(guider_ui.screen_65_label_12, LV_EVENT_CLICKED, NULL); // 加载界面Screen_62
        current_screen = SCREEN_62;
        g_TimerSentType = TIMERNULLTYPE;
        Screen62date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        // 单选逻辑：直接修改定时器类型
        uint8_t selected_type;
        switch (Index_Scr65)
        {
        case 0:
            selected_type = DefaultType;
            break;
        case 1:
            selected_type = FavType;
            break;
        case 2:
            selected_type = FullOpenType;
            break;
        case 3:
            selected_type = FullCloseType;
            break;
        case 4:
            selected_type = PctType;
            break;
        default:
            selected_type = DefaultType;
            break;
        }

        // 直接修改定时器的类型值
        if (g_TimerSentType == TIMER1TYPE)
        {
            CustonmTimers[g_TimerPreIndex].Type1 = selected_type;
        }
        else if (g_TimerSentType == TIMER2TYPE)
        {
            CustonmTimers[g_TimerPreIndex].Type2 = selected_type;
        }
    }
    Screen65date_preloading();
}

void Screen65date_preloading(void)
{
    // 设置标题
    if (g_TimerSentType == TIMER1TYPE)
    {
        lv_label_set_text(guider_ui.screen_65_label_2, "Edit Behavior 1");
    }
    else if (g_TimerSentType == TIMER2TYPE)
    {
        lv_label_set_text(guider_ui.screen_65_label_2, "Edit Behavior 2");
    }

    // 重置所有按钮背景色
    lv_obj_set_style_bg_color(guider_ui.screen_65_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_65_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_65_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_65_btn_4, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_65_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_65_btn_6, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_65_btn_7, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_65_btn_8, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_65_btn_9, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_65_btn_10, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 高亮当前光标位置
    switch (Index_Scr65 + 1)
    {
    case 1:
        lv_event_send(guider_ui.screen_65_btn_1, LV_EVENT_CLICKED, NULL);
        lv_event_send(guider_ui.screen_65_btn_6, LV_EVENT_CLICKED, NULL);
        break;
    case 2:
        lv_event_send(guider_ui.screen_65_btn_2, LV_EVENT_CLICKED, NULL);
        lv_event_send(guider_ui.screen_65_btn_7, LV_EVENT_CLICKED, NULL);
        break;
    case 3:
        lv_event_send(guider_ui.screen_65_btn_3, LV_EVENT_CLICKED, NULL);
        lv_event_send(guider_ui.screen_65_btn_8, LV_EVENT_CLICKED, NULL);
        break;
    case 4:
        lv_event_send(guider_ui.screen_65_btn_4, LV_EVENT_CLICKED, NULL);
        lv_event_send(guider_ui.screen_65_btn_9, LV_EVENT_CLICKED, NULL);
        break;
    case 5:
        lv_event_send(guider_ui.screen_65_btn_5, LV_EVENT_CLICKED, NULL);
        lv_event_send(guider_ui.screen_65_btn_10, LV_EVENT_CLICKED, NULL);
        break;
    default:
        break;
    }

    // 更新PCT百分比显示
    if (g_TimerSentType == TIMER1TYPE)
    {
        lv_label_set_text_fmt(guider_ui.screen_65_btn_5_label, "Open   " LV_SYMBOL_LEFT "  %d%% " LV_SYMBOL_RIGHT " ", CustonmTimers[g_TimerPreIndex].TypePct1);
    }
    else if (g_TimerSentType == TIMER2TYPE)
    {
        lv_label_set_text_fmt(guider_ui.screen_65_btn_5_label, "Open   " LV_SYMBOL_LEFT "  %d%%  " LV_SYMBOL_RIGHT " ", CustonmTimers[g_TimerPreIndex].TypePct2);
    }

    // 隐藏所有复选框
    lv_obj_add_flag(guider_ui.screen_65_btn_6_label, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_65_btn_7_label, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_65_btn_8_label, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_65_btn_9_label, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_65_btn_10_label, LV_OBJ_FLAG_HIDDEN);

    // 只显示当前选中的复选框（单选逻辑）
    // 直接读取定时器的类型值
    uint8_t current_type;
    if (g_TimerSentType == TIMER1TYPE)
    {
        current_type = CustonmTimers[g_TimerPreIndex].Type1;
    }
    else if (g_TimerSentType == TIMER2TYPE)
    {
        current_type = CustonmTimers[g_TimerPreIndex].Type2;
    }
    else
    {
        current_type = DefaultType;
    }

    switch (current_type)
    {
    case DefaultType: // 0
        lv_obj_clear_flag(guider_ui.screen_65_btn_6_label, LV_OBJ_FLAG_HIDDEN);
        break;
    case FavType: // 1
        lv_obj_clear_flag(guider_ui.screen_65_btn_7_label, LV_OBJ_FLAG_HIDDEN);
        break;
    case FullOpenType: // 2
        lv_obj_clear_flag(guider_ui.screen_65_btn_8_label, LV_OBJ_FLAG_HIDDEN);
        break;
    case FullCloseType: // 3
        lv_obj_clear_flag(guider_ui.screen_65_btn_9_label, LV_OBJ_FLAG_HIDDEN);
        break;
    case PctType: // 4
        lv_obj_clear_flag(guider_ui.screen_65_btn_10_label, LV_OBJ_FLAG_HIDDEN);
        break;
    default:
        break;
    }
}
