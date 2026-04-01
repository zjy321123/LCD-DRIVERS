#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Calendar.h"
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
extern S_LOCALTIMEDATA g_LocalTime;

static uint8_t Index_Scr69 = 0;
static uint8_t Edit_Hours = 0;   // 是否正在编辑小时（0=否，1=是）
static uint8_t Edit_Minutes = 0; // 是否正在编辑分钟（0=否，1=是）
uint8_t Editing_Tim = 0;

// 保存编辑前的原始时间数据
static uint8_t Backup_Hours = 0;
static uint8_t Backup_Minutes = 0;
static uint8_t Backup_Weekdays = 0;
static uint8_t Backup_Format = 0;
static uint8_t Backup_AM_PM = 0;

// 保存当前时间到备份
static void BackupCurrentTime(void)
{
    Backup_Hours = g_LocalTime.Hours;
    Backup_Minutes = g_LocalTime.Minutes;
    Backup_Weekdays = g_LocalTime.Weekdays;
    Backup_Format = g_LocalTime.Format;
    Backup_AM_PM = g_LocalTime.AM_PM;
}

// 从备份恢复时间
static void RestoreBackupTime(void)
{
    g_LocalTime.Hours = Backup_Hours;
    g_LocalTime.Minutes = Backup_Minutes;
    g_LocalTime.Weekdays = Backup_Weekdays;
    g_LocalTime.Format = Backup_Format;
    g_LocalTime.AM_PM = Backup_AM_PM;
}

