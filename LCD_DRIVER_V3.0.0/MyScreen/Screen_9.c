#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include <stdint.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Customdate.h"
#include "flash.h"
extern current_screen_t current_screen, Mem_Screen;

lv_group_t *Screen9group;
extern uint8_t ShadePreIndex;
extern uint8_t GroupPreIndex;
extern uint8_t g_TimerPreIndex;
static lv_obj_t *keyboard;         // 全局键盘对象声明
static bool is_upper_case = false; // 跟踪当前大小写状态
uint8_t CachesSahde = 0;
// 键盘模式
typedef enum
{
    KB_MODE_ALPHA, // 字母模式
    KB_MODE_NUMBER // 数字模式
} keyboard_mode_t;

static keyboard_mode_t current_kb_mode = KB_MODE_ALPHA; // 当前键盘模式
void Keyboard_SaveDates(void)
{
    const char *text = lv_textarea_get_text(guider_ui.screen_9_ta_1);
    static uint8_t isidemsfull;
    char NewpairIndex = 0;
    // 将跳转与保存数据区分开，方便看
    if (Mem_Screen == SCREEN_8) // 从新增通道编辑页面过来
    {
        NewpairIndex = FindShadeEmptyIndex();
        if (IsShadeNameExist(text)) // 检查是否有重复的定时器名称
        {
            lv_label_set_text(guider_ui.screen_9_label_7, "Duplicate name already exists");
            lv_obj_clear_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            return; // 不保存数据
        }
        else if (text == NULL || strlen(text) == 0) // 修正：判断text是否为空
        {
            lv_label_set_text(guider_ui.screen_9_label_7, "The name cannot be empty");
            lv_obj_clear_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            return; // 不保存数据
        }
        else if (NewpairIndex >= ARRAY_SIZE)
        {
            if (isidemsfull) // 再次按下返回主界面
            {
                lv_event_send(guider_ui.screen_9_label_6, LV_EVENT_LONG_PRESSED, NULL); // 加载界面screen
                current_screen = SCREEN;
                Mem_Screen = SCREEN_NUll;
                isidemsfull = 0;
                Screendate_preloading();
                return; // 不保存数据
            }
            {
                lv_label_set_text(guider_ui.screen_9_label_7, "Exceeds maximum number of shades");
                lv_obj_clear_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
                isidemsfull = 1;
                return; // 不保存数据
            }
        }
        else // 超过最大定时数
        {
            lv_obj_add_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            strncpy((char *)CustomShadeDates[NewpairIndex].DataArray, text, STRING_SIZE); // 确保字符串以\0结尾
            CustomShadeDates[NewpairIndex].DataArray[STRING_SIZE - 1] = '\0';
            CustomShadeDates[NewpairIndex].ThemeShow = CustomShadeDates[NewpairIndex].Show = CustomShadeDates[NewpairIndex].Used = 1; // 标记为默认启用与使用
            CustomShadeDates[NewpairIndex].CurShade = NewpairIndex;
            CachesSahde = NewpairIndex; // 保存对应通道
            SaveDataToFlash();
            lv_event_send(guider_ui.screen_9_label_6, LV_EVENT_CLICKED, NULL); // 加载界面screen_10
            ShadePreIndex = NewpairIndex;
            current_screen = SCREEN_10;
            Mem_Screen = SCREEN_NUll;
            Screen10date_preloading();
            return;
        }
    }
    else if (Mem_Screen == SCREEN_20) // 从通道编辑页面过来
    {
        if (IsShadeNameExist(text)) // 检查是否有重复的通道名称
        {
            lv_label_set_text(guider_ui.screen_9_label_7, "Duplicate name already exists");
            lv_obj_clear_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            return; // 不保存数据
        }
        else if (text == NULL || strlen(text) == 0) // 修正：判断text是否为空
        {
            lv_label_set_text(guider_ui.screen_9_label_7, "The name cannot be empty");
            lv_obj_clear_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            return; // 不保存数据
        }
        else
        {
            lv_obj_add_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            strncpy((char *)CustomShadeDates[ShadePreIndex].DataArray, text, STRING_SIZE); // 确保字符串以\0结尾
            CustomShadeDates[ShadePreIndex].DataArray[STRING_SIZE - 1] = '\0';
            SaveDataToFlash();
            lv_event_send(guider_ui.screen_9_label_6, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
            current_screen = SCREEN_20;
            Mem_Screen = SCREEN_NUll;
            Screen20date_preloading();
            return;
        }
    }
    else if (Mem_Screen == SCREEN_57) // 从新增群组编辑页面过来
    {
        NewpairIndex = FindGroupEmptyIndex();
        if (IsGroupNameExist(text)) // 检查是否有重复的定时器名称
        {
            lv_label_set_text(guider_ui.screen_9_label_7, "Duplicate name already exists");
            lv_obj_clear_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            return; // 不保存数据
        }
        else if (text == NULL || strlen(text) == 0) // 修正：判断text是否为空
        {
            lv_label_set_text(guider_ui.screen_9_label_7, "The name cannot be empty");
            lv_obj_clear_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            return; // 不保存数据
        }
        else if (NewpairIndex >= (ARRAY_SIZE - 1))
        {
            if (isidemsfull) // 再次按下返回主界面
            {
                lv_event_send(guider_ui.screen_9_label_6, LV_EVENT_LONG_PRESSED, NULL); // 加载界面screen
                current_screen = SCREEN;
                Mem_Screen = SCREEN_NUll;
                isidemsfull = 0;
                Screendate_preloading();
                return; // 不保存数据
            }
            {
                lv_label_set_text(guider_ui.screen_9_label_7, "Exceeds maximum number of groups");
                lv_obj_clear_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
                isidemsfull = 1;
                return; // 不保存数据
            }
        }
        else // 超过最大定时数
        {
            lv_obj_add_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            strncpy((char *)CustomGroupDates[NewpairIndex].DataArray, text, STRING_SIZE); // 确保字符串以\0结尾
            CustomGroupDates[NewpairIndex].DataArray[STRING_SIZE - 1] = '\0';
            CustomGroupDates[NewpairIndex].ThemeShow = CustomGroupDates[NewpairIndex].Show = CustomGroupDates[NewpairIndex].Used = 1; // 标记为默认启用与使用
            SaveDataToFlash();
            GroupPreIndex = NewpairIndex;
            lv_event_send(guider_ui.screen_9_label_6, LV_EVENT_PRESSED, NULL); // 加载界面screen_57
            current_screen = SCREEN_57;
            Mem_Screen = SCREEN_NUll;
            Screen57date_preloading();
            return;
        } //
    }
    else if (Mem_Screen == SCREEN_59) // 编辑群组名字
    {
        if (IsGroupNameExist(text)) // 检查是否有重复的定时器名称
        {
            lv_label_set_text(guider_ui.screen_9_label_7, "Duplicate name already exists");
            lv_obj_clear_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            return; // 不保存数据
        }
        else if (text == NULL || strlen(text) == 0) // 修正：判断text是否为空
        {
            lv_label_set_text(guider_ui.screen_9_label_7, "The name cannot be empty");
            lv_obj_clear_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            return; // 不保存数据
        }
        else
        {
            lv_obj_add_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            strncpy((char *)CustomGroupDates[GroupPreIndex].DataArray, text, STRING_SIZE); // 确保字符串以\0结尾
            CustomGroupDates[GroupPreIndex].DataArray[STRING_SIZE - 1] = '\0';
            SaveDataToFlash();
            lv_event_send(guider_ui.screen_9_label_6, LV_EVENT_PRESS_LOST, NULL); // 加载界面screen_59
            current_screen = SCREEN_59;
            Mem_Screen = SCREEN_NUll;
            Screen59date_preloading();
            return;
        }
    }
    else if (Mem_Screen == SCREEN_61) // 新增定时器
    {

        NewpairIndex = FindTimerEmptyIndex();
        if (IsTimerNameExist(text)) // 检查是否有重复的定时器名称
        {
            lv_label_set_text(guider_ui.screen_9_label_7, "Duplicate name already exists");
            lv_obj_clear_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            return; // 不保存数据
        }
        else if (text == NULL || strlen(text) == 0) // 修正：判断text是否为空
        {
            lv_label_set_text(guider_ui.screen_9_label_7, "The name cannot be empty");
            lv_obj_clear_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            return; // 不保存数据
        }
        else if ((NewpairIndex >= TIMERS_SIZE))
        {
            lv_label_set_text(guider_ui.screen_9_label_7, "Exceeds maximum number of groups");
            lv_obj_clear_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            return; // 不保存数据
        }
        else // 超过最大定时数
        {
            lv_obj_add_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            strncpy((char *)CustonmTimers[NewpairIndex].DataArray, text, STRING_SIZE); // 确保字符串以\0结尾
            CustonmTimers[NewpairIndex].DataArray[STRING_SIZE - 1] = '\0';
            CustonmTimers[NewpairIndex].Used = 1; // 标记为默认启用与使用
            SaveDataToFlash();
            lv_event_send(guider_ui.screen_9_label_6, LV_EVENT_VALUE_CHANGED, NULL); // 加载界面screen_61
            current_screen = SCREEN_61;
            Mem_Screen = SCREEN_NUll;
            Screen61date_preloading();
            return;
        }
    }

    else if (Mem_Screen == SCREEN_62) // 有记忆点先跳转记忆点
    {

        if (IsTimerNameExist(text)) // 检查是否有重复的定时器名称
        {
            lv_label_set_text(guider_ui.screen_9_label_7, "Duplicate name already exists");
            lv_obj_clear_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            return; // 不保存数据
        }
        else if (text == NULL || strlen(text) == 0) // 修正：判断text是否为空
        {
            lv_label_set_text(guider_ui.screen_9_label_7, "The name cannot be empty");
            lv_obj_clear_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            return; // 不保存数据
        }
        else
        {
            lv_obj_add_flag(guider_ui.screen_9_label_7, LV_OBJ_FLAG_HIDDEN);
            strncpy((char *)CustonmTimers[g_TimerPreIndex].DataArray, text, STRING_SIZE); // 确保字符串以\0结尾
            CustonmTimers[g_TimerPreIndex].DataArray[STRING_SIZE - 1] = '\0';
            SaveDataToFlash();
            lv_event_send(guider_ui.screen_9_label_6, LV_EVENT_RELEASED, NULL); // 加载界面screen_62
            current_screen = SCREEN_62;
            Screen62date_preloading();
            Mem_Screen = SCREEN_NUll;
            return;
        }
    }
}
// 更新键盘显示模式
void update_keyboard_mode(void)
{
    if (!keyboard)
        return;

    if (current_kb_mode == KB_MODE_NUMBER)
    {
        lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_SPECIAL);
    }
    else
    {
        lv_keyboard_set_mode(keyboard,
                             is_upper_case ? LV_KEYBOARD_MODE_TEXT_UPPER
                                           : LV_KEYBOARD_MODE_TEXT_LOWER);
    }
    lv_obj_invalidate(keyboard);
}

