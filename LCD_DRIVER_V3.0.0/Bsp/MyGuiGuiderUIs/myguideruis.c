#include "myguideruis.h"
#include <stdlib.h>
#include "lvgl.h"
#include "main.h"

extern lv_ui guider_ui;

guider_screen_t current_guider_screen = GUIDER_SCREEN;

static uint32_t last_key_time = 0;
static const uint32_t KEY_DEBOUNCE_MS = 50;
/**
 * @brief   屏幕加载事件回调函数
 * @author  zjy
 * @date    2025.04.02
 * @param   e: LVGL事件指针
 * @retval  无
 */
static void screen_loaded_cb(lv_event_t *e)
{
    lv_obj_t *scr = lv_event_get_target(e);
    guider_screen_t screen_id = (guider_screen_t)(uintptr_t)lv_obj_get_user_data(scr);

    if (screen_id < GUIDER_SCREEN_NULL)
        current_guider_screen = screen_id;
    else
        current_guider_screen = GUIDER_SCREEN_NULL;
}
/**
 * @brief   设置屏幕ID
 * @author  zjy
 * @date    2025.04.02
 * @param   scr: 屏幕对象指针
 * @param   id: 屏幕ID
 * @retval  无
 */
static void set_screen_id(lv_obj_t *scr, guider_screen_t id)
{
    lv_obj_set_user_data(scr, (void *)(uintptr_t)id);
    lv_obj_add_event_cb(scr, screen_loaded_cb, LV_EVENT_SCREEN_LOADED, NULL);
}
/**
 * @brief   初始化屏幕
 * @author  zjy
 * @date    2025.04.02
 * @retval  无
 */
void GuiderScreen_Init(void)
{
    lv_obj_set_user_data(guider_ui.screen, (void *)GUIDER_SCREEN);
    lv_obj_add_event_cb(guider_ui.screen, screen_loaded_cb, LV_EVENT_SCREEN_LOADED, NULL);

    current_guider_screen = GUIDER_SCREEN;
}
/**
 * @brief   初始化屏幕
 * @author  zjy
 * @date    2025.04.02
 * @param   screen_type: 屏幕类型
 * @retval  无
 */
void GuiderScreen_SetupAfterCreate(guider_screen_t screen_type)
{
    switch (screen_type)
    {
    case GUIDER_SCREEN:
        set_screen_id(guider_ui.screen, GUIDER_SCREEN);
        break;
    case GUIDER_SCREEN_1:
        set_screen_id(guider_ui.screen_1, GUIDER_SCREEN_1);
        break;
    case GUIDER_SCREEN_2:
        set_screen_id(guider_ui.screen_2, GUIDER_SCREEN_2);
        break;
    case GUIDER_SCREEN_3:
        set_screen_id(guider_ui.screen_3, GUIDER_SCREEN_3);
        break;
    case GUIDER_SCREEN_4:
        set_screen_id(guider_ui.screen_4, GUIDER_SCREEN_4);
        break;
    default:
        break;
    }
}
/**
 * @brief   获取按键事件
 * @author  zjy
 * @date    2025.04.02
 * @retval  按键事件
 */
KeyCode_t GetKeyPressed(void)
{
    if (HAL_GPIO_ReadPin(ForwardKey1_GPIO_Port, ForwardKey1_Pin) == GPIO_PIN_RESET)
        return KEY_FORWARD;
    if (HAL_GPIO_ReadPin(RightKey5_GPIO_Port, RightKey5_Pin) == GPIO_PIN_RESET)
        return KEY_RIGHT;
    if (HAL_GPIO_ReadPin(MenuKey7_GPIO_Port, MenuKey7_Pin) == GPIO_PIN_RESET)
        return KEY_MENU;
    if (HAL_GPIO_ReadPin(EnterKey2_GPIO_Port, EnterKey2_Pin) == GPIO_PIN_RESET)
        return KEY_ENTER;
    if (HAL_GPIO_ReadPin(BehindKey3_GPIO_Port, BehindKey3_Pin) == GPIO_PIN_RESET)
        return KEY_BEHIND;
    if (HAL_GPIO_ReadPin(ReturnKey6_GPIO_Port, ReturnKey6_Pin) == GPIO_PIN_RESET)
        return KEY_RETURN;
    if (HAL_GPIO_ReadPin(LeftKey4_GPIO_Port, LeftKey4_Pin) == GPIO_PIN_RESET)
        return KEY_LEFT;
    return KEY_NONE;
}
/**
 * @brief   检查对象是否有效
 * @author  zjy
 * @date    2025.04.02
 * @param   obj: 待检查的对象指针
 * @retval  0: 无效
 * @retval  1: 有效
 */