void Screen_69_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Edit_Hours)
        {
            if (g_LocalTime.Format == FORMAT_12HOUSRS)
            {
                // 12小时制：0-11
                if (g_LocalTime.Hours < 11)
                {
                    g_LocalTime.Hours++;
                }
                else
                {
                    // 从11增加，变成00并切换AM/PM
                    g_LocalTime.Hours = 0;
                    g_LocalTime.AM_PM = !g_LocalTime.AM_PM;
                }
            }
            else if (g_LocalTime.Format == FORMAT_24HOUSRS)
            {
                if (g_LocalTime.Hours < 23)
                    g_LocalTime.Hours++;
                else
                    g_LocalTime.Hours = 0;
            }
        }
        else if (Edit_Minutes)
        {
            if (g_LocalTime.Minutes < 59)
                g_LocalTime.Minutes++;
            else
                g_LocalTime.Minutes = 0;
        }
        else
        {
            if (Index_Scr69 == 0)
                Index_Scr69 = 5;
            else
                Index_Scr69--;
        }
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Edit_Hours)
        {
            if (g_LocalTime.Format == FORMAT_12HOUSRS)
            {
                // 12小时制：0-11
                if (g_LocalTime.Hours > 0)
                {
                    g_LocalTime.Hours--;
                }
                else
                {
                    // 从00减少，变成11并切换AM/PM
                    g_LocalTime.Hours = 11;
                    g_LocalTime.AM_PM = !g_LocalTime.AM_PM;
                }
            }
            else
            {
                if (g_LocalTime.Hours > 0)
                    g_LocalTime.Hours--;
                else
                    g_LocalTime.Hours = 23;
            }
        }
        else if (Edit_Minutes)
        {
            if (g_LocalTime.Minutes > 0)
                g_LocalTime.Minutes--;
            else
                g_LocalTime.Minutes = 59;
        }
        else
        {
            if (Index_Scr69 == 5)
                Index_Scr69 = 0;
            else
                Index_Scr69++;
        }
    }
    else if (Keyid == KEYLEFT_TRG)
    {

        if (Index_Scr69 == 2)
        {
            if (!Editing_Tim) // 第一次进入编辑模式，保存备份
                BackupCurrentTime();
            Editing_Tim = 1; // 进入编辑模式，阻止RTC更新
            // 星期减少：MON(1) → SUN(7) → SAT(6) → ... → MON(1)
            if (g_LocalTime.Weekdays > 1)
                g_LocalTime.Weekdays--;
            else
                g_LocalTime.Weekdays = 7; // 从 MON 变为 SUN
        }
        else if (Index_Scr69 == 3)
        {
            if (!Editing_Tim) // 第一次进入编辑模式，保存备份
                BackupCurrentTime();
            Editing_Tim = 1; // 进入编辑模式，阻止RTC更新
            if (g_LocalTime.Format == FORMAT_12HOUSRS)
            {
                // 从12小时制切换到24小时制
                // 如果是PM，需要加12
                if (g_LocalTime.AM_PM) // PM
                    g_LocalTime.Hours += 12;
                g_LocalTime.Format = FORMAT_24HOUSRS;
            }
            else if (g_LocalTime.Format == FORMAT_24HOUSRS)
            {
                // 从24小时制切换到12小时制
                if (g_LocalTime.Hours >= 12)
                {
                    g_LocalTime.Hours -= 12;
                    g_LocalTime.AM_PM = 1; // PM
                }
                else
                {
                    g_LocalTime.AM_PM = 0; // AM
                }
                g_LocalTime.Format = FORMAT_12HOUSRS;
            }
        }
    }
    else if (Keyid == KEYRIGHT_TRG)
    {

        if (Index_Scr69 == 2)
        {
            // 星期增加：MON(1) → TUE(2) → ... → SUN(7) → MON(1)
            if (!Editing_Tim) // 第一次进入编辑模式，保存备份
                BackupCurrentTime();
            Editing_Tim = 1; // 进入编辑模式，阻止RTC更新
            if (g_LocalTime.Weekdays < 7)
                g_LocalTime.Weekdays++;
            else
                g_LocalTime.Weekdays = 1; // 从 SUN 变为 MON
        }
        else if (Index_Scr69 == 3)
        {
            if (!Editing_Tim) // 第一次进入编辑模式，保存备份
                BackupCurrentTime();
            Editing_Tim = 1; // 进入编辑模式，阻止RTC更新
            if (g_LocalTime.Format == FORMAT_12HOUSRS)
            {
                // 从12小时制切换到24小时制
                // 如果是PM，需要加12
                if (g_LocalTime.AM_PM) // PM
                    g_LocalTime.Hours += 12;
                g_LocalTime.Format = FORMAT_24HOUSRS;
            }
            else if (g_LocalTime.Format == FORMAT_24HOUSRS)
            {
                // 从24小时制切换到12小时制
                if (g_LocalTime.Hours >= 12)
                {
                    g_LocalTime.Hours -= 12;
                    g_LocalTime.AM_PM = 1; // PM
                }
                else
                {
                    g_LocalTime.AM_PM = 0; // AM
                }
                g_LocalTime.Format = FORMAT_12HOUSRS;
            }
        }
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Index_Scr69 == 0)
        {
            if (Edit_Hours)
            {
                Edit_Hours = 0;
            }
            else
            {
                if (!Editing_Tim) // 第一次进入编辑模式，保存备份
                    BackupCurrentTime();
                Edit_Hours = 1;
                Editing_Tim = 1; // 进入编辑模式，阻止RTC更新
            }
            Edit_Minutes = 0;
        }
        else if (Index_Scr69 == 1)
        {
            if (Edit_Minutes)
            {
                Edit_Minutes = 0;
            }
            else
            {
                if (!Editing_Tim) // 第一次进入编辑模式，保存备份
                    BackupCurrentTime();
                Edit_Minutes = 1;
                Editing_Tim = 1; // 进入编辑模式，阻止RTC更新
            }
            Edit_Hours = 0;
        }
        else if (Index_Scr69 == 4)
        {
            // 取消编辑，恢复原始时间
            if (Editing_Tim)
            {
                RestoreBackupTime();
            }
            Editing_Tim = 0;
            Edit_Hours = 0;
            Edit_Minutes = 0;
            lv_event_send(guider_ui.screen_69_btn_5, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_68
            current_screen = SCREEN_68;
            Screen68date_preloading();
            return;
        }
        else if (Index_Scr69 == 5)
        {
            // 保存时间：需要将12小时制转换为24小时制
            uint8_t save_hours = g_LocalTime.Hours;
            if (g_LocalTime.Format == FORMAT_12HOUSRS)
            {
                // 12小时制（0-11 + AM/PM）转换为 24小时制（0-23）
                if (g_LocalTime.AM_PM) // PM
                {
                    save_hours = g_LocalTime.Hours + 12;
                }
                // else: AM，保持原值
            }
            RTC_SetCurrentTime(save_hours, g_LocalTime.Minutes, 0);
            RTC_SetCurrentDate(0, 0, 0, g_LocalTime.Weekdays);
            S_TIMER_EXECUTE_INFO nextTimer;
            RTC_Set_Next_AlarmTimer(&nextTimer);
            SaveDataToFlash();
            lv_event_send(guider_ui.screen_69_btn_5, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_68
            current_screen = SCREEN_68;
            Screen68date_preloading();
            Editing_Tim = 0;
            Edit_Hours = 0;
            Edit_Minutes = 0;
            return;
        }
    }
    else if (Keyid == KEYMENU_TRG) // 保存数据
    {
        // 保存时间：需要将12小时制转换为24小时制
        uint8_t save_hours = g_LocalTime.Hours;
        if (g_LocalTime.Format == FORMAT_12HOUSRS)
        {
            // 12小时制（0-11 + AM/PM）转换为 24小时制（0-23）
            if (g_LocalTime.AM_PM) // PM
            {
                save_hours = g_LocalTime.Hours + 12;
            }
            // else: AM，保持原值
        }
        RTC_SetCurrentTime(save_hours, g_LocalTime.Minutes, 0);
        RTC_SetCurrentDate(0, 0, 0, g_LocalTime.Weekdays);
        SaveDataToFlash();
        lv_event_send(guider_ui.screen_69_btn_5, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_68
        current_screen = SCREEN_68;
        Screen68date_preloading();
        Editing_Tim = 0;
        Edit_Hours = 0;
        Edit_Minutes = 0;
        return;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        // 恢复编辑前的时间，取消所有修改
        if (Editing_Tim)
        {
            RestoreBackupTime();
        }
        Editing_Tim = 0;
        Edit_Hours = 0;
        Edit_Minutes = 0;
        lv_event_send(guider_ui.screen_69_btn_5, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_68
        current_screen = SCREEN_68;
        Screen68date_preloading();
        return;
    }
    Screen69date_preloading();
}
void Screen69date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_69_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_69_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_69_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_69_btn_4, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_69_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_69_btn_6, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(guider_ui.screen_69_label_10, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_69_label_12, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_69_label_13, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_69_label_11, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_69_label_14, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_69_label_15, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_69_label_16, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_69_label_17, LV_OBJ_FLAG_HIDDEN);

    // 只有在未编辑状态下才发送点击事件，避免重置编辑状态
    if (!Edit_Hours && !Edit_Minutes)
    {
        if (Index_Scr69 == 0)
            lv_event_send(guider_ui.screen_69_btn_1, LV_EVENT_CLICKED, NULL);
        else if (Index_Scr69 == 1)
            lv_event_send(guider_ui.screen_69_btn_2, LV_EVENT_CLICKED, NULL);
        else if (Index_Scr69 == 2)
        {
            lv_event_send(guider_ui.screen_69_btn_3, LV_EVENT_CLICKED, NULL);
            lv_obj_clear_flag(guider_ui.screen_69_label_14, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(guider_ui.screen_69_label_16, LV_OBJ_FLAG_HIDDEN);
        }
        else if (Index_Scr69 == 3)
        {
            lv_event_send(guider_ui.screen_69_btn_4, LV_EVENT_CLICKED, NULL);
            lv_obj_clear_flag(guider_ui.screen_69_label_15, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(guider_ui.screen_69_label_17, LV_OBJ_FLAG_HIDDEN);
        }
        else if (Index_Scr69 == 4)
            lv_event_send(guider_ui.screen_69_btn_5, LV_EVENT_CLICKED, NULL);
        else if (Index_Scr69 == 5)
            lv_event_send(guider_ui.screen_69_btn_6, LV_EVENT_CLICKED, NULL);
    }
    else
    {
        // 在编辑模式下，仍然需要显示左右箭头
        if (Index_Scr69 == 0)
            lv_event_send(guider_ui.screen_69_btn_1, LV_EVENT_CLICKED, NULL);
        else if (Index_Scr69 == 1)
            lv_event_send(guider_ui.screen_69_btn_2, LV_EVENT_CLICKED, NULL);
        if (Index_Scr69 == 2)
        {
            lv_obj_clear_flag(guider_ui.screen_69_label_14, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(guider_ui.screen_69_label_16, LV_OBJ_FLAG_HIDDEN);
        }
        else if (Index_Scr69 == 3)
        {
            lv_obj_clear_flag(guider_ui.screen_69_label_15, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(guider_ui.screen_69_label_17, LV_OBJ_FLAG_HIDDEN);
        }
    }

    if (Edit_Hours)
    {
        lv_obj_clear_flag(guider_ui.screen_69_label_10, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_69_label_12, LV_OBJ_FLAG_HIDDEN);
    }
    if (Edit_Minutes)
    {
        lv_obj_clear_flag(guider_ui.screen_69_label_11, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_69_label_13, LV_OBJ_FLAG_HIDDEN);
    }

    lv_label_set_text_fmt(guider_ui.screen_69_btn_1_label, "%d", g_LocalTime.Hours);
    lv_label_set_text_fmt(guider_ui.screen_69_btn_2_label, "%d", g_LocalTime.Minutes);

    // 确保星期在有效范围内 (1-7)
    if (g_LocalTime.Weekdays < 1 || g_LocalTime.Weekdays > 7)
        g_LocalTime.Weekdays = 1; // 默认设为星期一

    if (g_LocalTime.Weekdays == MON)
        lv_label_set_text(guider_ui.screen_69_btn_3_label, "     Day                     Mon  ");
    else if (g_LocalTime.Weekdays == TUE)
        lv_label_set_text(guider_ui.screen_69_btn_3_label, "     Day                     Tue  ");
    else if (g_LocalTime.Weekdays == WED)
        lv_label_set_text(guider_ui.screen_69_btn_3_label, "     Day                     Wed  ");
    else if (g_LocalTime.Weekdays == THU)
        lv_label_set_text(guider_ui.screen_69_btn_3_label, "     Day                     Thu  ");
    else if (g_LocalTime.Weekdays == FRI)
        lv_label_set_text(guider_ui.screen_69_btn_3_label, "     Day                     Fri  ");
    else if (g_LocalTime.Weekdays == SAT)
        lv_label_set_text(guider_ui.screen_69_btn_3_label, "     Day                     Sat  ");
    else if (g_LocalTime.Weekdays == SUN)
        lv_label_set_text(guider_ui.screen_69_btn_3_label, "     Day                     Sun  ");

    if (g_LocalTime.Format == FORMAT_12HOUSRS)
    {
        // 12小时制：根据g_LocalTime.AM_PM显示AM或PM
        if (g_LocalTime.AM_PM)
            lv_label_set_text(guider_ui.screen_69_label_5, "PM");
        else
            lv_label_set_text(guider_ui.screen_69_label_5, "AM");
        lv_label_set_text(guider_ui.screen_69_btn_4_label, "     Timer                12HR");
    }
    else if (g_LocalTime.Format == FORMAT_24HOUSRS)
    {
        // 24小时制：不显示AM/PM
        lv_label_set_text(guider_ui.screen_69_label_5, "HM");
        lv_label_set_text(guider_ui.screen_69_btn_4_label, "     Timer                24HR");
    }
}
