#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "tim.h"
#include "Syscfg.h"
#include "Customdate.h"
#include "Lms6ds3tr.h"
extern current_screen_t current_screen, Pre_screen, Mem_Screen;
static uint8_t Index_Scr68 = 0;
uint8_t BacklightLevel = 0;
extern uint8_t SysWakeUpTick;
extern uint8_t S_Waketime;
void Screen_68_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG)
    {
        if (Index_Scr68 < 5)
            Index_Scr68++;
    }
    else if (Keyid == KEYFORWARD_TRG)
    {
        if (Index_Scr68 > 0)
            Index_Scr68--;
    }
    else if (Keyid == KEYLEFT_TRG)
    {
        if (Index_Scr68 == 0)
        {
            if (BacklightLevel > 0)
            {
                BacklightLevel--;
                SaveDataToFlash();
                __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, BACKLIGHT_LOWVALUE + ((1000 - BACKLIGHT_LOWVALUE) / 5) * BacklightLevel);
            }
        }
        else if (Index_Scr68 == 1)
        {
            if (SysWakeUpTick > 5)
            {
                SysWakeUpTick -= 5;
                S_Waketime = SysWakeUpTick;
                SaveDataToFlash();
            }
        }
        else if (Index_Scr68 == 2)
        {
            if (Lsm6d3trsystem.status)
                Lsm6d3trsystem.status = 0;
            else
                Lsm6d3trsystem.status = 1;
            SaveDataToFlash();
        }
    }
    else if (Keyid == KEYRIGHT_TRG)
    {
        if (Index_Scr68 == 0)
        {
            if (BacklightLevel < 4)
            {
                BacklightLevel++;
                SaveDataToFlash();
                __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, BACKLIGHT_LOWVALUE + ((1000 - BACKLIGHT_LOWVALUE) / 5) * BacklightLevel);
            }
        }
        else if (Index_Scr68 == 1)
        {
            if (SysWakeUpTick < 61)
            {
                SysWakeUpTick += 5;
                S_Waketime = SysWakeUpTick;
                SaveDataToFlash();
            }
        }
        else if (Index_Scr68 == 2)
        {
            if (Lsm6d3trsystem.status)
                Lsm6d3trsystem.status = 0;
            else
                Lsm6d3trsystem.status = 1;
            SaveDataToFlash();
        }
    }
    if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_68_label_3, LV_EVENT_CLICKED, NULL); // 加载界面screen_57
        current_screen = SCREEN_1;
        Screen1date_preloading();
        return;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        if (Index_Scr68 == 0)
        {
        }
        else if (Index_Scr68 == 1)
        {
        }
        else if (Index_Scr68 == 2)
        {
        }
        else if (Index_Scr68 == 3)
        {
            lv_event_send(guider_ui.screen_68_btn_4, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_69
            current_screen = SCREEN_69;
            Screen69date_preloading();
            return;
        }
        else if (Index_Scr68 == 4)
        {
            lv_event_send(guider_ui.screen_68_btn_5, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_70
            current_screen = SCREEN_70;
            Screen70date_preloading();
            return;
        }
        else if (Index_Scr68 == 5)
        {
            lv_event_send(guider_ui.screen_68_btn_6, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_72
            current_screen = SCREEN_72;
            Screen72date_preloading();
            return;
        }
    }
    Screen68date_preloading();
}

void Screen68date_preloading(void)
{
    lv_obj_set_style_bg_color(guider_ui.screen_68_btn_1, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_68_btn_2, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_68_btn_3, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_68_btn_4, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_68_btn_5, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_68_btn_6, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_68_btn_8, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_68_btn_9, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_68_btn_10, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_68_btn_11, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(guider_ui.screen_68_btn_12, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(guider_ui.screen_68_label_4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_68_label_5, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_68_label_6, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_68_label_7, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_68_label_8, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_68_label_9, LV_OBJ_FLAG_HIDDEN);
    if (Index_Scr68 == 0)
    {
        lv_obj_clear_flag(guider_ui.screen_68_label_4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_68_label_5, LV_OBJ_FLAG_HIDDEN);
    }
    else if (Index_Scr68 == 1)
    {
        lv_obj_clear_flag(guider_ui.screen_68_label_6, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_68_label_7, LV_OBJ_FLAG_HIDDEN);
    }
    else if (Index_Scr68 == 2)
    {
        lv_obj_clear_flag(guider_ui.screen_68_label_8, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_68_label_9, LV_OBJ_FLAG_HIDDEN);
    }

    if (BacklightLevel == 0)
        lv_event_send(guider_ui.screen_68_btn_8, LV_EVENT_CLICKED, NULL);
    else if (BacklightLevel == 1)
    {
        lv_event_send(guider_ui.screen_68_btn_8, LV_EVENT_CLICKED, NULL);
        lv_event_send(guider_ui.screen_68_btn_9, LV_EVENT_CLICKED, NULL);
    }
    else if (BacklightLevel == 2)
    {
        lv_event_send(guider_ui.screen_68_btn_8, LV_EVENT_CLICKED, NULL);
        lv_event_send(guider_ui.screen_68_btn_9, LV_EVENT_CLICKED, NULL);
        lv_event_send(guider_ui.screen_68_btn_10, LV_EVENT_CLICKED, NULL);
    }
    else if (BacklightLevel == 3)
    {
        lv_event_send(guider_ui.screen_68_btn_8, LV_EVENT_CLICKED, NULL);
        lv_event_send(guider_ui.screen_68_btn_9, LV_EVENT_CLICKED, NULL);
        lv_event_send(guider_ui.screen_68_btn_10, LV_EVENT_CLICKED, NULL);
        lv_event_send(guider_ui.screen_68_btn_11, LV_EVENT_CLICKED, NULL);
    }
    else if (BacklightLevel == 4)
    {
        lv_event_send(guider_ui.screen_68_btn_8, LV_EVENT_CLICKED, NULL);
        lv_event_send(guider_ui.screen_68_btn_9, LV_EVENT_CLICKED, NULL);
        lv_event_send(guider_ui.screen_68_btn_10, LV_EVENT_CLICKED, NULL);
        lv_event_send(guider_ui.screen_68_btn_11, LV_EVENT_CLICKED, NULL);
        lv_event_send(guider_ui.screen_68_btn_12, LV_EVENT_CLICKED, NULL);
    }
    lv_label_set_text_fmt(guider_ui.screen_68_btn_2_label, "  Idle                      %d  sec", SysWakeUpTick);

    if (Lsm6d3trsystem.status)
        lv_label_set_text(guider_ui.screen_68_label_10, "ON");
    else
        lv_label_set_text(guider_ui.screen_68_label_10, "OFF");

    if (Index_Scr68 == 0)
        lv_event_send(guider_ui.screen_68_btn_1, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr68 == 1)
        lv_event_send(guider_ui.screen_68_btn_2, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr68 == 2)
        lv_event_send(guider_ui.screen_68_btn_3, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr68 == 3)
        lv_event_send(guider_ui.screen_68_btn_4, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr68 == 4)
        lv_event_send(guider_ui.screen_68_btn_5, LV_EVENT_CLICKED, NULL);
    else if (Index_Scr68 == 5)
        lv_event_send(guider_ui.screen_68_btn_6, LV_EVENT_CLICKED, NULL);
}