// 新增：切换到字母键盘模式
void switch_to_alpha_mode(void)
{
    if (!keyboard)
        return;
    current_kb_mode = KB_MODE_ALPHA;

    // 重置为小写字母模式
    is_upper_case = false;

    update_keyboard_mode();
    lv_group_send_data(Screen9group, LV_KEY_RIGHT); // 下移一个焦点，矫正焦点
}
// 切换字母大小写模式
void toggle_case_mode(void)
{
    if (!keyboard)
        return;
    is_upper_case = !is_upper_case;
    update_keyboard_mode();
}

// 切换到数字键盘模式
void switch_to_number_mode(void)
{
    if (!keyboard)
        return;

    current_kb_mode = KB_MODE_NUMBER;
    update_keyboard_mode();
}

void Screen_9_Movie(unsigned int Keyid)
{
    lv_obj_t *focused = lv_group_get_focused(Screen9group);
    if (Keyid == KEYFORWARD_TRG)
    {
        lv_group_send_data(Screen9group, LV_KEY_UP);
    }
    else if (Keyid == KEYBEHIND_TRG)
    {
        lv_group_send_data(Screen9group, LV_KEY_DOWN);
    }
    else if (Keyid == KEYLEFT_TRG)
    {
        lv_group_send_data(Screen9group, LV_KEY_LEFT);
    }
    else if (Keyid == KEYRIGHT_TRG)
    {
        lv_group_send_data(Screen9group, LV_KEY_RIGHT);
    }
    else if (Keyid == KEYENTER_TRG)
    {
        // 检查键盘是否已显示
        if (lv_obj_has_flag(keyboard, LV_OBJ_FLAG_HIDDEN))
        {
            lv_keyboard_set_textarea(keyboard, guider_ui.screen_9_ta_1);
            lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);

            // 确保光标可见 - LVGL 8.3 版本
            lv_textarea_set_cursor_pos(guider_ui.screen_9_ta_1, LV_TEXTAREA_CURSOR_LAST);
            lv_obj_add_state(guider_ui.screen_9_ta_1, LV_STATE_FOCUSED);
            lv_obj_add_state(guider_ui.screen_9_ta_1, LV_STATE_EDITED);

            // 强制光标显示
            lv_textarea_set_cursor_click_pos(guider_ui.screen_9_ta_1, true);
            lv_obj_invalidate(guider_ui.screen_9_ta_1);

            lv_group_focus_obj(keyboard);
        }
        else
        {
            // 键盘已显示时，模拟按下当前聚焦的键盘按钮
            if (focused)
            {
                if (lv_obj_has_class(focused, &lv_btnmatrix_class))
                {
                    const char *btn_text = lv_btnmatrix_get_btn_text(focused, lv_btnmatrix_get_selected_btn(focused));

                    if (strcmp(btn_text, "1#") == 0)
                    {
                        switch_to_number_mode();
                        return;
                    }
                    // 处理数字键盘上的"abc"按钮
                    else if (strcmp(btn_text, "abc") == 0 && current_kb_mode == KB_MODE_NUMBER)
                    {
                        switch_to_alpha_mode();
                        return;
                    }
                    // 处理字母键盘上的大小写按钮
                    else if ((strcmp(btn_text, "ABC") == 0 || strcmp(btn_text, "abc") == 0) &&
                             current_kb_mode == KB_MODE_ALPHA)
                    {
                        toggle_case_mode();
                        return;
                    }
                    // 处理对号图标和键盘图标
                    else if (strcmp(btn_text, LV_SYMBOL_OK) == 0 ||    // 对号图标
                             strcmp(btn_text, LV_SYMBOL_CLOSE) == 0 || // 关闭图标
                             strcmp(btn_text, "Done") == 0 ||          // 完成文本
                             strcmp(btn_text, "Close") == 0)
                    { // 关闭文本
                        // 模拟按下键盘的确定按钮
                        lv_event_send(keyboard, LV_EVENT_READY, NULL);
                        return;
                    }
                }
                lv_event_send(focused, LV_EVENT_PRESSED, NULL);
            }
        }
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        if (!lv_obj_has_flag(keyboard, LV_OBJ_FLAG_HIDDEN))
        {
            lv_event_send(keyboard, LV_EVENT_CANCEL, NULL);
        }
        else
        {
            if (Mem_Screen == SCREEN_20) // 有记忆点先跳转记忆点
            {
                Mem_Screen = SCREEN_NUll;
                lv_event_send(guider_ui.screen_9_label_6, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_20
                current_screen = SCREEN_20;
                Screen20date_preloading();
                // 返回就不处理通道名字
                return;
            }
            else if (Mem_Screen == SCREEN_57) // 有记忆点先跳转记忆点
            {
                Mem_Screen = SCREEN_NUll;
                lv_event_send(guider_ui.screen_9_label_6, LV_EVENT_PRESSED, NULL); // 加载界面screen_57
                current_screen = SCREEN_57;
                Screen57date_preloading();
                // 返回就不处理通道名字
                return;
            }
            else if (Mem_Screen == SCREEN_59) // 有记忆点先跳转记忆点
            {
                Mem_Screen = SCREEN_NUll;
                lv_event_send(guider_ui.screen_9_label_6, LV_EVENT_PRESS_LOST, NULL); // 加载界面screen_59
                current_screen = SCREEN_59;
                Screen59date_preloading();
                // 返回就不处理通道名字
                return;
            }
            else if (Mem_Screen == SCREEN_62) // 有记忆点先跳转记忆点
            {
                Mem_Screen = SCREEN_NUll;

                lv_event_send(guider_ui.screen_9_label_6, LV_EVENT_RELEASED, NULL); // 加载界面screen_62
                current_screen = SCREEN_62;
                Screen62date_preloading();
                // 返回就不处理通道名字
                return;
            }
        }
    }
    else if (Keyid == KEYMENU_TRG)
        Keyboard_SaveDates();
}

