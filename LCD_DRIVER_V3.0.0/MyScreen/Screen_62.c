#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Customdate.h"
#include "Calendar.h"
extern current_screen_t current_screen, Pre_screen, Mem_Screen;
extern uint8_t g_TimerPreIndex;
uint8_t g_TimerSentType;
uint8_t g_TimerAlarmType;
#define TimerIdems 9u // 子状态数量
const char *TimerMenuTexts[TimerIdems] =
    {
        "Edit Name", // 0  编辑名字
        "Enable"
        "  " LV_SYMBOL_LEFT "  "
        "ON"
        "  " LV_SYMBOL_RIGHT,   // 1  是否使能定时通道
        "Edit Schedule",        // 2  编辑日期
        "Edit Groups & Shades", // 3  编辑群组与通道
        "Edit SentType 1",      // 4  编辑时刻1操作方式
        "Edit Timer 1",         // 5  编辑时刻1的时间
        "Edit SentType 2",      // 6  编辑时刻2操作方式
        "Edit Timer 2",         // 7  编辑时刻2的时间
        "Delete Timer",         // 8  删除定时
};

static uint8_t Index_Scr62, TimerMenu62Cnt = 0;

void Screen_62_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG) // 向下滚动
    {
        if (Index_Scr62 == TimerIdems - 1)
        {
            // 当前在最后一项，跳转到第一项
            Index_Scr62 = 0;
            TimerMenu62Cnt = 0;
        }
        else
        {
            Index_Scr62++;

            // 当选中项在显示区域的最后两个时滚动显示
            if (Index_Scr62 >= TimerMenu62Cnt + 3)
            {
                TimerMenu62Cnt = (TimerMenu62Cnt + 1) % TimerIdems;
            }
        }
    }
    else if (Keyid == KEYFORWARD_TRG) // 向上滚动
    {
        if (Index_Scr62 == 0)
        {
            // 当前在第一项，跳转到最后一项
            Index_Scr62 = TimerIdems - 1;
            // 设置显示区域为最后5项的起始位置
            TimerMenu62Cnt = (TimerIdems >= 3) ? (TimerIdems - 3) : 0;
        }
        else
        {
            Index_Scr62--;

            // 当选中项在显示区域的前两个时滚动显示
            if (Index_Scr62 < TimerMenu62Cnt)
            {
                TimerMenu62Cnt = (TimerMenu62Cnt + TimerIdems - 1) % TimerIdems;
            }
        }
    }
    else if (Keyid == KEYLEFT_TRG || Keyid == KEYRIGHT_TRG)
    {
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_62_label_3, LV_EVENT_CLICKED, NULL); // 加载界面screen_61
        current_screen = SCREEN_61;
        Screen61date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        switch (Index_Scr62)
        {
        case 0:                                                                     //"Edit Name"
            lv_event_send(guider_ui.screen_62_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen9
            current_screen = SCREEN_9;
            Mem_Screen = SCREEN_62; // 屏幕记忆点，便于跳转
            Screen9date_preloading();
            return;
        case 1: //"Enable shade"
            CustonmTimers[g_TimerPreIndex].TimerEnable = !CustonmTimers[g_TimerPreIndex].TimerEnable;
            SaveDataToFlash(); // 保存到Flash
            S_TIMER_EXECUTE_INFO nextTimer;
            RTC_Set_Next_AlarmTimer(&nextTimer);
            // 直接更新菜单文本
            TimerMenuTexts[1] = CustonmTimers[g_TimerPreIndex].TimerEnable ? "Enable"
                                                                             "  " LV_SYMBOL_LEFT "  "
                                                                             "ON"
                                                                             "  " LV_SYMBOL_RIGHT
                                                                           : "Enable"
                                                                             "  " LV_SYMBOL_LEFT "  "
                                                                             "OFF"
                                                                             "  " LV_SYMBOL_RIGHT;
            break;
        case 2:                                                               //"Edit Schedule"
            lv_event_send(guider_ui.screen_62_btn_1, LV_EVENT_PRESSED, NULL); // 加载界面screen63 选择日期
            current_screen = SCREEN_63;
            Screen63date_preloading();
            return;
        case 3:                                                                //"Edit Groups & Shades"
            lv_event_send(guider_ui.screen_62_btn_1, LV_EVENT_PRESSING, NULL); // 加载界面screen64
            current_screen = SCREEN_64;
            Screen64date_preloading();
            return;
        case 4:                                                                  //"Edit Behavior 1"
            lv_event_send(guider_ui.screen_62_btn_1, LV_EVENT_PRESS_LOST, NULL); // 加载界面screen65
            current_screen = SCREEN_65;
            g_TimerSentType = TIMER1TYPE;
            Screen65date_preloading();
            return;
        case 5:                                                                //"Edit Timer 1"
            lv_event_send(guider_ui.screen_62_btn_1, LV_EVENT_RELEASED, NULL); // 加载界面screen66
            current_screen = SCREEN_66;
            g_TimerAlarmType = ALARM1TYPE;
            Screen66date_preloading();
            return;
        case 6:                                                                  //"Edit Behavior 2"
            lv_event_send(guider_ui.screen_62_btn_1, LV_EVENT_PRESS_LOST, NULL); // 加载界面screen65
            current_screen = SCREEN_65;
            g_TimerSentType = TIMER2TYPE;
            Screen65date_preloading();
            return;
        case 7:                                                                //"Edit Timer 2"
            lv_event_send(guider_ui.screen_62_btn_1, LV_EVENT_RELEASED, NULL); // 加载界面screen66
            current_screen = SCREEN_66;
            g_TimerAlarmType = ALARM2TYPE;
            Screen66date_preloading();
            return;
        case 8:                                                                     // "Delate Timer"
            lv_event_send(guider_ui.screen_62_btn_1, LV_EVENT_VALUE_CHANGED, NULL); // 加载界面screen67
            current_screen = SCREEN_67;
            Screen67date_preloading();
            return;
        default:
            break;
        }
    }
    Screen62date_preloading();
}
void Screen62date_preloading(void)
{

    // 根据实际状态更新菜单文本
    TimerMenuTexts[1] = CustonmTimers[g_TimerPreIndex].TimerEnable ? "Enable"
                                                                     "  " LV_SYMBOL_LEFT "  "
                                                                     "ON"
                                                                     "  " LV_SYMBOL_RIGHT
                                                                   : "Enable"
                                                                     "  " LV_SYMBOL_LEFT "  "
                                                                     "OFF"
                                                                     "  " LV_SYMBOL_RIGHT;

    lv_obj_set_size(guider_ui.screen_62_line_2, 4, Index_Scr62 * (210 / TimerIdems));

    char display_text[21]; // 19字节最大需求 + 1
    char *current_data = CustonmTimers[g_TimerPreIndex].DataArray;

    strcpy(display_text, "Edit ");
    if (strlen(current_data) > 10)
    {
        strncat(display_text, current_data, 10);
        strcat(display_text, "...");
    }
    else
    {
        strcat(display_text, (const char *)current_data);
    }
    lv_label_set_text(guider_ui.screen_62_label_3, display_text);
    // 设置3个按钮的文本（循环显示）
    for (int i = 0; i < 3; i++)
    {
        uint8_t itemIdx = (TimerMenu62Cnt + i) % TimerIdems;
        switch (i)
        {
        case 0:
            lv_label_set_text(guider_ui.screen_62_btn_1_label, TimerMenuTexts[itemIdx]);
            break;
        case 1:
            lv_label_set_text(guider_ui.screen_62_btn_2_label, TimerMenuTexts[itemIdx]);
            break;
        case 2:
            lv_label_set_text(guider_ui.screen_62_btn_3_label, TimerMenuTexts[itemIdx]);
            break;
        }
    }
    // 根据Type1显示对应的类型名称和时间
    const char *typeStr = "";
    if (CustonmTimers[g_TimerPreIndex].Type1 == DefaultType)
        typeStr = "none";
    else if (CustonmTimers[g_TimerPreIndex].Type1 == FavType)
        typeStr = "fav";
    else if (CustonmTimers[g_TimerPreIndex].Type1 == FullOpenType)
        typeStr = "FullOpen";
    else if (CustonmTimers[g_TimerPreIndex].Type1 == FullCloseType)
        typeStr = "FullClose";
    else if (CustonmTimers[g_TimerPreIndex].Type1 == PctType)
    {
        if (CustonmTimers[g_TimerPreIndex].TypePct1 == 0)
            typeStr = "PCT0";
        else if (CustonmTimers[g_TimerPreIndex].TypePct1 == 10)
            typeStr = "PCT10";
        else if (CustonmTimers[g_TimerPreIndex].TypePct1 == 20)
            typeStr = "PCT20";
        else if (CustonmTimers[g_TimerPreIndex].TypePct1 == 30)
            typeStr = "PCT30";
        else if (CustonmTimers[g_TimerPreIndex].TypePct1 == 40)
            typeStr = "PCT40";
        else if (CustonmTimers[g_TimerPreIndex].TypePct1 == 50)
            typeStr = "PCT50";
        else if (CustonmTimers[g_TimerPreIndex].TypePct1 == 60)
            typeStr = "PCT60";
        else if (CustonmTimers[g_TimerPreIndex].TypePct1 == 70)
            typeStr = "PCT70";
        else if (CustonmTimers[g_TimerPreIndex].TypePct1 == 80)
            typeStr = "PCT80";
        else if (CustonmTimers[g_TimerPreIndex].TypePct1 == 90)
            typeStr = "PCT90";
        else if (CustonmTimers[g_TimerPreIndex].TypePct1 == 100)
            typeStr = "PCT100";
    }

    // 格式化时间并判断AM/PM
    uint8_t hour = CustonmTimers[g_TimerPreIndex].Type1Hours;
    uint8_t minute = CustonmTimers[g_TimerPreIndex].Type1Minutes;
    
    if (CustonmTimers[g_TimerPreIndex].Format == FORMAT_12HOUSRS)
    {
        // 12小时制：Hours 是 0-11，使用 Type1_AM_PM 判断AM/PM
        const char *ampm = CustonmTimers[g_TimerPreIndex].Type1_AM_PM ? "PM" : "AM";
        lv_label_set_text_fmt(guider_ui.screen_62_label_4, "%s\n%02d : %02d %s", typeStr, hour, minute, ampm);
    }
    else
    {
        // 24小时制：Hours 是 0-23，直接显示
        lv_label_set_text_fmt(guider_ui.screen_62_label_4, "%s\n%02d : %02d", typeStr, hour, minute);
    }

    // 根据Type2显示对应的类型名称和时间
    const char *typeStr2 = "";
    if (CustonmTimers[g_TimerPreIndex].Type2 == DefaultType)
        typeStr2 = "none";
    else if (CustonmTimers[g_TimerPreIndex].Type2 == FavType)
        typeStr2 = "fav";
    else if (CustonmTimers[g_TimerPreIndex].Type2 == FullOpenType)
        typeStr2 = "FullOpen";
    else if (CustonmTimers[g_TimerPreIndex].Type2 == FullCloseType)
        typeStr2 = "FullClose";
    else if (CustonmTimers[g_TimerPreIndex].Type2 == PctType)
    {
        if (CustonmTimers[g_TimerPreIndex].TypePct2 == 0)
            typeStr2 = "PCT0";
        else if (CustonmTimers[g_TimerPreIndex].TypePct2 == 10)
            typeStr2 = "PCT10";
        else if (CustonmTimers[g_TimerPreIndex].TypePct2 == 20)
            typeStr2 = "PCT20";
        else if (CustonmTimers[g_TimerPreIndex].TypePct2 == 30)
            typeStr2 = "PCT30";
        else if (CustonmTimers[g_TimerPreIndex].TypePct2 == 40)
            typeStr2 = "PCT40";
        else if (CustonmTimers[g_TimerPreIndex].TypePct2 == 50)
            typeStr2 = "PCT50";
        else if (CustonmTimers[g_TimerPreIndex].TypePct2 == 60)
            typeStr2 = "PCT60";
        else if (CustonmTimers[g_TimerPreIndex].TypePct2 == 70)
            typeStr2 = "PCT70";
        else if (CustonmTimers[g_TimerPreIndex].TypePct2 == 80)
            typeStr2 = "PCT80";
        else if (CustonmTimers[g_TimerPreIndex].TypePct2 == 90)
            typeStr2 = "PCT90";
        else if (CustonmTimers[g_TimerPreIndex].TypePct2 == 100)
            typeStr2 = "PCT100";
    }

    // 格式化Type2时间并判断AM/PM
    uint8_t hour2 = CustonmTimers[g_TimerPreIndex].Type2Hours;
    uint8_t minute2 = CustonmTimers[g_TimerPreIndex].Type2Minutes;
    
    if (CustonmTimers[g_TimerPreIndex].Format == FORMAT_12HOUSRS)
    {
        // 12小时制：Hours 是 0-11，使用 Type2_AM_PM 判断AM/PM
        const char *ampm2 = CustonmTimers[g_TimerPreIndex].Type2_AM_PM ? "PM" : "AM";
        lv_label_set_text_fmt(guider_ui.screen_62_label_5, "%s\n%02d : %02d %s", typeStr2, hour2, minute2, ampm2);
    }
    else
    {
        // 24小时制：Hours 是 0-23，直接显示
        lv_label_set_text_fmt(guider_ui.screen_62_label_5, "%s\n%02d : %02d", typeStr2, hour2, minute2);
    }

    lv_obj_set_style_bg_color(guider_ui.screen_62_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_62_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_62_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_62_btn_6, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_62_btn_7, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_62_btn_8, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_62_btn_9, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_62_btn_10, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_62_btn_11, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_62_btn_12, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 根据Weekdays字段，发送对应星期的label点击事件
    // bit0=周一(label_6), bit1=周二(label_7), ..., bit6=周日(label_12)
    for (volatile uint8_t day = 1; day <= 7; day++)
    {
        if (IsWeekdayInTimer(g_TimerPreIndex, day))
        {
            switch (day)
            {
            case 1: // 周一
                lv_event_send(guider_ui.screen_62_btn_6, LV_EVENT_CLICKED, NULL);
                break;
            case 2: // 周二
                lv_event_send(guider_ui.screen_62_btn_7, LV_EVENT_CLICKED, NULL);
                break;
            case 3: // 周三
                lv_event_send(guider_ui.screen_62_btn_8, LV_EVENT_CLICKED, NULL);
                break;
            case 4: // 周四
                lv_event_send(guider_ui.screen_62_btn_9, LV_EVENT_CLICKED, NULL);
                break;
            case 5: // 周五
                lv_event_send(guider_ui.screen_62_btn_10, LV_EVENT_CLICKED, NULL);
                break;
            case 6: // 周六
                lv_event_send(guider_ui.screen_62_btn_11, LV_EVENT_CLICKED, NULL);
                break;
            case 7: // 周日
                lv_event_send(guider_ui.screen_62_btn_12, LV_EVENT_CLICKED, NULL);
                break;
            default:
                break;
            }
        }
    }

    // 高亮当前选中的按钮
    uint8_t selectedPos = (Index_Scr62 - TimerMenu62Cnt + TimerIdems) % TimerIdems;
    if (selectedPos < 3)
    {
        switch (selectedPos)
        {
        case 0:
            lv_event_send(guider_ui.screen_62_btn_1, LV_EVENT_CLICKED, NULL);
            break;
        case 1:
            lv_event_send(guider_ui.screen_62_btn_2, LV_EVENT_CLICKED, NULL);
            break;
        case 2:
            lv_event_send(guider_ui.screen_62_btn_3, LV_EVENT_CLICKED, NULL);
            break;
        }
    }
}
