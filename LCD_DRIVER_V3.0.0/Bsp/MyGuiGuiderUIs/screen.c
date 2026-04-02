/**
 * @file    screen.c
 * @brief   每一页UI的逻辑事件单独分开写，适合大量组件的情况
 * @author  zjy
 * @date    2026.04.02
 */

#include "myguideruis.h"
#include "lvgl.h"
/**
 * @brief   Screen 0 处理按键事件
 * @author  zjy
 * @date    2025.04.02
 * @param   key: 按键事件
 * @retval  无
 */
void GuiderScreen_Movie(KeyCode_t key)
{
    if (!GuiderScreen_IsObjValid(guider_ui.screen_btn_1))
        return;

    switch (key)
    {
    case KEY_FORWARD:
        lv_obj_send_event(guider_ui.screen_btn_1, LV_EVENT_CLICKED, NULL);
        break;
    case KEY_RIGHT:
        lv_obj_send_event(guider_ui.screen_btn_1, LV_EVENT_SHORT_CLICKED, NULL);
        break;
    case KEY_BEHIND:
        lv_obj_send_event(guider_ui.screen_btn_1, LV_EVENT_PRESSED, NULL);
        break;
    case KEY_LEFT:
        lv_obj_send_event(guider_ui.screen_btn_1, LV_EVENT_PRESSING, NULL);
        break;
    default:
        return;
    }
}


