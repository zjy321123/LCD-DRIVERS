#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Customdate.h"
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
static uint8_t Index_Scr21 = 0;
extern uint8_t ShadePreIndex;
void Screen_21_Movie(unsigned int Keyid)
{
    if (Keyid == KEYFORWARD_TRG)
    {
        if (Index_Scr21 == 0)
            Index_Scr21 = 1;
        else if (Index_Scr21 == 1)
            Index_Scr21 = 0;
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        if (Index_Scr21 == 0)
            Index_Scr21 = 1;
        else if (Index_Scr21 == 1)
            Index_Scr21 = 0;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Index_Scr21 == 0)
        {
            SetCustomShadeDataWithNumber(ShadePreIndex, 0, 0, 0, 0, 0, 0, NULL);
            lv_event_send(guider_ui.screen_21_btn_1, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_2
            current_screen = SCREEN_2;
            Screen2date_preloading();
            return;
        }
        else if (Index_Scr21 == 1)
        {
            lv_event_send(guider_ui.screen_21_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
            current_screen = SCREEN_20;
            Screen20date_preloading();
            return;
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_21_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
        current_screen = SCREEN_20;
        Screen20date_preloading();
        return;
    }
    Screen21date_preloading();
}

void Screen21date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_21_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_21_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Index_Scr21 == 0)
        lv_event_send(guider_ui.screen_21_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr21 == 1)
        lv_event_send(guider_ui.screen_21_btn_2, LV_EVENT_CLICKED, NULL);

    char display_text[21]; // 19字节最大需求 + 1
    char *current_data = (char *)CustomShadeDates[ShadePreIndex].DataArray;

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
    lv_label_set_text(guider_ui.screen_21_label_3, display_text);
}
