#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "Customdate.h"

extern lv_indev_t *indev_keypad;
extern lv_ui guider_ui;
extern uint8_t g_CacheCmd;
extern current_screen_t current_screen, Pre_screen;
extern uint8_t current_index, current_mode;
// 当前选中的项索引和显示的第一项索引
static uint8_t ShadeMenuCnt = 0;
static uint8_t ShadeMenuStart = 0;
uint8_t ShadePreIndex = 0;
void Screen_2_Movie(unsigned int Keyid)
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

    uint8_t totalItems = visibleCount + 1; // 加上"Pair New Shade"项

    if (Keyid == KEYFORWARD_TRG) // 向上滚动
    {
        if (ShadeMenuCnt == 0)
        {
            // 当前在第一项，跳转到最后一项
            ShadeMenuCnt = totalItems - 1;
            // 设置显示区域为最后5项的起始位置
            ShadeMenuStart = (totalItems >= 5) ? (totalItems - 5) : 0;
        }
        else
        {
            ShadeMenuCnt--;

            // 当选中项在显示区域的前两个时滚动显示
            if (ShadeMenuCnt < ShadeMenuStart)
            {
                ShadeMenuStart = ShadeMenuCnt; // 直接设置起始位置为当前选中项
            }
        }
    }
    else if (Keyid == KEYBEHIND_TRG) // 向下滚动
    {
        if (ShadeMenuCnt == totalItems - 1)
        {
            // 当前在最后一项，跳转到第一项
            ShadeMenuCnt = 0;
            ShadeMenuStart = 0;
        }
        else
        {
            ShadeMenuCnt++;

            // 当选中项在显示区域的最后两个时滚动显示
            if (ShadeMenuCnt >= ShadeMenuStart + 5)
            {
                ShadeMenuStart = ShadeMenuCnt - 4; // 直接调整起始位置
            }
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_2_label_4, LV_EVENT_CLICKED, NULL); // 加载界面Screen_1
        current_screen = SCREEN_1;
        Screen1date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (ShadeMenuCnt == 0)
        {
            // "Pair New Shade" 的特殊处理

            lv_event_send(guider_ui.screen_2_btn_1, LV_EVENT_SHORT_CLICKED, NULL);
            current_screen = SCREEN_3;
        }
        else
        {
            // 打开对应shade的设置界面
            uint8_t actualIndex = visibleShades[ShadeMenuCnt - 1]; // 获取实际的shade索引
            current_index = actualIndex;                       // 确保发射信号对应索引通道
            current_mode = 0;                                      //防止之前群组编辑干扰
            // 这里可以保存当前选中的shade索引，供后续界面使用
            // g_SelectedShadeIndex = actualIndex;

            lv_event_send(guider_ui.screen_2_btn_2, LV_EVENT_SHORT_CLICKED, NULL);
            current_screen = SCREEN_20;
            ShadePreIndex = actualIndex;
            Screen20date_preloading();
        }
        return;
    }
    Screen2date_preloading();
}

