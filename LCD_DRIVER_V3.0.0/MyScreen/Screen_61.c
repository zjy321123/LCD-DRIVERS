#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "Customdate.h"

extern lv_indev_t *indev_keypad;
extern lv_ui guider_ui;            // 声明 界面对象
static uint8_t TimerMenuCnt = 0;   // 当前选中的项目索引（总列表中的位置）
static uint8_t TimerMenuStart = 0; // 显示区域的起始索引
extern uint8_t g_CacheCmd;
extern current_screen_t current_screen, Mem_Screen;
uint8_t g_TimerPreIndex;
void Screen_61_Movie(unsigned int Keyid)
{
    // 收集所有Used=1的定时器（被使用的定时器）
    uint8_t used_timers[TIMERS_SIZE] = {0};
    uint8_t total_count = 0;

    // 收集Used=1的定时器索引
    for (uint8_t i = 0; i < TIMERS_SIZE && total_count < TIMERS_SIZE; i++)
    {
        if (CustonmTimers[i].Used)
        {
            used_timers[total_count] = i;
            total_count++;
        }
    }

    uint8_t totalItems = total_count + 1; // +1 因为有"Add Timer"项

    if (Keyid == KEYFORWARD_TRG) // 向上滚动
    {
        if (TimerMenuCnt == 0)
        {
            // 当前在第一项，跳转到最后一项
            TimerMenuCnt = totalItems - 1;
            // 设置显示区域为最后5项的起始位置
            TimerMenuStart = (totalItems >= 5) ? (totalItems - 5) : 0;
        }
        else
        {
            TimerMenuCnt--;

            // 当选中项在显示区域的前面时滚动显示
            if (TimerMenuCnt < TimerMenuStart)
            {
                TimerMenuStart = TimerMenuCnt;
            }
        }
    }
    else if (Keyid == KEYBEHIND_TRG) // 向下滚动
    {
        if (TimerMenuCnt == totalItems - 1)
        {
            // 当前在最后一项，跳转到第一项
            TimerMenuCnt = 0;
            TimerMenuStart = 0;
        }
        else
        {
            TimerMenuCnt++;

            // 当选中项在显示区域的后面时滚动显示
            if (TimerMenuCnt >= TimerMenuStart + 5)
            {
                TimerMenuStart = TimerMenuCnt - 4;
            }
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_61_label_7, LV_EVENT_CLICKED, NULL); // 加载界面Screen_1
        current_screen = SCREEN_1;
        Screen1date_preloading();
        return;
    }
    else if (Keyid == KEYMENU_TRG)
    {
        // 根据当前选中的按钮，跳转到对应的编辑界面
        if (TimerMenuCnt > 0) // 排除"Add Timer"项
        {
            uint8_t item_index = TimerMenuCnt - 1;
            if (item_index < total_count)
            {
                g_TimerPreIndex = used_timers[item_index]; // 使用实际的定时器索引
            }
            lv_event_send(guider_ui.screen_61_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_62
            current_screen = SCREEN_62;
            Screen62date_preloading();
        }
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (TimerMenuCnt == 0)
        {
            // 第一项是"Add Timer"，跳转到添加定时器界面
            lv_event_send(guider_ui.screen_61_btn_1, LV_EVENT_PRESSED, NULL);
            current_screen = SCREEN_9;
            Mem_Screen = SCREEN_61; // 屏幕记忆点，便于跳转
            Screen9date_preloading();
            return;
        }
        else
        {
            // 切换定时器的TimerEnable状态（开关ON/OFF）
            uint8_t item_index = TimerMenuCnt - 1; // 减1因为第一项是"Add Timer"

            if (item_index < total_count)
            {
                uint8_t timer_idx = used_timers[item_index];
                CustonmTimers[timer_idx].TimerEnable = !CustonmTimers[timer_idx].TimerEnable;
                S_TIMER_EXECUTE_INFO nextTimer;
                RTC_Set_Next_AlarmTimer(&nextTimer);
                SaveDataToFlash(); // 保存到Flash
            }
        }
    }

    Screen61date_preloading();
}

void Screen61date_preloading(void)
{
    // 收集所有Used=1的定时器（被使用的定时器）
    uint8_t used_timers[TIMERS_SIZE] = {0};
    uint8_t total_count = 0;

    // 收集Used=1的定时器索引
    for (uint8_t i = 0; i < TIMERS_SIZE && total_count < TIMERS_SIZE; i++)
    {
        if (CustonmTimers[i].Used)
        {
            used_timers[total_count] = i;
            total_count++;
        }
    }

    uint8_t totalItems = total_count + 1; // +1 因为第一个是"Add Timer"

    // 确保选中项在有效范围内
    if (TimerMenuCnt >= totalItems)
    {
        TimerMenuCnt = totalItems - 1;
    }

    // 根据总项目数量显示/隐藏按钮
    if (totalItems <= 1)
    {
        lv_obj_clear_flag(guider_ui.screen_61_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_sw_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_btn_5, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_sw_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_sw_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_sw_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_sw_5, LV_OBJ_FLAG_HIDDEN);
    }
    else if (totalItems == 2)
    {
        lv_obj_clear_flag(guider_ui.screen_61_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_sw_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_sw_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_btn_5, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_sw_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_sw_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_sw_5, LV_OBJ_FLAG_HIDDEN);
    }
    else if (totalItems == 3)
    {
        lv_obj_clear_flag(guider_ui.screen_61_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_sw_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_sw_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_sw_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_btn_5, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_sw_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_sw_5, LV_OBJ_FLAG_HIDDEN);
    }
    else if (totalItems == 4)
    {
        lv_obj_clear_flag(guider_ui.screen_61_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_sw_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_sw_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_sw_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_sw_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_btn_5, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_61_sw_5, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_clear_flag(guider_ui.screen_61_sw_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_sw_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_sw_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_sw_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_sw_5, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_61_btn_5, LV_OBJ_FLAG_HIDDEN);
    }

    // 重置按钮样式
    lv_obj_set_style_text_color(guider_ui.screen_61_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_61_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_61_btn_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_61_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_61_btn_3, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_61_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_61_btn_4, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_61_btn_4, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_61_btn_5, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_61_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 设置5个按钮的文本（从TimerMenuStart开始）
    for (int i = 0; i < 5; i++)
    {
        uint8_t displayIndex = TimerMenuStart + i;

        // 确保索引在有效范围内
        if (displayIndex >= totalItems)
        {
            // 超出范围，清空按钮文本并隐藏按钮
            switch (i)
            {
            case 0:
                lv_label_set_text(guider_ui.screen_61_btn_1_label, "");
                lv_obj_add_flag(guider_ui.screen_61_btn_1, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(guider_ui.screen_61_sw_1, LV_OBJ_FLAG_HIDDEN);
                break;
            case 1:
                lv_label_set_text(guider_ui.screen_61_btn_2_label, "");
                lv_obj_add_flag(guider_ui.screen_61_btn_2, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(guider_ui.screen_61_sw_2, LV_OBJ_FLAG_HIDDEN);
                break;
            case 2:
                lv_label_set_text(guider_ui.screen_61_btn_3_label, "");
                lv_obj_add_flag(guider_ui.screen_61_btn_3, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(guider_ui.screen_61_sw_3, LV_OBJ_FLAG_HIDDEN);
                break;
            case 3:
                lv_label_set_text(guider_ui.screen_61_btn_4_label, "");
                lv_obj_add_flag(guider_ui.screen_61_btn_4, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(guider_ui.screen_61_sw_4, LV_OBJ_FLAG_HIDDEN);
                break;
            case 4:
                lv_label_set_text(guider_ui.screen_61_btn_5_label, "");
                lv_obj_add_flag(guider_ui.screen_61_btn_5, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(guider_ui.screen_61_sw_5, LV_OBJ_FLAG_HIDDEN);
                break;
            }
            continue;
        }

        char itemName[STRING_SIZE] = {0};
        uint8_t timer_enabled = 0;

        if (displayIndex == 0)
        {
            // 第一项是"Add Timer"
            strcpy(itemName, "Add Timer");
        }
        else
        {
            // 其他项是定时器名称（Used=1）
            uint8_t item_index = displayIndex - 1; // 减1因为第一项是"Add Timer"

            if (item_index < total_count)
            {
                uint8_t timer_idx = used_timers[item_index];
                // 直接使用定时器的DataArray名称
                strncpy(itemName, CustonmTimers[timer_idx].DataArray, STRING_SIZE);
                itemName[STRING_SIZE - 1] = '\0';
                // TimerEnable只用于显示开关状态
                timer_enabled = CustonmTimers[timer_idx].TimerEnable;
            }
        }

        // 设置按钮文本
        switch (i)
        {
        case 0:
            lv_label_set_text(guider_ui.screen_61_btn_1_label, itemName);
            // 处理开关
            if (displayIndex == 0)
            {
                lv_obj_add_flag(guider_ui.screen_61_sw_1, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                lv_obj_clear_flag(guider_ui.screen_61_sw_1, LV_OBJ_FLAG_HIDDEN);
                if (timer_enabled)
                    lv_obj_add_state(guider_ui.screen_61_sw_1, LV_STATE_CHECKED);
                else
                    lv_obj_clear_state(guider_ui.screen_61_sw_1, LV_STATE_CHECKED);
            }
            break;
        case 1:
            lv_label_set_text(guider_ui.screen_61_btn_2_label, itemName);
            if (displayIndex == 0)
            {
                lv_obj_add_flag(guider_ui.screen_61_sw_2, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                lv_obj_clear_flag(guider_ui.screen_61_sw_2, LV_OBJ_FLAG_HIDDEN);
                if (timer_enabled)
                    lv_obj_add_state(guider_ui.screen_61_sw_2, LV_STATE_CHECKED);
                else
                    lv_obj_clear_state(guider_ui.screen_61_sw_2, LV_STATE_CHECKED);
            }
            break;
        case 2:
            lv_label_set_text(guider_ui.screen_61_btn_3_label, itemName);
            if (displayIndex == 0)
            {
                lv_obj_add_flag(guider_ui.screen_61_sw_3, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                lv_obj_clear_flag(guider_ui.screen_61_sw_3, LV_OBJ_FLAG_HIDDEN);
                if (timer_enabled)
                    lv_obj_add_state(guider_ui.screen_61_sw_3, LV_STATE_CHECKED);
                else
                    lv_obj_clear_state(guider_ui.screen_61_sw_3, LV_STATE_CHECKED);
            }
            break;
        case 3:
            lv_label_set_text(guider_ui.screen_61_btn_4_label, itemName);
            if (displayIndex == 0)
            {
                lv_obj_add_flag(guider_ui.screen_61_sw_4, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                lv_obj_clear_flag(guider_ui.screen_61_sw_4, LV_OBJ_FLAG_HIDDEN);
                if (timer_enabled)
                    lv_obj_add_state(guider_ui.screen_61_sw_4, LV_STATE_CHECKED);
                else
                    lv_obj_clear_state(guider_ui.screen_61_sw_4, LV_STATE_CHECKED);
            }
            break;
        case 4:
            lv_label_set_text(guider_ui.screen_61_btn_5_label, itemName);
            if (displayIndex == 0)
            {
                lv_obj_add_flag(guider_ui.screen_61_sw_5, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                lv_obj_clear_flag(guider_ui.screen_61_sw_5, LV_OBJ_FLAG_HIDDEN);
                if (timer_enabled)
                    lv_obj_add_state(guider_ui.screen_61_sw_5, LV_STATE_CHECKED);
                else
                    lv_obj_clear_state(guider_ui.screen_61_sw_5, LV_STATE_CHECKED);
            }
            break;
        }
    }

    // 高亮当前选中的按钮
    uint8_t selectedPos = TimerMenuCnt - TimerMenuStart;
    if (selectedPos < 5)
    {
        switch (selectedPos)
        {
        case 0:
            lv_event_send(guider_ui.screen_61_btn_1, LV_EVENT_CLICKED, NULL);
            break;
        case 1:
            lv_event_send(guider_ui.screen_61_btn_2, LV_EVENT_CLICKED, NULL);
            break;
        case 2:
            lv_event_send(guider_ui.screen_61_btn_3, LV_EVENT_CLICKED, NULL);
            break;
        case 3:
            lv_event_send(guider_ui.screen_61_btn_4, LV_EVENT_CLICKED, NULL);
            break;
        case 4:
            lv_event_send(guider_ui.screen_61_btn_5, LV_EVENT_CLICKED, NULL);
            break;
        }
    }
}
