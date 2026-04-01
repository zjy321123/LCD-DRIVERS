#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Customdate.h"
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
static uint8_t Index_Scr60 = 0;
extern uint8_t GroupPreIndex;
void Screen_60_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Index_Scr60 == 0)
            Index_Scr60 = 1;
        else if (Index_Scr60 == 1)
            Index_Scr60 = 0;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Index_Scr60 == 0)
            Index_Scr60 = 1;
        else if (Index_Scr60 == 1)
            Index_Scr60 = 0;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Index_Scr60 == 0)
        {
            // CustomGroupDates[GroupPreIndex].Show = 0;
            // CustomGroupDates[GroupPreIndex].Show = 0;
            // CustomGroupDates[GroupPreIndex].Used = 0;
            // CustomGroupDates[GroupPreIndex].ContainShades = 0X0000;
            // memset(CustomGroupDates[GroupPreIndex].DataArray, 0, STRING_SIZE);
            SetCustomGroupDataWithNumber(GroupPreIndex, 0, 0, 0, 0, 0, 0, NULL);
            lv_event_send(guider_ui.screen_60_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_57
            current_screen = SCREEN_57;
            Screen57date_preloading();
            return;
        }
        else if (Index_Scr60 == 1)
        {
            lv_event_send(guider_ui.screen_60_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_59
            current_screen = SCREEN_59;
            Screen59date_preloading();
            return;
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_60_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_59
        current_screen = SCREEN_59;
        Screen59date_preloading();
        return;
    }
    Screen60date_preloading();
}

void Screen60date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_60_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_60_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Index_Scr60 == 0)
        lv_event_send(guider_ui.screen_60_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr60 == 1)
        lv_event_send(guider_ui.screen_60_btn_2, LV_EVENT_CLICKED, NULL);

    char display_text[21]; // 19字节最大需求 + 1
    char *current_data = (char *)CustomShadeDates[GroupPreIndex].DataArray;

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
    lv_label_set_text(guider_ui.screen_60_label_3, display_text);
}
