#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Customdate.h"
extern current_screen_t current_screen, Pre_screen, Mem_Screen;
extern uint8_t g_TimerPreIndex;
#define CalendarIdems 7u // 子状态数量
const char *CalendarMenuTexts[CalendarIdems] =
    {
        "Monday",    // 0
        "Tuesday",   // 1
        "Wednesday", // 2
        "Thursday",  // 3
        "Friday",    // 4
        "Saturday",  // 5
        "Sunday",    // 6
};
static uint8_t Index_Scr63, CalendarMenu63Cnt = 0;
void Screen_63_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG) // 向下滚动
    {
        if (Index_Scr63 == CalendarIdems - 1)
        {
            // 当前在最后一项，跳转到第一项
            Index_Scr63 = 0;
            CalendarMenu63Cnt = 0;
        }
        else
        {
            Index_Scr63++;

            // 当选中项在显示区域的最后两个时滚动显示
            if (Index_Scr63 >= CalendarMenu63Cnt + 5)
            {
                CalendarMenu63Cnt = (CalendarMenu63Cnt + 1) % CalendarIdems;
            }
        }
    }
    else if (Keyid == KEYFORWARD_TRG) // 向上滚动
    {
        if (Index_Scr63 == 0)
        {
            // 当前在第一项，跳转到最后一项
            Index_Scr63 = CalendarIdems - 1;
            // 设置显示区域为最后5项的起始位置
            CalendarMenu63Cnt = (CalendarIdems >= 5) ? (CalendarIdems - 5) : 0;
        }
        else
        {
            Index_Scr63--;

            // 当选中项在显示区域的前两个时滚动显示
            if (Index_Scr63 < CalendarMenu63Cnt)
            {
                CalendarMenu63Cnt = (CalendarMenu63Cnt + CalendarIdems - 1) % CalendarIdems;
            }
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {

        lv_event_send(guider_ui.screen_63_label_2, LV_EVENT_CLICKED, NULL); // 加载界面screen_62
        current_screen = SCREEN_62;
        Screen62date_preloading();
        return;
    }
    else if (Keyid == KEYMENU_TRG)
    {
        S_TIMER_EXECUTE_INFO nextTimer;
        RTC_Set_Next_AlarmTimer(&nextTimer);
        SaveDataToFlash();                                                  // 保存到Flash
        lv_event_send(guider_ui.screen_63_label_2, LV_EVENT_CLICKED, NULL); // 加载界面screen_62
        current_screen = SCREEN_62;
        Screen62date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        // 切换对应星期的状态
        // Index_Scr63: 0=周一, 1=周二, ..., 6=周日
        // Weekdays位映射: bit0=周一, bit1=周二, ..., bit6=周日
        uint8_t weekday = Index_Scr63 + 1; // 转换为星期号(1-7)
        ToggleWeekdayInTimer(g_TimerPreIndex, weekday);
    }
    Screen63date_preloading();
}
void Screen63date_preloading(void)
{
    lv_obj_set_size(guider_ui.screen_63_line_2, 4, Index_Scr63 * (210 / CalendarIdems));
    char display_text[21]; // 19字节最大需求 + 1
    char *current_data = CustonmTimers[g_TimerPreIndex].DataArray;

    strcpy(display_text, "Schedule ");
    if (strlen(current_data) > 15)
    {
        strncat(display_text, current_data, 15);
        strcat(display_text, "...");
    }
    else
    {
        strcat(display_text, (const char *)current_data);
    }
    lv_label_set_text(guider_ui.screen_63_label_2, display_text);
    // 设置5个按钮的文本（循环显示）
    for (int i = 0; i < 5; i++)
    {
        uint8_t itemIdx = (CalendarMenu63Cnt + i) % CalendarIdems;

        switch (i)
        {
        case 0:
            lv_label_set_text(guider_ui.screen_63_btn_1_label, CalendarMenuTexts[itemIdx]);
            break;
        case 1:
            lv_label_set_text(guider_ui.screen_63_btn_2_label, CalendarMenuTexts[itemIdx]);
            break;
        case 2:
            lv_label_set_text(guider_ui.screen_63_btn_3_label, CalendarMenuTexts[itemIdx]);
            break;
        case 3:
            lv_label_set_text(guider_ui.screen_63_btn_4_label, CalendarMenuTexts[itemIdx]);
            break;
        case 4:
            lv_label_set_text(guider_ui.screen_63_btn_5_label, CalendarMenuTexts[itemIdx]);
            break;
        }
    }

    lv_obj_set_style_bg_color(guider_ui.screen_63_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_63_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_63_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_63_btn_4, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_63_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_63_btn_6, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_63_btn_7, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_63_btn_8, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_63_btn_9, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_63_btn_10, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 为每个可见项设置对应的复选框状态
    for (int i = 0; i < 5; i++)
    {
        uint8_t itemIdx = (CalendarMenu63Cnt + i) % CalendarIdems;
        lv_obj_t *check_label = NULL;

        // 直接从全局变量获取当前显示项的状态
        // itemIdx: 0=周一, 1=周二, ..., 6=周日
        // 转换为 weekday (1-7)
        uint8_t weekday = itemIdx + 1;
        uint8_t state = IsWeekdayInTimer(g_TimerPreIndex, weekday);

        // 获取对应的复选框标签
        switch (i)
        {
        case 0:
            check_label = guider_ui.screen_63_btn_6_label;
            break;
        case 1:
            check_label = guider_ui.screen_63_btn_7_label;
            break;
        case 2:
            check_label = guider_ui.screen_63_btn_8_label;
            break;
        case 3:
            check_label = guider_ui.screen_63_btn_9_label;
            break;
        case 4:
            check_label = guider_ui.screen_63_btn_10_label;
            break;
        }

        // 设置复选框显示状态
        if (state)
        {
            lv_obj_clear_flag(check_label, LV_OBJ_FLAG_HIDDEN); // 选中：显示复选框
        }
        else
        {
            lv_obj_add_flag(check_label, LV_OBJ_FLAG_HIDDEN); // 未选中：隐藏复选框
        }
    }

    // 高亮当前选中的按钮
    uint8_t selectedPos = (Index_Scr63 - CalendarMenu63Cnt + CalendarIdems) % CalendarIdems;
    if (selectedPos < 5)
    {
        switch (selectedPos)
        {
        case 0:
            lv_event_send(guider_ui.screen_63_btn_1, LV_EVENT_CLICKED, NULL);
            lv_event_send(guider_ui.screen_63_btn_6, LV_EVENT_CLICKED, NULL);
            break;
        case 1:
            lv_event_send(guider_ui.screen_63_btn_2, LV_EVENT_CLICKED, NULL);
            lv_event_send(guider_ui.screen_63_btn_7, LV_EVENT_CLICKED, NULL);
            break;
        case 2:
            lv_event_send(guider_ui.screen_63_btn_3, LV_EVENT_CLICKED, NULL);
            lv_event_send(guider_ui.screen_63_btn_8, LV_EVENT_CLICKED, NULL);
            break;
        case 3:
            lv_event_send(guider_ui.screen_63_btn_4, LV_EVENT_CLICKED, NULL);
            lv_event_send(guider_ui.screen_63_btn_9, LV_EVENT_CLICKED, NULL);
            break;
        case 4:
            lv_event_send(guider_ui.screen_63_btn_5, LV_EVENT_CLICKED, NULL);
            lv_event_send(guider_ui.screen_63_btn_10, LV_EVENT_CLICKED, NULL);
            break;
        }
    }
}
