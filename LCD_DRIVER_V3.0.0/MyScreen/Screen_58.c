#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "Customdate.h"
#include "log.h"
#include <stdio.h>
extern lv_indev_t *indev_keypad;
extern lv_ui guider_ui; // 声明 界面对象
// 当前选中的项索引和显示的第一项索引
static uint8_t GroupContianShadeCnt = 0;
static uint8_t GroupContianShadeStart = 0;
static uint16_t GroupContainShadesBackup = 0;
static uint8_t GroupContainBackupValid = 0;
static uint8_t GroupContainBackupIndex = 0;
extern uint8_t g_CacheCmd, GroupPreIndex;
extern current_screen_t current_screen, Mem_Screen;

void Screen58StoreGroupBackup(void)
{
    GroupContainShadesBackup = CustomGroupDates[GroupPreIndex].ContainShades;
    GroupContainBackupIndex = GroupPreIndex;
    GroupContainBackupValid = 1;
}

void Screen_58_Movie(unsigned int Keyid)
{

    // 收集所有可见的shade项索引
    uint8_t visibleShades[ARRAY_SIZE] = {0};
    uint8_t visibleCount = 0;

    for (uint8_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (CustomShadeDates[i].Show)
        {
            visibleShades[visibleCount] = i;
            visibleCount++;
        }
    }

    uint8_t totalItems = visibleCount;

    if (Keyid == KEYFORWARD_TRG) // 向上滚动
    {
        if (GroupContianShadeCnt == 0)
        {
            // 当前在第一项，跳转到最后一项
            GroupContianShadeCnt = totalItems - 1;
            // 设置显示区域为最后5项的起始位置
            GroupContianShadeStart = (totalItems >= 5) ? (totalItems - 5) : 0;
        }
        else
        {
            GroupContianShadeCnt--;

            // 当选中项在显示区域的前两个时滚动显示
            if (GroupContianShadeCnt < GroupContianShadeStart)
            {
                GroupContianShadeStart = GroupContianShadeCnt; // 直接设置起始位置为当前选中项
            }
        }
    }
    else if (Keyid == KEYBEHIND_TRG) // 向下滚动
    {
        if (GroupContianShadeCnt == totalItems - 1)
        {
            // 当前在最后一项，跳转到第一项
            GroupContianShadeCnt = 0;
            GroupContianShadeStart = 0;
        }
        else
        {
            GroupContianShadeCnt++;

            // 当选中项在显示区域的最后两个时滚动显示
            if (GroupContianShadeCnt >= GroupContianShadeStart + 5)
            {
                GroupContianShadeStart = GroupContianShadeCnt - 4; // 直接调整起始位置
            }
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        if (GroupContainBackupValid)
        {
            CustomGroupDates[GroupContainBackupIndex].ContainShades = GroupContainShadesBackup;
            GroupContainBackupValid = 0;
        }
        if (Mem_Screen == SCREEN_59) // 有记忆点先跳转记忆点
        {
            // [表情] 关键修复：群组内容改变后，刷新所有定时器的 ContainShades
            RefreshAllTimersContainShades();
            
            // 清空所有群组和通道的选择状态，下次进来重新抓取最新状态。
            for (uint8_t i = 0; i < ARRAY_SIZE; i++)
            {
                CustomShadeDates[i].GroupShaded = 0;
                CustomGroupDates[i].GroupShaded = 0;
            }
            Mem_Screen = SCREEN_NUll;
            //SaveDataToFlash();
            lv_event_send(guider_ui.screen_58_label_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_59
            current_screen = SCREEN_59;
            Screen59date_preloading();
            return;
        }
        else
        {
            // [表情] 关键修复：群组内容改变后，刷新所有定时器的 ContainShades
            RefreshAllTimersContainShades();
            
            // 清空所有群组和通道的选择状态，下次进来重新抓取最新状态。
            for (uint8_t i = 0; i < ARRAY_SIZE; i++)
            {
                CustomShadeDates[i].GroupShaded = 0;
                CustomGroupDates[i].GroupShaded = 0;
            }
             Mem_Screen = SCREEN_NUll;
            SaveDataToFlash();
            lv_event_send(guider_ui.screen_58_label_2, LV_EVENT_CLICKED, NULL); // 加载界面Screen_57
            current_screen = SCREEN_57;
            Screen57date_preloading();
            return;
        }
    }
    else if (Keyid == KEYMENU_TRG)
    {
        // [表情] 关键修复：群组内容改变后，刷新所有定时器的 ContainShades
        RefreshAllTimersContainShades();
        
        // 清空所有群组和通道的选择状态，下次进来重新抓取最新状态。
        for (uint8_t i = 0; i < ARRAY_SIZE; i++)
        {
            CustomShadeDates[i].GroupShaded = 0;
            CustomGroupDates[i].GroupShaded = 0;
        }
        SaveDataToFlash();
        GroupContainBackupValid = 0;
        lv_event_send(guider_ui.screen_58_label_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_59
        current_screen = SCREEN_59;
        Screen59date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        // 确定当前选中的项的实际索引
        uint8_t actualIndex = visibleShades[GroupContianShadeCnt];
        
        // [表情] 修复：获取真实通道号
        uint8_t channelNum = CustomShadeDates[actualIndex].CurShade;

        // 切换对应索引的GroupShaded标志
        CustomShadeDates[actualIndex].GroupShaded = !CustomShadeDates[actualIndex].GroupShaded;
        if (IsChannelInGroup(GroupPreIndex, channelNum))  // [表情] 使用通道号
        {
            if (!(CustomShadeDates[actualIndex].GroupShaded)) // 如果在群组里，且未被选中则删除
                RemoveChannelFromGroup(GroupPreIndex, channelNum);  // [表情] 使用通道号
        }
        else
        {
            if (CustomShadeDates[actualIndex].GroupShaded) // 如果未在群组里，且被选中则添加
                AddChannelToGroup(GroupPreIndex, channelNum);  // [表情] 使用通道号
        }
    }
    Screen58date_preloading();
}
//
void Screen58date_preloading(void)
{
    if (!GroupContainBackupValid || GroupContainBackupIndex != GroupPreIndex)
    {
        Screen58StoreGroupBackup();
    }
    // 收集所有可见的shade项索引
    uint8_t visibleShades[ARRAY_SIZE] = {0};
    uint8_t visibleCount = 0;

    for (uint8_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (CustomShadeDates[i].Show)
        {
            visibleShades[visibleCount] = i;
            visibleCount++;
            // [表情] 修复：使用通道号而不是索引，并且必须同时处理选中和未选中状态
            uint8_t channelNum = CustomShadeDates[i].CurShade;
            if (IsChannelInGroup(GroupPreIndex, channelNum)) // 判断通道号在群组里是否存在
            {
                CustomShadeDates[i].GroupShaded = 1;
            }
            else
            {
                CustomShadeDates[i].GroupShaded = 0;  // 不在群组中则清除选中状态
            }
        }
    }

    uint8_t totalItems = visibleCount;

    // 确保选中项在有效范围内
    if (GroupContianShadeCnt >= totalItems)
    {
        GroupContianShadeCnt = totalItems - 1;
    }

    // 更新滚动条位置
    if (totalItems > 1)
    {
        lv_obj_set_size(guider_ui.screen_58_line_2, 4, GroupContianShadeCnt * (210 / (totalItems - 1)));
    }
    else
    {
        lv_obj_set_size(guider_ui.screen_58_line_2, 4, 0);
    }
    lv_obj_set_style_text_color(guider_ui.screen_58_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_58_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_58_btn_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_58_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_58_btn_3, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_58_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_58_btn_4, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_58_btn_4, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_58_btn_5, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_58_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_text_color(guider_ui.screen_58_btn_6, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_58_btn_6, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_58_btn_7, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_58_btn_7, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_58_btn_8, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_58_btn_8, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_58_btn_9, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_58_btn_9, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_58_btn_10, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_58_btn_10, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 根据总可见项数量显示/隐藏按钮
    if (totalItems <= 1)
    {
        lv_obj_clear_flag(guider_ui.screen_58_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_5, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_6, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_7, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_8, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_9, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_10, LV_OBJ_FLAG_HIDDEN);
    }
    else if (totalItems == 2)
    {
        lv_obj_clear_flag(guider_ui.screen_58_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_5, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_6, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_7, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_8, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_9, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_10, LV_OBJ_FLAG_HIDDEN);
    }
    else if (totalItems == 3)
    {
        lv_obj_clear_flag(guider_ui.screen_58_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_5, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_6, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_7, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_8, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_9, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_10, LV_OBJ_FLAG_HIDDEN);
    }
    else if (totalItems == 4)
    {
        lv_obj_clear_flag(guider_ui.screen_58_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_5, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_6, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_7, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_8, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_9, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_58_btn_10, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_clear_flag(guider_ui.screen_58_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_5, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_6, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_7, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_8, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_9, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_58_btn_10, LV_OBJ_FLAG_HIDDEN);
    }

    // 设置5个按钮的文本（从GroupContianShadeStart开始）
    for (int i = 0; i < 5; i++)
    {
        uint8_t displayIndex = GroupContianShadeStart + i;

        // 确保索引在有效范围内
        if (displayIndex >= totalItems)
        {
            // 超出范围，清空按钮文本
            switch (i)
            {
            case 0:
                lv_label_set_text(guider_ui.screen_58_btn_1_label, "");
                break;
            case 1:
                lv_label_set_text(guider_ui.screen_58_btn_2_label, "");
                break;
            case 2:
                lv_label_set_text(guider_ui.screen_58_btn_3_label, "");
                break;
            case 3:
                lv_label_set_text(guider_ui.screen_58_btn_4_label, "");
                break;
            case 4:
                lv_label_set_text(guider_ui.screen_58_btn_5_label, "");
                break;
            }
            continue;
        }

        char shadeName[STRING_SIZE] = {0};

        // 从CustomShadeDates获取的数据
        uint8_t actualIndex = visibleShades[displayIndex]; // 获取实际的shade索引
        strncpy(shadeName, (char *)CustomShadeDates[actualIndex].DataArray, STRING_SIZE);
        shadeName[STRING_SIZE - 1] = '\0'; // 确保字符串终止

        // 设置按钮文本
        switch (i)
        {
        case 0:
            lv_label_set_text(guider_ui.screen_58_btn_1_label, shadeName);
            break;
        case 1:
            lv_label_set_text(guider_ui.screen_58_btn_2_label, shadeName);
            break;
        case 2:
            lv_label_set_text(guider_ui.screen_58_btn_3_label, shadeName);
            break;
        case 3:
            lv_label_set_text(guider_ui.screen_58_btn_4_label, shadeName);
            break;
        case 4:
            lv_label_set_text(guider_ui.screen_58_btn_5_label, shadeName);
            break;
        }
    }

    // 高亮当前选中的按钮
    uint8_t selectedPos = GroupContianShadeCnt - GroupContianShadeStart;
    if (selectedPos < 5)
    {
        switch (selectedPos)
        {
        case 0:
            lv_event_send(guider_ui.screen_58_btn_1, LV_EVENT_CLICKED, NULL);
            lv_event_send(guider_ui.screen_58_btn_6, LV_EVENT_CLICKED, NULL);
            break;
        case 1:
            lv_event_send(guider_ui.screen_58_btn_2, LV_EVENT_CLICKED, NULL);
            lv_event_send(guider_ui.screen_58_btn_7, LV_EVENT_CLICKED, NULL);
            break;
        case 2:
            lv_event_send(guider_ui.screen_58_btn_3, LV_EVENT_CLICKED, NULL);
            lv_event_send(guider_ui.screen_58_btn_8, LV_EVENT_CLICKED, NULL);
            break;
        case 3:
            lv_event_send(guider_ui.screen_58_btn_4, LV_EVENT_CLICKED, NULL);
            lv_event_send(guider_ui.screen_58_btn_9, LV_EVENT_CLICKED, NULL);
            break;
        case 4:
            lv_event_send(guider_ui.screen_58_btn_5, LV_EVENT_CLICKED, NULL);
            lv_event_send(guider_ui.screen_58_btn_10, LV_EVENT_CLICKED, NULL);
            break;
        }
    }
    // 更新复选框标签的可见性
    // 现在基于实际索引的GroupShaded值来显示/隐藏复选标记
    for (int i = 0; i < 5; i++)
    {
        uint8_t displayIndex = GroupContianShadeStart + i;

        // 超出范围则隐藏复选框
        if (displayIndex >= totalItems)
        {
            switch (i)
            {
            case 0:
                lv_obj_add_flag(guider_ui.screen_58_btn_6_label, LV_OBJ_FLAG_HIDDEN);
                break;
            case 1:
                lv_obj_add_flag(guider_ui.screen_58_btn_7_label, LV_OBJ_FLAG_HIDDEN);
                break;
            case 2:
                lv_obj_add_flag(guider_ui.screen_58_btn_8_label, LV_OBJ_FLAG_HIDDEN);
                break;
            case 3:
                lv_obj_add_flag(guider_ui.screen_58_btn_9_label, LV_OBJ_FLAG_HIDDEN);
                break;
            case 4:
                lv_obj_add_flag(guider_ui.screen_58_btn_10_label, LV_OBJ_FLAG_HIDDEN);
                break;
            }
            continue;
        }
        // 获取实际索引
        uint8_t actualIndex = visibleShades[displayIndex];

        // 根据GroupShaded标志显示/隐藏复选标记
        switch (i)
        {
        case 0:
            if (CustomShadeDates[actualIndex].GroupShaded)
                lv_obj_clear_flag(guider_ui.screen_58_btn_6_label, LV_OBJ_FLAG_HIDDEN);
            else
                lv_obj_add_flag(guider_ui.screen_58_btn_6_label, LV_OBJ_FLAG_HIDDEN);
            break;
        case 1:
            if (CustomShadeDates[actualIndex].GroupShaded)
                lv_obj_clear_flag(guider_ui.screen_58_btn_7_label, LV_OBJ_FLAG_HIDDEN);
            else
                lv_obj_add_flag(guider_ui.screen_58_btn_7_label, LV_OBJ_FLAG_HIDDEN);
            break;
        case 2:
            if (CustomShadeDates[actualIndex].GroupShaded)
                lv_obj_clear_flag(guider_ui.screen_58_btn_8_label, LV_OBJ_FLAG_HIDDEN);
            else
                lv_obj_add_flag(guider_ui.screen_58_btn_8_label, LV_OBJ_FLAG_HIDDEN);
            break;
        case 3:
            if (CustomShadeDates[actualIndex].GroupShaded)
                lv_obj_clear_flag(guider_ui.screen_58_btn_9_label, LV_OBJ_FLAG_HIDDEN);
            else
                lv_obj_add_flag(guider_ui.screen_58_btn_9_label, LV_OBJ_FLAG_HIDDEN);
            break;
        case 4:
            if (CustomShadeDates[actualIndex].GroupShaded)
                lv_obj_clear_flag(guider_ui.screen_58_btn_10_label, LV_OBJ_FLAG_HIDDEN);
            else
                lv_obj_add_flag(guider_ui.screen_58_btn_10_label, LV_OBJ_FLAG_HIDDEN);
            break;
        }
    }

    // // 群组保存通道数与索引
    // CustomGroupDates[GroupPreIndex].ContainShades = 0x0000;
    // for (uint8_t i = 0; i < ARRAY_SIZE; i++)
    // {
    //     if (CustomShadeDates[i].GroupShaded)
    //         CustomGroupDates[GroupPreIndex].ContainShades |= (1 << i);
    // }
    // char binary_str[25]; // 足够容纳16位二进制数（16位+3个空格+结束符）
    // print_binary_16bit(CustomGroupDates[GroupPreIndex].ContainShades, binary_str);
    // printf_DMA("ContainShades Binary: %s\r\n", binary_str);
}
