#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "Customdate.h"
extern lv_indev_t *indev_keypad;
extern lv_ui guider_ui; // 声明 界面对象
extern uint8_t g_CacheCmd, current_index;
extern current_screen_t current_screen, Mem_Screen;
static uint8_t GroupMenuCnt = 0;
static uint8_t GroupMenuStart = 0;
uint8_t GroupPreIndex = 0;
void Screen_57_Movie(unsigned int Keyid)
{

    // 收集所有可见的group项索引
    uint8_t visibleGroups[ARRAY_SIZE] = {0};
    uint8_t visibleCount = 0;

    for (uint8_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (CustomGroupDates[i].Show)
        {
            visibleGroups[visibleCount] = i;
            visibleCount++;
        }
    }

    uint8_t totalItems = visibleCount + 1; // 加上"Add Group"项

    if (Keyid == KEYFORWARD_TRG) // 向上滚动
    {
        if (GroupMenuCnt == 0)
        {
            // 当前在第一项，跳转到最后一项
            GroupMenuCnt = totalItems - 1;
            // 设置显示区域为最后5项的起始位置
            GroupMenuStart = (totalItems >= 5) ? (totalItems - 5) : 0;
        }
        else
        {
            GroupMenuCnt--;

            // 当选中项在显示区域的前两个时滚动显示
            if (GroupMenuCnt < GroupMenuStart)
            {
                GroupMenuStart = GroupMenuCnt; // 直接设置起始位置为当前选中项
            }
        }
    }
    else if (Keyid == KEYBEHIND_TRG) // 向下滚动
    {
        if (GroupMenuCnt == totalItems - 1)
        {
            // 当前在最后一项，跳转到第一项
            GroupMenuCnt = 0;
            GroupMenuStart = 0;
        }
        else
        {
            GroupMenuCnt++;

            // 当选中项在显示区域的最后两个时滚动显示
            if (GroupMenuCnt >= GroupMenuStart + 5)
            {
                GroupMenuStart = GroupMenuCnt - 4; // 直接调整起始位置
            }
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_57_label_2, LV_EVENT_CLICKED, NULL); // 加载界面Screen_1
        current_screen = SCREEN_1;
        Screen1date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {

        if (GroupMenuCnt == 0)
        {
            lv_event_send(guider_ui.screen_57_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_9
            current_screen = SCREEN_9;
            Screen9date_preloading();
            Mem_Screen = SCREEN_57; // 屏幕记忆点，便于跳转
        }
        else if (GroupMenuCnt == 1) // ALLSHADES群组，不允许修改
        {
            return;
        }
        else
        {
            // 打开对应group的设置界面
            uint8_t actualIndex = visibleGroups[GroupMenuCnt - 1]; // 获取实际的group索引
            current_index = actualIndex;                           // 群控获取索引，发信号
            // 这里可以保存当前选中的group索引，供后续界面使用
            // g_SelectedGroupIndex = actualIndex;
            GroupPreIndex = actualIndex;
            lv_event_send(guider_ui.screen_57_btn_2, LV_EVENT_SHORT_CLICKED, NULL);
            current_screen = SCREEN_59;
            Screen59date_preloading();
        }
        return;
    }
    Screen57date_preloading();
}

void Screen57date_preloading(void)
{

    // 收集所有可见的group项索引
    uint8_t visibleGroups[ARRAY_SIZE] = {0};
    uint8_t visibleCount = 0;

    for (uint8_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (CustomGroupDates[i].Show)
        {
            visibleGroups[visibleCount] = i;
            visibleCount++;
        }
    }

    uint8_t totalItems = visibleCount + 1; // 加上"Add Group"项

    // 确保选中项在有效范围内
    if (GroupMenuCnt >= totalItems)
    {
        GroupMenuCnt = totalItems - 1;
    }

    // 更新滚动条位置
    if (totalItems > 1)
    {
        lv_obj_set_size(guider_ui.screen_57_line_2, 4, GroupMenuCnt * (210 / (totalItems - 1)));
    }
    else
    {
        lv_obj_set_size(guider_ui.screen_57_line_2, 4, 0);
    }
    lv_obj_set_style_text_color(guider_ui.screen_57_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_57_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_57_btn_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_57_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_57_btn_3, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_57_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_57_btn_4, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_57_btn_4, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_57_btn_5, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_57_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 根据总可见项数量显示/隐藏按钮
    if (totalItems <= 1)
    {
        lv_obj_clear_flag(guider_ui.screen_57_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_57_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_57_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_57_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_57_btn_5, LV_OBJ_FLAG_HIDDEN);
    }
    else if (totalItems == 2)
    {
        lv_obj_clear_flag(guider_ui.screen_57_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_57_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_57_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_57_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_57_btn_5, LV_OBJ_FLAG_HIDDEN);
    }
    else if (totalItems == 3)
    {
        lv_obj_clear_flag(guider_ui.screen_57_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_57_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_57_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_57_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_57_btn_5, LV_OBJ_FLAG_HIDDEN);
    }
    else if (totalItems == 4)
    {
        lv_obj_clear_flag(guider_ui.screen_57_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_57_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_57_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_57_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_57_btn_5, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_clear_flag(guider_ui.screen_57_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_57_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_57_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_57_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_57_btn_5, LV_OBJ_FLAG_HIDDEN);
    }

    // 设置5个按钮的文本（从GroupMenuStart开始）
    for (int i = 0; i < 5; i++)
    {
        uint8_t displayIndex = GroupMenuStart + i;

        // 确保索引在有效范围内
        if (displayIndex >= totalItems)
        {
            // 超出范围，清空按钮文本并隐藏按钮
            switch (i)
            {
            case 0:
                lv_label_set_text(guider_ui.screen_57_btn_1_label, "");
                lv_obj_add_flag(guider_ui.screen_57_btn_1, LV_OBJ_FLAG_HIDDEN);
                break;
            case 1:
                lv_label_set_text(guider_ui.screen_57_btn_2_label, "");
                lv_obj_add_flag(guider_ui.screen_57_btn_2, LV_OBJ_FLAG_HIDDEN);
                break;
            case 2:
                lv_label_set_text(guider_ui.screen_57_btn_3_label, "");
                lv_obj_add_flag(guider_ui.screen_57_btn_3, LV_OBJ_FLAG_HIDDEN);
                break;
            case 3:
                lv_label_set_text(guider_ui.screen_57_btn_4_label, "");
                lv_obj_add_flag(guider_ui.screen_57_btn_4, LV_OBJ_FLAG_HIDDEN);
                break;
            case 4:
                lv_label_set_text(guider_ui.screen_57_btn_5_label, "");
                lv_obj_add_flag(guider_ui.screen_57_btn_5, LV_OBJ_FLAG_HIDDEN);
                break;
            }
            continue;
        }

        char groupName[STRING_SIZE] = {0};

        if (displayIndex == 0)
        {
            // 第一项是 "Add Group"
            strcpy(groupName, "Add Group");
        }
        else
        {
            // 其他项是从CustomGroupDates获取的数据
            uint8_t actualIndex = visibleGroups[displayIndex - 1]; // 获取实际的group索引
            strncpy(groupName, (char *)CustomGroupDates[actualIndex].DataArray, STRING_SIZE);
            groupName[STRING_SIZE - 1] = '\0'; // 确保字符串终止
        }

        // 设置按钮文本
        switch (i)
        {
        case 0:
            lv_label_set_text(guider_ui.screen_57_btn_1_label, groupName);
            break;
        case 1:
            lv_label_set_text(guider_ui.screen_57_btn_2_label, groupName);
            break;
        case 2:
            lv_label_set_text(guider_ui.screen_57_btn_3_label, groupName);
            break;
        case 3:
            lv_label_set_text(guider_ui.screen_57_btn_4_label, groupName);
            break;
        case 4:
            lv_label_set_text(guider_ui.screen_57_btn_5_label, groupName);
            break;
        }
    }

    // 高亮当前选中的按钮
    uint8_t selectedPos = GroupMenuCnt - GroupMenuStart;
    if (selectedPos < 5)
    {
        switch (selectedPos)
        {
        case 0:
            lv_event_send(guider_ui.screen_57_btn_1, LV_EVENT_CLICKED, NULL);
            break;
        case 1:
            lv_event_send(guider_ui.screen_57_btn_2, LV_EVENT_CLICKED, NULL);
            break;
        case 2:
            lv_event_send(guider_ui.screen_57_btn_3, LV_EVENT_CLICKED, NULL);
            break;
        case 3:
            lv_event_send(guider_ui.screen_57_btn_4, LV_EVENT_CLICKED, NULL);
            break;
        case 4:
            lv_event_send(guider_ui.screen_57_btn_5, LV_EVENT_CLICKED, NULL);
            break;
        }
    }
}
