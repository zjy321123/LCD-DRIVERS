#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Customdate.h"
#include "Calendar.h"
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
extern uint8_t g_TimerAlarmType;
extern uint8_t g_TimerPreIndex;

static uint8_t Index_Scr66 = 0;
static uint8_t EditingHours = 0; // 是否正在编辑小时
static uint8_t EditingMins = 0;  // 是否正在编辑分钟

void Screen_66_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (EditingHours)
        {
            // 编辑小时
            if (g_TimerAlarmType == ALARM1TYPE)
            {
                if (CustonmTimers[g_TimerPreIndex].Format == FORMAT_12HOUSRS)
                {
                    // 12小时制：0-11，从11增加时变成0并切换AM/PM
                    if (CustonmTimers[g_TimerPreIndex].Type1Hours < 11)
                    {
                        CustonmTimers[g_TimerPreIndex].Type1Hours++;
                    }
                    else
                    {
                        CustonmTimers[g_TimerPreIndex].Type1Hours = 0;
                        CustonmTimers[g_TimerPreIndex].Type1_AM_PM = !CustonmTimers[g_TimerPreIndex].Type1_AM_PM;
                    }
                }
                else if (CustonmTimers[g_TimerPreIndex].Format == FORMAT_24HOUSRS)
                {
                    if (CustonmTimers[g_TimerPreIndex].Type1Hours < 23)
                        CustonmTimers[g_TimerPreIndex].Type1Hours++;
                    else
                        CustonmTimers[g_TimerPreIndex].Type1Hours = 0;
                }
            }
            else if (g_TimerAlarmType == ALARM2TYPE)
            {
                if (CustonmTimers[g_TimerPreIndex].Format == FORMAT_12HOUSRS)
                {
                    // 12小时制：0-11，从11增加时变成0并切换AM/PM
                    if (CustonmTimers[g_TimerPreIndex].Type2Hours < 11)
                    {
                        CustonmTimers[g_TimerPreIndex].Type2Hours++;
                    }
                    else
                    {
                        CustonmTimers[g_TimerPreIndex].Type2Hours = 0;
                        CustonmTimers[g_TimerPreIndex].Type2_AM_PM = !CustonmTimers[g_TimerPreIndex].Type2_AM_PM;
                    }
                }
                else if (CustonmTimers[g_TimerPreIndex].Format == FORMAT_24HOUSRS)
                {
                    if (CustonmTimers[g_TimerPreIndex].Type2Hours < 23)
                        CustonmTimers[g_TimerPreIndex].Type2Hours++;
                    else
                        CustonmTimers[g_TimerPreIndex].Type2Hours = 0;
                }
            }
        }
        else if (EditingMins)
        {
            // 编辑分钟
            if (g_TimerAlarmType == ALARM1TYPE)
            {
                if (CustonmTimers[g_TimerPreIndex].Type1Minutes < 59)
                    CustonmTimers[g_TimerPreIndex].Type1Minutes++;
                else
                    CustonmTimers[g_TimerPreIndex].Type1Minutes = 0;
            }
            else if (g_TimerAlarmType == ALARM2TYPE)
            {
                if (CustonmTimers[g_TimerPreIndex].Type2Minutes < 59)
                    CustonmTimers[g_TimerPreIndex].Type2Minutes++;
                else
                    CustonmTimers[g_TimerPreIndex].Type2Minutes = 0;
            }
        }
        else
        {
            // 移动光标
            if (Index_Scr66 == 0)
                Index_Scr66 = 3;
            else
                Index_Scr66--;
        }
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (EditingHours)
        {
            // 编辑小时
            if (g_TimerAlarmType == ALARM1TYPE)
            {
                if (CustonmTimers[g_TimerPreIndex].Format == FORMAT_12HOUSRS)
                {
                    // 12小时制：0-11，从0减少时变成11并切换AM/PM
                    if (CustonmTimers[g_TimerPreIndex].Type1Hours > 0)
                    {
                        CustonmTimers[g_TimerPreIndex].Type1Hours--;
                    }
                    else
                    {
                        CustonmTimers[g_TimerPreIndex].Type1Hours = 11;
                        CustonmTimers[g_TimerPreIndex].Type1_AM_PM = !CustonmTimers[g_TimerPreIndex].Type1_AM_PM;
                    }
                }
                else
                {
                    // 24小时制：0-23
                    if (CustonmTimers[g_TimerPreIndex].Type1Hours > 0)
                        CustonmTimers[g_TimerPreIndex].Type1Hours--;
                    else
                        CustonmTimers[g_TimerPreIndex].Type1Hours = 23;
                }
            }
            else if (g_TimerAlarmType == ALARM2TYPE)
            {
                if (CustonmTimers[g_TimerPreIndex].Format == FORMAT_12HOUSRS)
                {
                    // 12小时制：0-11，从0减少时变成11并切换AM/PM
                    if (CustonmTimers[g_TimerPreIndex].Type2Hours > 0)
                    {
                        CustonmTimers[g_TimerPreIndex].Type2Hours--;
                    }
                    else
                    {
                        CustonmTimers[g_TimerPreIndex].Type2Hours = 11;
                        CustonmTimers[g_TimerPreIndex].Type2_AM_PM = !CustonmTimers[g_TimerPreIndex].Type2_AM_PM;
                    }
                }
                else
                {
                    // 24小时制：0-23
                    if (CustonmTimers[g_TimerPreIndex].Type2Hours > 0)
                        CustonmTimers[g_TimerPreIndex].Type2Hours--;
                    else
                        CustonmTimers[g_TimerPreIndex].Type2Hours = 23;
                }
            }
        }
        else if (EditingMins)
        {
            // 编辑分钟
            if (g_TimerAlarmType == ALARM1TYPE)
            {
                if (CustonmTimers[g_TimerPreIndex].Type1Minutes > 0)
                    CustonmTimers[g_TimerPreIndex].Type1Minutes--;
                else
                    CustonmTimers[g_TimerPreIndex].Type1Minutes = 59;
            }
            else if (g_TimerAlarmType == ALARM2TYPE)
            {
                if (CustonmTimers[g_TimerPreIndex].Type2Minutes > 0)
                    CustonmTimers[g_TimerPreIndex].Type2Minutes--;
                else
                    CustonmTimers[g_TimerPreIndex].Type2Minutes = 59;
            }
        }
        else
        {
            // 移动光标
            if (Index_Scr66 == 3)
                Index_Scr66 = 0;
            else
                Index_Scr66++;
        }
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Index_Scr66 == 0)
        {
            // 切换编辑小时状态
            EditingHours = !EditingHours;
            EditingMins = 0;
        }
        else if (Index_Scr66 == 1)
        {
            // 切换编辑分钟状态
            EditingMins = !EditingMins;
            EditingHours = 0;
        }
        else if (Index_Scr66 == 2)
        {
            // Save - 保存并返回
            EditingHours = 0;
            EditingMins = 0;
            S_TIMER_EXECUTE_INFO nextTimer;
            RTC_Set_Next_AlarmTimer(&nextTimer);
            SaveDataToFlash();
            g_TimerAlarmType = ALARMNULLTYPE;
            lv_event_send(guider_ui.screen_66_label_3, LV_EVENT_CLICKED, NULL);
            current_screen = SCREEN_62;
            Screen62date_preloading();
            return;
        }
        else if (Index_Scr66 == 3)
        {
            // Cancel - 返回不保存
            EditingHours = 0;
            EditingMins = 0;
            g_TimerAlarmType = ALARMNULLTYPE;
            lv_event_send(guider_ui.screen_66_label_3, LV_EVENT_CLICKED, NULL);
            current_screen = SCREEN_62;
            Screen62date_preloading();
            return;
        }
    }
    else if (Keyid == KEYMENU_TRG)
    {
        // 切换时间格式（12小时/24小时）
        if (CustonmTimers[g_TimerPreIndex].Format == FORMAT_12HOUSRS)
        {
            // 从12小时制切换到24小时制
            if (g_TimerAlarmType == ALARM1TYPE)
            {
                // 将 0-11 + AM/PM 转换为 0-23
                if (CustonmTimers[g_TimerPreIndex].Type1_AM_PM) // PM
                {
                    CustonmTimers[g_TimerPreIndex].Type1Hours += 12;
                }
                // AM时保持原值
            }
            else if (g_TimerAlarmType == ALARM2TYPE)
            {
                // 将 0-11 + AM/PM 转换为 0-23
                if (CustonmTimers[g_TimerPreIndex].Type2_AM_PM) // PM
                {
                    CustonmTimers[g_TimerPreIndex].Type2Hours += 12;
                }
                // AM时保持原值
            }
            CustonmTimers[g_TimerPreIndex].Format = FORMAT_24HOUSRS;
        }
        else if (CustonmTimers[g_TimerPreIndex].Format == FORMAT_24HOUSRS)
        {
            // 从24小时制切换到12小时制
            if (g_TimerAlarmType == ALARM1TYPE)
            {
                // 将 0-23 转换为 0-11 + AM/PM
                if (CustonmTimers[g_TimerPreIndex].Type1Hours >= 12)
                {
                    CustonmTimers[g_TimerPreIndex].Type1Hours -= 12;
                    CustonmTimers[g_TimerPreIndex].Type1_AM_PM = 1; // PM
                }
                else
                {
                    CustonmTimers[g_TimerPreIndex].Type1_AM_PM = 0; // AM
                }
            }
            else if (g_TimerAlarmType == ALARM2TYPE)
            {
                // 将 0-23 转换为 0-11 + AM/PM
                if (CustonmTimers[g_TimerPreIndex].Type2Hours >= 12)
                {
                    CustonmTimers[g_TimerPreIndex].Type2Hours -= 12;
                    CustonmTimers[g_TimerPreIndex].Type2_AM_PM = 1; // PM
                }
                else
                {
                    CustonmTimers[g_TimerPreIndex].Type2_AM_PM = 0; // AM
                }
            }
            CustonmTimers[g_TimerPreIndex].Format = FORMAT_12HOUSRS;
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        EditingHours = 0;
        EditingMins = 0;
        g_TimerAlarmType = ALARMNULLTYPE;
        lv_event_send(guider_ui.screen_66_label_3, LV_EVENT_CLICKED, NULL);
        current_screen = SCREEN_62;
        Screen62date_preloading();
        return;
    }
    Screen66date_preloading();
}