void Screen2date_preloading(void)
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

    uint8_t totalItems = visibleCount + 1; // 加上"Pair New Shade"项

    // 确保选中项在有效范围内
    if (ShadeMenuCnt >= totalItems)
    {
        ShadeMenuCnt = totalItems - 1;
    }

    // 更新滚动条位置
    if (totalItems > 1)
    {
        lv_obj_set_size(guider_ui.screen_2_line_2, 4, ShadeMenuCnt * (210 / (totalItems - 1)));
    }
    else
    {
        lv_obj_set_size(guider_ui.screen_2_line_2, 4, 0);
    }

    // 重置所有按钮样式
    lv_obj_set_style_text_color(guider_ui.screen_2_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_2_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_2_btn_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_2_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_2_btn_3, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_2_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_2_btn_4, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_2_btn_4, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(guider_ui.screen_2_btn_5, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_2_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 根据总可见项数量显示/隐藏按钮
    if (totalItems <= 1)
    {
        lv_obj_clear_flag(guider_ui.screen_2_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_2_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_2_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_2_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_2_btn_5, LV_OBJ_FLAG_HIDDEN);
    }
    else if (totalItems == 2)
    {
        lv_obj_clear_flag(guider_ui.screen_2_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_2_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_2_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_2_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_2_btn_5, LV_OBJ_FLAG_HIDDEN);
    }
    else if (totalItems == 3)
    {
        lv_obj_clear_flag(guider_ui.screen_2_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_2_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_2_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_2_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_2_btn_5, LV_OBJ_FLAG_HIDDEN);
    }
    else if (totalItems == 4)
    {
        lv_obj_clear_flag(guider_ui.screen_2_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_2_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_2_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_2_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.screen_2_btn_5, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_clear_flag(guider_ui.screen_2_btn_1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_2_btn_2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_2_btn_3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_2_btn_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_2_btn_5, LV_OBJ_FLAG_HIDDEN);
    }

    // 设置5个按钮的文本（从ShadeMenuStart开始）
    for (int i = 0; i < 5; i++)
    {
        uint8_t displayIndex = ShadeMenuStart + i;

        // 确保索引在有效范围内
        if (displayIndex >= totalItems)
        {
            // 超出范围，清空按钮文本
            switch (i)
            {
            case 0:
                lv_label_set_text(guider_ui.screen_2_btn_1_label, "");
                lv_obj_add_flag(guider_ui.screen_2_btn_1, LV_OBJ_FLAG_HIDDEN);
                break;
            case 1:
                lv_label_set_text(guider_ui.screen_2_btn_2_label, "");
                lv_obj_add_flag(guider_ui.screen_2_btn_2, LV_OBJ_FLAG_HIDDEN);
                break;
            case 2:
                lv_label_set_text(guider_ui.screen_2_btn_3_label, "");
                lv_obj_add_flag(guider_ui.screen_2_btn_3, LV_OBJ_FLAG_HIDDEN);
                break;
            case 3:
                lv_label_set_text(guider_ui.screen_2_btn_4_label, "");
                lv_obj_add_flag(guider_ui.screen_2_btn_4, LV_OBJ_FLAG_HIDDEN);
                break;
            case 4:
                lv_label_set_text(guider_ui.screen_2_btn_5_label, "");
                lv_obj_add_flag(guider_ui.screen_2_btn_5, LV_OBJ_FLAG_HIDDEN);
                break;
            }
            continue;
        }

        char shadeName[STRING_SIZE] = {0};

        if (displayIndex == 0)
        {
            // 第一项是 "Pair New Shade"
            strcpy(shadeName, "Pair New Shade");
        }
        else
        {
            // 其他项是从CustomShadeDates获取的数据
            uint8_t actualIndex = visibleShades[displayIndex - 1]; // 获取实际的shade索引
            strncpy(shadeName, (char *)CustomShadeDates[actualIndex].DataArray, STRING_SIZE);
            shadeName[STRING_SIZE - 1] = '\0'; // 确保字符串终止
        }

        // 设置按钮文本
        switch (i)
        {
        case 0:
            lv_label_set_text(guider_ui.screen_2_btn_1_label, shadeName);
            break;
        case 1:
            lv_label_set_text(guider_ui.screen_2_btn_2_label, shadeName);
            break;
        case 2:
            lv_label_set_text(guider_ui.screen_2_btn_3_label, shadeName);
            break;
        case 3:
            lv_label_set_text(guider_ui.screen_2_btn_4_label, shadeName);
            break;
        case 4:
            lv_label_set_text(guider_ui.screen_2_btn_5_label, shadeName);
            break;
        }
    }

    // 高亮当前选中的按钮
    uint8_t selectedPos = ShadeMenuCnt - ShadeMenuStart;
    if (selectedPos < 5)
    {
        switch (selectedPos)
        {
        case 0:
            lv_event_send(guider_ui.screen_2_btn_1, LV_EVENT_CLICKED, NULL);
            break;
        case 1:
            lv_event_send(guider_ui.screen_2_btn_2, LV_EVENT_CLICKED, NULL);
            break;
        case 2:
            lv_event_send(guider_ui.screen_2_btn_3, LV_EVENT_CLICKED, NULL);
            break;
        case 3:
            lv_event_send(guider_ui.screen_2_btn_4, LV_EVENT_CLICKED, NULL);
            break;
        case 4:
            lv_event_send(guider_ui.screen_2_btn_5, LV_EVENT_CLICKED, NULL);
            break;
        }
    }
}
