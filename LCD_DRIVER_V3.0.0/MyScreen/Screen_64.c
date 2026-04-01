#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Customdate.h"
#include <string.h>
#include <stdio.h>

extern current_screen_t current_screen, Pre_screen, Mem_Screen;
extern uint8_t g_TimerPreIndex;

// 定义项目类型
typedef enum
{
    ITEM_TYPE_GROUP = 0,
    ITEM_TYPE_SHADE = 1
} ItemType_t;

// 定义显示项结构
typedef struct
{
    ItemType_t type;  // 类型：群组或通道
    uint8_t index;    // 在对应数组中的索引
    const char *text; // 显示文本
} DisplayItem_t;

static uint8_t Index_Scr64, Menu64Cnt = 0;
static DisplayItem_t display_items[ARRAY_SIZE * 2]; // 最多32个（16群组+16通道）
static uint8_t total_items = 0;

void Screen_64_Movie(unsigned int Keyid)
{
    // 收集所有 Used=1 的群组和通道
    total_items = 0;

    // 先收集群组
    for (uint8_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (CustomGroupDates[i].Used)
        {
            display_items[total_items].type = ITEM_TYPE_GROUP;
            display_items[total_items].index = i;
            display_items[total_items].text = CustomGroupDates[i].DataArray;
            total_items++;
        }
    }

    // 再收集通道
    for (uint8_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (CustomShadeDates[i].Used)
        {
            display_items[total_items].type = ITEM_TYPE_SHADE;
            display_items[total_items].index = i;
            display_items[total_items].text = CustomShadeDates[i].DataArray;
            total_items++;
        }
    }

    if (total_items == 0)
    {
        // 如果没有可用的项目，返回上一级
        lv_event_send(guider_ui.screen_64_label_12, LV_EVENT_CLICKED, NULL);
        current_screen = SCREEN_62;
        Screen62date_preloading();
        return;
    }

    if (Keyid == KEYBEHIND_TRG) // 向下滚动
    {
        if (Index_Scr64 == total_items - 1)
        {
            Index_Scr64 = 0;
            Menu64Cnt = 0;
        }
        else
        {
            Index_Scr64++;
            if (Index_Scr64 >= Menu64Cnt + 5)
            {
                Menu64Cnt++;
            }
        }
    }
    else if (Keyid == KEYFORWARD_TRG) // 向上滚动
    {
        if (Index_Scr64 == 0)
        {
            Index_Scr64 = total_items - 1;
            Menu64Cnt = (total_items >= 5) ? (total_items - 5) : 0;
        }
        else
        {
            Index_Scr64--;
            if (Index_Scr64 < Menu64Cnt)
            {
                Menu64Cnt = Index_Scr64;
            }
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_64_label_12, LV_EVENT_CLICKED, NULL);
        current_screen = SCREEN_62;
        Screen62date_preloading();
        return;
    }
    else if (Keyid == KEYMENU_TRG)
    {
        // ✅ 调试：打印保存前的数据
        // printf("\n========== [Screen_64 MENU Save] ==========\n");
        // printf("Timer Index: %d\n", g_TimerPreIndex);
        // printf("TimerShadesGroups: 0x%08X\n", CustonmTimers[g_TimerPreIndex].TimerShadesGroups);
        // printf("ContainShades: 0x%04X\n", CustonmTimers[g_TimerPreIndex].ContainShades);

        // 解析并显示选中的群组
        // printf("Selected Groups: ");
        for (uint8_t i = 0; i < ARRAY_SIZE; i++)
        {
            if (CustonmTimers[g_TimerPreIndex].TimerShadesGroups & (1UL << i))
            {
                // printf("%d ", i);
            }
        }
        // printf("\n");

        // 解析并显示选中的通道（索引）
        // printf("Selected Shades (index): ");
        for (uint8_t i = 0; i < ARRAY_SIZE; i++)
        {
            if (CustonmTimers[g_TimerPreIndex].TimerShadesGroups & (1UL << (16 + i)))
            {
                // printf("%d ", i);
            }
        }
        // printf("\n");

        // 解析并显示 ContainShades 包含的通道号
        uint16_t contain = CustonmTimers[g_TimerPreIndex].ContainShades;
        uint8_t low = contain & 0xFF;
        uint8_t high = (contain >> 8) & 0xFF;
        uint16_t mask = (high) | (low << 8);
        // printf("ContainShades channels: ");
        for (uint8_t i = 0; i < 16; i++)
        {
            if (mask & (1 << i))
            {
                // printf("%d ", i + 1);
            }
        }
        // printf("\n");

        // 打印每个选中通道的 CurShade 值
        // printf("Shade Index -> CurShade mapping: ");
        for (uint8_t i = 0; i < ARRAY_SIZE; i++)
        {
            if (CustonmTimers[g_TimerPreIndex].TimerShadesGroups & (1UL << (16 + i)))
            {
                // printf("[%d->%d] ", i, CustomShadeDates[i].CurShade);
            }
        }
        // printf("\n==========================================\n\n");

        S_TIMER_EXECUTE_INFO nextTimer;
        RTC_Set_Next_AlarmTimer(&nextTimer);
        SaveDataToFlash(); // 保存到Flash
        lv_event_send(guider_ui.screen_64_label_12, LV_EVENT_CLICKED, NULL);
        current_screen = SCREEN_62;
        Screen62date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        // 切换对应项的选中状态
        DisplayItem_t *item = &display_items[Index_Scr64];

        // printf("\n[Screen64 ENTER] Index_Scr64=%d, Type=%d, ItemIndex=%d\n",Index_Scr64, item->type, item->index);

               if (item->type == ITEM_TYPE_SHADE)
               {
                   // 通道：切换选中状态（已选中则取消，未选中则选中）
                   uint8_t before_state = IsShadeInTimer(g_TimerPreIndex, item->index);
                   // printf("  UI Check Before: IsShadeInTimer(%d, %d) = %d\n",g_TimerPreIndex, item->index, before_state);
                   ToggleShadeInTimer(g_TimerPreIndex, item->index);

                   uint8_t after_state = IsShadeInTimer(g_TimerPreIndex, item->index);
                   // printf("  UI Check After: IsShadeInTimer(%d, %d) = %d\n",g_TimerPreIndex, item->index, after_state);
               }
               else if (item->type == ITEM_TYPE_GROUP)
               {
                   // 群组：切换选中状态（已选中则取消，未选中则选中）
                   ToggleGroupInTimer(g_TimerPreIndex, item->index);
               }
    }
    Screen64date_preloading();
}

