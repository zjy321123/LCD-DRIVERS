#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Customdate.h"
extern current_screen_t current_screen, Pre_screen, Mem_Screen;
static uint8_t Index_Scr59 = 0;
extern uint16_t Autotick;
extern uint8_t GroupPreIndex;
extern uint8_t current_mode;
void Screen_59_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG)
    {
        if (Index_Scr59 < 4)
            Index_Scr59++;
    }
    else if (Keyid == KEYFORWARD_TRG)
    {
        if (Index_Scr59 > 0)
            Index_Scr59--;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_59_label_2, LV_EVENT_CLICKED, NULL); // 加载界面screen_57
        current_screen = SCREEN_57;
        Screen57date_preloading();
        SaveDataToFlash();
        return;
    }
    else if (Keyid == KEYLEFT_TRG || Keyid == KEYRIGHT_TRG)
    {
        if (Index_Scr59 == 1)
        {
            CustomGroupDates[GroupPreIndex].ThemeShow = !CustomGroupDates[GroupPreIndex].ThemeShow;
        }
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Index_Scr59 == 0)
        {
            lv_event_send(guider_ui.screen_59_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_9
            current_screen = SCREEN_9;
            Mem_Screen = SCREEN_59; // 屏幕记忆点，便于跳转
            Screen9date_preloading();
            return;
        }
        else if (Index_Scr59 == 2)
        {
            lv_event_send(guider_ui.screen_59_btn_6, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_2
            current_screen = SCREEN_20;
            Screen20date_preloading();
            current_mode = 1;       // 编辑时使能群组标志位
            Mem_Screen = SCREEN_59; // 屏幕记忆点，便于跳转
            return;
        }
        else if (Index_Scr59 == 3)
        {
            lv_event_send(guider_ui.screen_59_btn_3, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_58
            current_screen = SCREEN_58;
            Screen58date_preloading();
            Mem_Screen = SCREEN_59; // 屏幕记忆点，便于跳转

            return;
        }
        else if (Index_Scr59 == 4)
        {
            lv_event_send(guider_ui.screen_59_btn_4, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_60
            current_screen = SCREEN_60;
            Screen60date_preloading();
            return;
        }
    }
    Screen59date_preloading();
}

void Screen59date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_59_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_59_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_59_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_59_btn_4, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_59_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_59_btn_6, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Index_Scr59 == 0)
        lv_event_send(guider_ui.screen_59_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr59 == 1)
    {
        lv_event_send(guider_ui.screen_59_btn_2, LV_EVENT_CLICKED, NULL);
        lv_event_send(guider_ui.screen_59_btn_5, LV_EVENT_CLICKED, NULL);
    }
    else if (Index_Scr59 == 2)
        lv_event_send(guider_ui.screen_59_btn_6, LV_EVENT_CLICKED, NULL);

    else if (Index_Scr59 == 3)
        lv_event_send(guider_ui.screen_59_btn_3, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr59 == 4)
        lv_event_send(guider_ui.screen_59_btn_4, LV_EVENT_CLICKED, NULL);

    if (CustomGroupDates[GroupPreIndex].ThemeShow)
        lv_label_set_text(guider_ui.screen_59_btn_5_label, "" LV_SYMBOL_LEFT "  ON  " LV_SYMBOL_RIGHT " ");
    else
        lv_label_set_text(guider_ui.screen_59_btn_5_label, "" LV_SYMBOL_LEFT "  OFF  " LV_SYMBOL_RIGHT " ");

    char display_text[21]; // 19字节最大需求 + 1
    char *current_data = CustomGroupDates[GroupPreIndex].DataArray;

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
    lv_label_set_text(guider_ui.screen_59_label_2, display_text);
}
