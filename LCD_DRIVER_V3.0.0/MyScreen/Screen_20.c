#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "system.h"
#include "keyboard.h"
#include "Customdate.h"
extern current_screen_t current_screen, Pre_screen, Mem_Screen, Mem_Screen2;
extern uint8_t ShadePreIndex, current_mode, GroupPreIndex;

#define Idems 20u // 子状态数量
const char *MenuTexts[Idems] =
    {
        "Edit Name",                // 0  编辑名字
        "Show Shade",               // 1  是否显示通道
        "Pair",                     // 2  对码
        "Position Level Check",     // 3  检测电机运行位置
        "Battery Level Check",      // 4  检测电电池电量
        "Set Fav Position",         // 5  设置最佳位置
        "Change direction",         // 6  换向
        "Pct Operation",            // 7  百分比控制
        "Set Upper limit",          // 8  设置上行程
        "Set Lower limit",          // 9  设置下行程
        "Adjust Speed",             // 10  调节速度
        "Toggle Tilt/Roller Mode",  // 11  点动、连动切换
        "Adjust Soft Stop",         // 12  缓起缓停
        "Set LED",                  // 13  LED设置
        "Obstacle Switch",          // 14 打开关闭遇阻模式
        "Set Obstacle Mode ",       // 15 调节遇阻模式
        "Set Obstacle Sensitivity", // 16 调节遇阻灵敏度
        "Special function",         // 17 特殊功能
        "Factory Motor",            // 18 出厂化电机
        "Delete Shade"              // 19 删除通道 （放最后）
};