void Screen64date_preloading(void)
{
    // 收集所有 Used=1 的群组和通道
    total_items = 0;

    // 先收集群组
    for (uint8_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (CustomGroupDates[i].Used)
        {
            display_items[total_items].type = ITEM_TYPE_GROUP;
            display_items[total_items].index = i;
            display_items[total_items].text = CustomGroupDates[i].DataArray;
            total_items++;
        }
    }

    // 再收集通道
    for (uint8_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (CustomShadeDates[i].Used)
        {
            display_items[total_items].type = ITEM_TYPE_SHADE;
            display_items[total_items].index = i;
            display_items[total_items].text = CustomShadeDates[i].DataArray;
            total_items++;
        }
    }

    if (total_items == 0)
    {
        return;
    }

    lv_obj_set_size(guider_ui.screen_64_line_2, 4, total_items > 0 ? (Index_Scr64 * 210 / total_items) : 0);

    // 设置5个按钮的文本和可见性
    lv_obj_t *buttons[5] = {
        guider_ui.screen_64_btn_1,
        guider_ui.screen_64_btn_2,
        guider_ui.screen_64_btn_3,
        guider_ui.screen_64_btn_4,
        guider_ui.screen_64_btn_5};

    lv_obj_t *button_labels[5] = {
        guider_ui.screen_64_btn_1_label,
        guider_ui.screen_64_btn_2_label,
        guider_ui.screen_64_btn_3_label,
        guider_ui.screen_64_btn_4_label,
        guider_ui.screen_64_btn_5_label};

    lv_obj_t *check_labels[5] = {
        guider_ui.screen_64_btn_6_label,
        guider_ui.screen_64_btn_7_label,
        guider_ui.screen_64_btn_8_label,
        guider_ui.screen_64_btn_9_label,
        guider_ui.screen_64_btn_10_label};

    lv_obj_t *check_buttons[5] = {
        guider_ui.screen_64_btn_6,
        guider_ui.screen_64_btn_7,
        guider_ui.screen_64_btn_8,
        guider_ui.screen_64_btn_9,
        guider_ui.screen_64_btn_10};

    for (int i = 0; i < 5; i++)
    {
        uint8_t item_pos = Menu64Cnt + i;

        if (item_pos < total_items)
        {
            // 显示按钮和文本
            DisplayItem_t *item = &display_items[item_pos];
            lv_label_set_text(button_labels[i], item->text);
            lv_obj_clear_flag(buttons[i], LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(check_buttons[i], LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
            // 隐藏多余的按钮
            lv_obj_add_flag(buttons[i], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(check_buttons[i], LV_OBJ_FLAG_HIDDEN);
        }
    }

    // 重置所有按钮背景色
    lv_obj_set_style_bg_color(guider_ui.screen_64_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_64_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_64_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_64_btn_4, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_64_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_64_btn_6, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_64_btn_7, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_64_btn_8, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_64_btn_9, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_64_btn_10, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 为每个可见项设置对应的复选框状态
    for (int i = 0; i < 5; i++)
    {
        uint8_t item_pos = Menu64Cnt + i;

        if (item_pos < total_items)
        {
            DisplayItem_t *item = &display_items[item_pos];
            uint8_t state = 0;

            if (item->type == ITEM_TYPE_SHADE)
            {
                // 通道：直接检查索引是否选中
                state = IsShadeInTimer(g_TimerPreIndex, item->index);
            }
            else if (item->type == ITEM_TYPE_GROUP)
            {
                // 群组：直接检查索引是否选中
                state = IsGroupInTimer(g_TimerPreIndex, item->index);
            }

            // 设置复选框显示状态
            if (state)
            {
                lv_obj_clear_flag(check_labels[i], LV_OBJ_FLAG_HIDDEN); // 选中：显示复选框
            }
            else
            {
                lv_obj_add_flag(check_labels[i], LV_OBJ_FLAG_HIDDEN); // 未选中：隐藏复选框
            }
        }
        else
        {
            // 如果没有对应项，隐藏复选框
            lv_obj_add_flag(check_labels[i], LV_OBJ_FLAG_HIDDEN);
        }
    }

    // 高亮当前选中的按钮
    uint8_t selectedPos = Index_Scr64 - Menu64Cnt;
    if (selectedPos < 5 && Index_Scr64 < total_items)
    {
        switch (selectedPos)
        {
        case 0:
            lv_event_send(guider_ui.screen_64_btn_1, LV_EVENT_CLICKED, NULL);
            lv_event_send(guider_ui.screen_64_btn_6, LV_EVENT_CLICKED, NULL);
            break;
        case 1:
            lv_event_send(guider_ui.screen_64_btn_2, LV_EVENT_CLICKED, NULL);
            lv_event_send(guider_ui.screen_64_btn_7, LV_EVENT_CLICKED, NULL);
            break;
        case 2:
            lv_event_send(guider_ui.screen_64_btn_3, LV_EVENT_CLICKED, NULL);
            lv_event_send(guider_ui.screen_64_btn_8, LV_EVENT_CLICKED, NULL);
            break;
        case 3:
            lv_event_send(guider_ui.screen_64_btn_4, LV_EVENT_CLICKED, NULL);
            lv_event_send(guider_ui.screen_64_btn_9, LV_EVENT_CLICKED, NULL);
            break;
        case 4:
            lv_event_send(guider_ui.screen_64_btn_5, LV_EVENT_CLICKED, NULL);
            lv_event_send(guider_ui.screen_64_btn_10, LV_EVENT_CLICKED, NULL);
            break;
        }
    }
}