uint8_t GuiderScreen_IsObjValid(lv_obj_t *obj)
{
    if (obj == NULL)
        return 0;
    return lv_obj_is_valid(obj);
}
/**
 * @brief   处理屏幕按键事件，定位当前有效屏幕
 * @author  zjy
 * @date    2025.04.02
 * @param   key: 按键事件
 * @retval  无
 * @note    当前为了完全与UI设计隔离开，是采用了独立按键，并不绑定到LVGL，改芯片只有36MSPI
 * 所以会出现快速切屏过程中，LVGL渲染要时间，会出现屏幕渲染到一半时，出现上页是新屏幕的控件，下半
 * 页是旧屏幕的控件，导致出现野指针或者获取实际屏幕错误，依旧为上一个，故此采用边界控件固定法采用，
 * 2个边界控件，一个上，一个下，用于判断当前屏幕是否为有效屏幕，且确保在切换屏幕时，边界控件的事件
 * 能够正常触发，从而更新当前屏幕。
 */
void GuiderScreen_KeysProcess(KeyCode_t key)
{
    lv_obj_t *scr = lv_scr_act();

    if (lv_obj_is_valid(guider_ui.screen_btn_uppos) && lv_obj_get_screen(guider_ui.screen_btn_uppos) == scr && lv_obj_is_valid(guider_ui.screen_btn_downpos) && lv_obj_get_screen(guider_ui.screen_btn_downpos) == scr)
        current_guider_screen = GUIDER_SCREEN;
    else if (lv_obj_is_valid(guider_ui.screen_1_btn_uppos) && lv_obj_get_screen(guider_ui.screen_1_btn_uppos) == scr && lv_obj_is_valid(guider_ui.screen_1_btn_downpos) && lv_obj_get_screen(guider_ui.screen_1_btn_downpos) == scr)
        current_guider_screen = GUIDER_SCREEN_1;
    else if (lv_obj_is_valid(guider_ui.screen_2_btn_uppos) && lv_obj_get_screen(guider_ui.screen_2_btn_uppos) == scr && lv_obj_is_valid(guider_ui.screen_2_btn_downpos) && lv_obj_get_screen(guider_ui.screen_2_btn_downpos) == scr)
        current_guider_screen = GUIDER_SCREEN_2;
    else if (lv_obj_is_valid(guider_ui.screen_3_btn_uppos) && lv_obj_get_screen(guider_ui.screen_3_btn_uppos) == scr && lv_obj_is_valid(guider_ui.screen_3_btn_downpos) && lv_obj_get_screen(guider_ui.screen_3_btn_downpos) == scr)
        current_guider_screen = GUIDER_SCREEN_3;
    else if (lv_obj_is_valid(guider_ui.screen_4_btn_uppos) && lv_obj_get_screen(guider_ui.screen_4_btn_uppos) == scr && lv_obj_is_valid(guider_ui.screen_4_btn_downpos) && lv_obj_get_screen(guider_ui.screen_4_btn_downpos) == scr)
        current_guider_screen = GUIDER_SCREEN_4;
    else
        return;

    switch (current_guider_screen)
    {
    case GUIDER_SCREEN:
        GuiderScreen_Movie(key);
        break;
    case GUIDER_SCREEN_1:
        GuiderScreen_1_Movie(key);
        break;
    case GUIDER_SCREEN_2:
        GuiderScreen_2_Movie(key);
        break;
    case GUIDER_SCREEN_3:
        GuiderScreen_3_Movie(key);
        break;
    case GUIDER_SCREEN_4:
        GuiderScreen_4_Movie(key);
        break;
    default:
        break;
    }
}




/**
 * @brief   处理屏幕按键事件
 * @author  zjy
 * @date    2025.04.02
 * @retval  无
 */
void screen_monies(void)
{
    uint32_t current_time = lv_tick_get();

    if (current_time - last_key_time < KEY_DEBOUNCE_MS)
        return;

    KeyCode_t key = GetKeyPressed();

    if (key != KEY_NONE)
    {
        last_key_time = current_time;
        GuiderScreen_KeysProcess(key);
    }
}