static uint8_t Scre20Cnt, Menu20Cnt = 0;
void Screen_20_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG) // 向下滚动
    {
        if (Scre20Cnt == Idems - 1)
        {
            // 当前在最后一项，跳转到第一项
            Scre20Cnt = 0;
            Menu20Cnt = 0;
        }
        else
        {
            Scre20Cnt++;

            // 当选中项在显示区域的最后两个时滚动显示
            if (Scre20Cnt >= Menu20Cnt + 5)
            {
                Menu20Cnt = (Menu20Cnt + 1) % Idems;
            }
        }
    }
    else if (Keyid == KEYFORWARD_TRG) // 向上滚动
    {
        if (Scre20Cnt == 0)
        {
            // 当前在第一项，跳转到最后一项
            Scre20Cnt = Idems - 1;
            // 设置显示区域为最后5项的起始位置
            Menu20Cnt = (Idems >= 5) ? (Idems - 5) : 0;
        }
        else
        {
            Scre20Cnt--;

            // 当选中项在显示区域的前两个时滚动显示
            if (Scre20Cnt < Menu20Cnt)
            {
                Menu20Cnt = (Menu20Cnt + Idems - 1) % Idems;
            }
        }
    }
    else if (Keyid == KEYLEFT_TRG || Keyid == KEYRIGHT_TRG)
    {
        if (Scre20Cnt == 1)
        {
            if (current_mode == 0) // 群组控制时不允行编辑
                CustomShadeDates[ShadePreIndex].ThemeShow = !CustomShadeDates[ShadePreIndex].ThemeShow;
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        if (Mem_Screen == SCREEN_59)
        {
            Mem_Screen = SCREEN_NUll;
            lv_event_send(guider_ui.screen_20_label_4, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_59
            current_screen = SCREEN_59;
            Screen59date_preloading();
            return;
        }
        else
        {
            lv_event_send(guider_ui.screen_20_label_4, LV_EVENT_CLICKED, NULL); // 加载界面screen_2
            current_screen = SCREEN_2;
            SaveDataToFlash();
            Screen2date_preloading();
            return;
        }
    }
    else if (Keyid == KEYENTER_TRG)
    {
        extern U_SAVEMSG g_MsgSave;
        switch (Scre20Cnt)
        {
        case 0:
            if (current_mode) // 群组控制时不允行编辑
                return;
            //"Edit Name"
            lv_event_send(guider_ui.screen_20_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen9
            current_screen = SCREEN_9;
            Mem_Screen = SCREEN_20; // 屏幕记忆点，便于跳转
            Screen9date_preloading();
            return;
        case 1: //"Show shade"

            break;
        case 2:
            if (current_mode) // 群组控制时不允行编辑
                return;
            //"Pair"
            lv_event_send(guider_ui.screen_20_btn_2, LV_EVENT_PRESSING, NULL); // 加载界面screen3
            Mem_Screen = SCREEN_20;
            current_screen = SCREEN_3;
            return;
        case 3:
            if ((current_mode) || (g_MsgSave.RadioMode == RM_OOK)) // 群组控制时不允行编辑.OOK时不允许编辑
                return;
            //"Position Level Check"
            lv_event_send(guider_ui.screen_20_btn_1, LV_EVENT_PRESSING, NULL); // 加载界面screen22
            current_screen = SCREEN_22;
            return;
        case 4:                                                    //"Battery Level Check"
            if ((current_mode) || (g_MsgSave.RadioMode == RM_OOK)) // 群组控制时不允行编辑.OOK时不允许编辑
                return;
            lv_event_send(guider_ui.screen_20_btn_1, LV_EVENT_PRESS_LOST, NULL); // 加载界面screen24
            current_screen = SCREEN_24;
            return;
        case 5:                                                                     //"Set Fav.Position""
            lv_event_send(guider_ui.screen_20_btn_1, LV_EVENT_VALUE_CHANGED, NULL); // 加载界面screen26
            current_screen = SCREEN_26;
            return;
        case 6:                                                                    //"Change direction"换向                                                                //"Pct Operation",
            lv_event_send(guider_ui.screen_20_btn_2, LV_EVENT_LONG_PRESSED, NULL); // 加载界面screen10
            current_screen = SCREEN_10;
            Mem_Screen = SCREEN_20;
            Screen10date_preloading();
            return;
        case 7:                                                                    // "Pct Operation"                                                               //"Pct Operation",
            lv_event_send(guider_ui.screen_20_btn_1, LV_EVENT_LONG_PRESSED, NULL); // 加载界面screen28
            current_screen = SCREEN_28;
            Screen28date_preloading();
            return;
        case 8:                                                                           //"Set Upper limit"
            lv_event_send(guider_ui.screen_20_btn_1, LV_EVENT_LONG_PRESSED_REPEAT, NULL); // 加载界面screen14
            current_screen = SCREEN_14;
            Mem_Screen2 = SCREEN_20;
            return;
        case 9:                                                              //"Set Lowwer limit"
            lv_event_send(guider_ui.screen_20_btn_1, LV_EVENT_SCROLL, NULL); // 加载界面screen32
            current_screen = SCREEN_16;
            Mem_Screen2 = SCREEN_20;
            return;
        case 10:                                                                   // "Adjust Speed"
            lv_event_send(guider_ui.screen_20_btn_1, LV_EVENT_SCROLL_BEGIN, NULL); // 加载界面screen34
            current_screen = SCREEN_34;
            return;
        case 11:                                                                 // "Toggle Tilt/Roller Mode"
            lv_event_send(guider_ui.screen_20_btn_1, LV_EVENT_SCROLL_END, NULL); // 加载界面screen37
            current_screen = SCREEN_37;
            return;
        case 12:                                                              // "Adjust Soft Stop"
            lv_event_send(guider_ui.screen_20_btn_1, LV_EVENT_FOCUSED, NULL); // 加载界面screen39
            current_screen = SCREEN_39;
            return;
        case 13:                                                                // "Set LED"
            lv_event_send(guider_ui.screen_20_btn_1, LV_EVENT_DEFOCUSED, NULL); // 加载界面screen42
            current_screen = SCREEN_42;
            return;
        case 14:                                                            // "Obstacle Switch"
            lv_event_send(guider_ui.screen_20_btn_1, LV_EVENT_LEAVE, NULL); // 加载界面screen45
            current_screen = SCREEN_45;
            return;
        case 15:                                                               // "Set Obstacle Mode"
            lv_event_send(guider_ui.screen_20_btn_1, LV_EVENT_HIT_TEST, NULL); // 加载界面screen48
            current_screen = SCREEN_48;
            return;
        case 16:                                                          // "Set Obstacle Sensitivity"
            lv_event_send(guider_ui.screen_20_btn_1, LV_EVENT_KEY, NULL); // 加载界面screen51
            current_screen = SCREEN_51;
            return;
        case 17:                                                              // "special function"
            lv_event_send(guider_ui.screen_20_btn_2, LV_EVENT_PRESSED, NULL); // 加载界面screen56
            current_screen = SCREEN_56;
            Screen56date_preloading();
            return;
        case 18:                                                                    // 出厂化电机
            lv_event_send(guider_ui.screen_20_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen54
            current_screen = SCREEN_54;
            Screen54date_preloading();
            return;
        case 19:              //"Delete shade"
            if (current_mode) // 群组控制时不允行编辑
                return;
            lv_event_send(guider_ui.screen_20_btn_1, LV_EVENT_PRESSED, NULL); // 加载界面screen21
            Screen21date_preloading();
            current_screen = SCREEN_21;
            return;
        default:
            break;
        }
    }
    Screen20date_preloading();
}
void Screen20date_preloading(void)
{
    lv_obj_set_size(guider_ui.screen_20_line_2, 4, Scre20Cnt * (210 / Idems));
    // 设置5个按钮的文本（循环显示）
    for (int i = 0; i < 5; i++)
    {
        uint8_t itemIdx = (Menu20Cnt + i) % Idems;

        char buttonText[50] = {0};

        // 如果是索引1（Show Shade），根据ThemeShow的值动态设置文本
        if (itemIdx == 1)
        {
            if (CustomShadeDates[ShadePreIndex].ThemeShow == 1)
            {
                strcpy(buttonText, "Show Shade  <  ON >");
            }
            else
            {
                strcpy(buttonText, "Show Shade  < OFF >");
            }
        }
        else
        {
            strcpy(buttonText, MenuTexts[itemIdx]);
        }

        switch (i)
        {
        case 0:
            lv_label_set_text(guider_ui.screen_20_btn_1_label, buttonText);
            break;
        case 1:
            lv_label_set_text(guider_ui.screen_20_btn_2_label, buttonText);
            break;
        case 2:
            lv_label_set_text(guider_ui.screen_20_btn_3_label, buttonText);
            break;
        case 3:
            lv_label_set_text(guider_ui.screen_20_btn_4_label, buttonText);
            break;
        case 4:
            lv_label_set_text(guider_ui.screen_20_btn_5_label, buttonText);
            break;
        }
    }

    lv_obj_set_style_bg_color(guider_ui.screen_20_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_20_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_20_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_20_btn_4, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_20_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // lv_label_set_text(guider_ui.screen_20_label_3, "Edit ");
    char display_text[21]; // 19字节最大需求 + 1
    char *current_data = NULL;
    if (current_mode)
        current_data = (char *)CustomGroupDates[GroupPreIndex].DataArray;
    else
        current_data = (char *)CustomShadeDates[ShadePreIndex].DataArray;

    strcpy(display_text, "Edit ");
    if (strlen(current_data) > 10)
    {
        strncat(display_text, current_data, 10);
        strcat(display_text, "...");
    }
    else
    {
        strcat(display_text, current_data);
    }

    lv_label_set_text(guider_ui.screen_20_label_3, display_text);
    // 高亮当前选中的按钮
    uint8_t selectedPos = (Scre20Cnt - Menu20Cnt + Idems) % Idems;
    if (selectedPos < 5)
    {
        // 计算当前选中按钮对应的实际菜单索引
        uint8_t currentItemIdx = (Menu20Cnt + selectedPos) % Idems;

        switch (selectedPos)
        {
        case 0:
            if ((currentItemIdx == (Idems - 1)) || (currentItemIdx == (Idems - 2))) // Factory Motor Delete Shade显示为红色
                lv_event_send(guider_ui.screen_20_btn_2, LV_EVENT_VALUE_CHANGED, NULL);
            else
                lv_event_send(guider_ui.screen_20_btn_1, LV_EVENT_CLICKED, NULL);
            break;
        case 1:
            if ((currentItemIdx == (Idems - 1)) || (currentItemIdx == (Idems - 2))) // Factory Motor Delete Shade显示为红色
                lv_event_send(guider_ui.screen_20_btn_2, LV_EVENT_PRESSED, NULL);
            else
                lv_event_send(guider_ui.screen_20_btn_2, LV_EVENT_CLICKED, NULL);
            break;
        case 2:
            if ((currentItemIdx == (Idems - 1)) || (currentItemIdx == (Idems - 2))) // Factory Motor Delete Shade显示为红色
                lv_event_send(guider_ui.screen_20_btn_3, LV_EVENT_SHORT_CLICKED, NULL);
            else
                lv_event_send(guider_ui.screen_20_btn_3, LV_EVENT_CLICKED, NULL);
            break;
        case 3:
            if ((currentItemIdx == (Idems - 1)) || (currentItemIdx == (Idems - 2))) // Factory Motor Delete Shade显示为红色
                lv_event_send(guider_ui.screen_20_btn_4, LV_EVENT_SHORT_CLICKED, NULL);
            else
                lv_event_send(guider_ui.screen_20_btn_4, LV_EVENT_CLICKED, NULL);
            break;
        case 4:
            if ((currentItemIdx == (Idems - 1)) || (currentItemIdx == (Idems - 2))) // Factory Motor Delete Shade显示为红色
                lv_event_send(guider_ui.screen_20_btn_5, LV_EVENT_SHORT_CLICKED, NULL);
            else
                lv_event_send(guider_ui.screen_20_btn_5, LV_EVENT_CLICKED, NULL);
            break;
        }
    }
}