void Screen66date_preloading(void)
{
    // 重置所有按钮背景色
    lv_obj_set_style_bg_color(guider_ui.screen_66_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_66_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_66_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_66_btn_4, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 隐藏所有编辑指示器
    lv_obj_add_flag(guider_ui.screen_66_label_10, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_66_label_12, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_66_label_13, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_66_label_11, LV_OBJ_FLAG_HIDDEN);

    // 高亮当前光标位置
    if (Index_Scr66 == 0)
        lv_event_send(guider_ui.screen_66_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr66 == 1)
        lv_event_send(guider_ui.screen_66_btn_2, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr66 == 2)
        lv_event_send(guider_ui.screen_66_btn_3, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr66 == 3)
        lv_event_send(guider_ui.screen_66_btn_4, LV_EVENT_CLICKED, NULL);

    // 显示编辑状态指示器
    if (EditingHours)
    {
        lv_obj_clear_flag(guider_ui.screen_66_label_10, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_66_label_12, LV_OBJ_FLAG_HIDDEN);
    }
    if (EditingMins)
    {
        lv_obj_clear_flag(guider_ui.screen_66_label_11, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_66_label_13, LV_OBJ_FLAG_HIDDEN);
    }

    // 更新时间显示
    if (g_TimerAlarmType == ALARM1TYPE)
    {
        lv_label_set_text(guider_ui.screen_66_label_3, "Edit Timer 1");
        lv_label_set_text_fmt(guider_ui.screen_66_btn_1_label, "%d", CustonmTimers[g_TimerPreIndex].Type1Hours);
        lv_label_set_text_fmt(guider_ui.screen_66_btn_2_label, "%d", CustonmTimers[g_TimerPreIndex].Type1Minutes);
    }
    else if (g_TimerAlarmType == ALARM2TYPE)
    {
        lv_label_set_text(guider_ui.screen_66_label_3, "Edit Timer 2");
        lv_label_set_text_fmt(guider_ui.screen_66_btn_1_label, "%d", CustonmTimers[g_TimerPreIndex].Type2Hours);
        lv_label_set_text_fmt(guider_ui.screen_66_btn_2_label, "%d", CustonmTimers[g_TimerPreIndex].Type2Minutes);
    }

    // 更新时间格式显示（AM/PM或HM）
    if (CustonmTimers[g_TimerPreIndex].Format == FORMAT_12HOUSRS)
    {
        // 12小时制：显示AM或PM
        if (g_TimerAlarmType == ALARM1TYPE)
        {
            lv_label_set_text(guider_ui.screen_66_label_5,
                              CustonmTimers[g_TimerPreIndex].Type1_AM_PM ? "PM" : "AM");
        }
        else if (g_TimerAlarmType == ALARM2TYPE)
        {
            lv_label_set_text(guider_ui.screen_66_label_5,
                              CustonmTimers[g_TimerPreIndex].Type2_AM_PM ? "PM" : "AM");
        }
    }
    else if (CustonmTimers[g_TimerPreIndex].Format == FORMAT_24HOUSRS)
    {
        // 24小时制：显示HM
        lv_label_set_text(guider_ui.screen_66_label_5, "HM");
    }
}