// 键盘事件回调
// 修改键盘事件回调
static void keyboard_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *kb = lv_event_get_current_target(e);

    if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL)
    {
        // 保存文本框内容到数组
        const char *text = lv_textarea_get_text(guider_ui.screen_9_ta_1);
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_group_focus_obj(guider_ui.screen_9_ta_1);
        Keyboard_SaveDates();
    }
    else if (code == LV_EVENT_VALUE_CHANGED)
    {
        uint16_t btn_id = lv_btnmatrix_get_selected_btn(kb);
        const char *txt = lv_btnmatrix_get_btn_text(kb, btn_id);

        if (txt)
        {
            // 数字切换按钮
            if (strcmp(txt, "1#") == 0)
            {
                switch_to_number_mode();
            }
            // 字母切换按钮 (数字键盘上的"abc")
            else if (strcmp(txt, "abc") == 0 && current_kb_mode == KB_MODE_NUMBER)
            {
                switch_to_alpha_mode();
            }
            // 大小写切换按钮 (字母键盘上的"abc"或"ABC")
            else if (strcmp(txt, "abc") == 0 || strcmp(txt, "ABC") == 0)
            {
                // 确保只在字母模式下处理大小写切换
                if (current_kb_mode == KB_MODE_ALPHA)
                {
                    toggle_case_mode();
                }
            }
            // 处理对号图标（完成按钮）和键盘图标（关闭按钮）
            else if (strcmp(txt, LV_SYMBOL_OK) == 0 ||    // 对号图标
                     strcmp(txt, LV_SYMBOL_CLOSE) == 0 || // 关闭图标
                     strcmp(txt, "Done") == 0 ||          // 完成文本
                     strcmp(txt, "Close") == 0)
            { // 关闭文本
                // 模拟按下键盘的确定按钮
                lv_event_send(kb, LV_EVENT_READY, NULL);
            }
        }
    }
}
void Screen9date_preloading(void)
{
    keyboard = lv_keyboard_create(lv_scr_act());
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(keyboard, 240, 120);
    lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(keyboard, keyboard_event, LV_EVENT_ALL, NULL);

    // 创建组
    Screen9group = lv_group_create();

    // 将所有可聚焦对象添加到组中
    lv_group_add_obj(Screen9group, guider_ui.screen_9_ta_1); // 文本框
    lv_group_add_obj(Screen9group, keyboard);                // 键盘

    // 设置文本框的光标属性
    lv_textarea_set_cursor_click_pos(guider_ui.screen_9_ta_1, true);          // 允许点击移动光标
    lv_obj_set_style_anim_time(guider_ui.screen_9_ta_1, 500, LV_PART_CURSOR); // 设置光标闪烁时间(毫秒)

    // 自定义光标样式
    static lv_style_t cursor_style;
    lv_style_init(&cursor_style);
    lv_style_set_border_color(&cursor_style, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_border_width(&cursor_style, 2);
    lv_style_set_border_side(&cursor_style, LV_BORDER_SIDE_LEFT);
    lv_obj_add_style(guider_ui.screen_9_ta_1, &cursor_style, LV_PART_CURSOR);

    if (Mem_Screen == SCREEN_8) // 从新增通道编辑页面过来
    {
        // 新增不需要
    }
    else if (Mem_Screen == SCREEN_20) // 从通道编辑页面过来
    {
        char *current_data = CustomShadeDates[ShadePreIndex].DataArray;
        if (current_data != NULL && strlen(current_data) > 0) // 判断当前文本是否为空
        {
            lv_textarea_set_text(guider_ui.screen_9_ta_1, current_data); // 显示原来的名字
        }
        else
        {
            lv_textarea_set_text(guider_ui.screen_9_ta_1, "edit name");
        }
    }
    else if (Mem_Screen == SCREEN_57) // 从新增群组编辑页面过来
    {
        // 新增不需要
    }
    else if (Mem_Screen == SCREEN_59) // 从群组编辑页面过来
    {
        char *current_data = CustomGroupDates[GroupPreIndex].DataArray;
        if (current_data != NULL && strlen(current_data) > 0) // 判断当前文本是否为空
        {
            lv_textarea_set_text(guider_ui.screen_9_ta_1, current_data); // 显示原来的名字
        }
        else
        {
            lv_textarea_set_text(guider_ui.screen_9_ta_1, "edit name");
        }
    }
    else if (Mem_Screen == SCREEN_61) // 有记忆点先跳转记忆点
    {
        // 新增不需要
    }
    else if (Mem_Screen == SCREEN_62) // 有记忆点先跳转记忆点
    {
        char *current_data = CustonmTimers[g_TimerPreIndex].DataArray;
        if (current_data != NULL && strlen(current_data) > 0) // 判断当前文本是否为空
        {
            lv_textarea_set_text(guider_ui.screen_9_ta_1, current_data); // 显示原来的名字
        }
        else
        {
            lv_textarea_set_text(guider_ui.screen_9_ta_1, "edit name");
        }
    }

    // 设置初始焦点
    lv_group_focus_obj(guider_ui.screen_9_ta_1);
}
