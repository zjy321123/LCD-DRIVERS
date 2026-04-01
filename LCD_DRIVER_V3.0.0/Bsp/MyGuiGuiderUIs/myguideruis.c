#include "myguideruis.h"
#include <stdlib.h>
#include "lvgl.h"
#include "main.h"

extern lv_ui guider_ui;

guider_screen_t current_guider_screen = GUIDER_SCREEN;

static uint32_t last_key_time = 0;
static const uint32_t KEY_DEBOUNCE_MS = 50;

static const int SCREEN_ID_0 = 0;
static const int SCREEN_ID_1 = 1;
static const int SCREEN_ID_2 = 2;
static const int SCREEN_ID_3 = 3;
static const int SCREEN_ID_4 = 4;

static void screen_loaded_cb(lv_event_t *e)
{
    lv_obj_t *scr = lv_event_get_target(e);
    int *id = (int *)lv_obj_get_user_data(scr);

    if (id == &SCREEN_ID_0)
        current_guider_screen = GUIDER_SCREEN;
    else if (id == &SCREEN_ID_1)
        current_guider_screen = GUIDER_SCREEN_1;
    else if (id == &SCREEN_ID_2)
        current_guider_screen = GUIDER_SCREEN_2;
    else if (id == &SCREEN_ID_3)
        current_guider_screen = GUIDER_SCREEN_3;
    else if (id == &SCREEN_ID_4)
        current_guider_screen = GUIDER_SCREEN_4;
    else
        current_guider_screen = GUIDER_SCREEN_NULL;
}

static void set_screen_id(lv_obj_t *scr, const int *id)
{
    lv_obj_set_user_data(scr, (void *)id);
    lv_obj_add_event_cb(scr, screen_loaded_cb, LV_EVENT_SCREEN_LOADED, NULL);
}

void GuiderScreen_Init(void)
{
    lv_obj_set_user_data(guider_ui.screen, (void *)&SCREEN_ID_0);
    lv_obj_add_event_cb(guider_ui.screen, screen_loaded_cb, LV_EVENT_SCREEN_LOADED, NULL);

    current_guider_screen = GUIDER_SCREEN;
}

void GuiderScreen_SetupAfterCreate(guider_screen_t screen_type)
{
    switch (screen_type)
    {
    case GUIDER_SCREEN:
        set_screen_id(guider_ui.screen, &SCREEN_ID_0);
        break;
    case GUIDER_SCREEN_1:
        set_screen_id(guider_ui.screen_1, &SCREEN_ID_1);
        break;
    case GUIDER_SCREEN_2:
        set_screen_id(guider_ui.screen_2, &SCREEN_ID_2);
        break;
    case GUIDER_SCREEN_3:
        set_screen_id(guider_ui.screen_3, &SCREEN_ID_3);
        break;
    case GUIDER_SCREEN_4:
        set_screen_id(guider_ui.screen_4, &SCREEN_ID_4);
        break;
    default:
        break;
    }
}

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

uint8_t GuiderScreen_IsObjValid(lv_obj_t *obj)
{
    if (obj == NULL)
        return 0;
    return lv_obj_is_valid(obj);
}

void GuiderScreen_KeysProcess(KeyCode_t key)
{
    lv_obj_t *scr = lv_scr_act();

    if (lv_obj_is_valid(guider_ui.screen_btn_1) && lv_obj_get_screen(guider_ui.screen_btn_1) == scr)
        current_guider_screen = GUIDER_SCREEN;
    else if (lv_obj_is_valid(guider_ui.screen_1_btn_1) && lv_obj_get_screen(guider_ui.screen_1_btn_1) == scr)
        current_guider_screen = GUIDER_SCREEN_1;
    else if (lv_obj_is_valid(guider_ui.screen_2_btn_1) && lv_obj_get_screen(guider_ui.screen_2_btn_1) == scr)
        current_guider_screen = GUIDER_SCREEN_2;
    else if (lv_obj_is_valid(guider_ui.screen_3_btn_1) && lv_obj_get_screen(guider_ui.screen_3_btn_1) == scr)
        current_guider_screen = GUIDER_SCREEN_3;
    else if (lv_obj_is_valid(guider_ui.screen_4_btn_1) && lv_obj_get_screen(guider_ui.screen_4_btn_1) == scr)
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

void GuiderScreen_1_Movie(KeyCode_t key)
{
    if (!GuiderScreen_IsObjValid(guider_ui.screen_1_btn_1))
        return;

    switch (key)
    {
    case KEY_BEHIND:
        lv_obj_send_event(guider_ui.screen_1_btn_1, LV_EVENT_CLICKED, NULL);
        break;
    case KEY_RIGHT:
        lv_obj_send_event(guider_ui.screen_1_btn_1, LV_EVENT_SHORT_CLICKED, NULL);
        break;
    case KEY_FORWARD:
        lv_obj_send_event(guider_ui.screen_1_btn_1, LV_EVENT_PRESSED, NULL);
        break;
    case KEY_LEFT:
        lv_obj_send_event(guider_ui.screen_1_btn_1, LV_EVENT_PRESSING, NULL);
        break;
    default:
        return;
    }
}

void GuiderScreen_2_Movie(KeyCode_t key)
{
    if (!GuiderScreen_IsObjValid(guider_ui.screen_2_btn_1))
        return;

    switch (key)
    {
    case KEY_LEFT:
        lv_obj_send_event(guider_ui.screen_2_btn_1, LV_EVENT_CLICKED, NULL);
        break;
    case KEY_BEHIND:
        lv_obj_send_event(guider_ui.screen_2_btn_1, LV_EVENT_SHORT_CLICKED, NULL);
        break;
    case KEY_FORWARD:
        lv_obj_send_event(guider_ui.screen_2_btn_1, LV_EVENT_PRESSED, NULL);
        break;
    case KEY_RIGHT:
        lv_obj_send_event(guider_ui.screen_2_btn_1, LV_EVENT_PRESSING, NULL);
        break;
    default:
        return;
    }
}

void GuiderScreen_3_Movie(KeyCode_t key)
{
    if (!GuiderScreen_IsObjValid(guider_ui.screen_3_btn_1))
        return;

    switch (key)
    {
    case KEY_LEFT:
        lv_obj_send_event(guider_ui.screen_3_btn_1, LV_EVENT_CLICKED, NULL);
        break;
    case KEY_FORWARD:
        lv_obj_send_event(guider_ui.screen_3_btn_1, LV_EVENT_SHORT_CLICKED, NULL);
        break;
    case KEY_RIGHT:
        lv_obj_send_event(guider_ui.screen_3_btn_1, LV_EVENT_PRESSED, NULL);
        break;
    case KEY_BEHIND:
        lv_obj_send_event(guider_ui.screen_3_btn_1, LV_EVENT_PRESSING, NULL);
        break;
    default:
        return;
    }
}

void GuiderScreen_4_Movie(KeyCode_t key)
{
    if (!GuiderScreen_IsObjValid(guider_ui.screen_4_btn_1))
        return;

    switch (key)
    {
    case KEY_LEFT:
        lv_obj_send_event(guider_ui.screen_4_btn_1, LV_EVENT_CLICKED, NULL);
        break;
    case KEY_FORWARD:
        lv_obj_send_event(guider_ui.screen_4_btn_1, LV_EVENT_SHORT_CLICKED, NULL);
        break;
    case KEY_RIGHT:
        lv_obj_send_event(guider_ui.screen_4_btn_1, LV_EVENT_PRESSED, NULL);
        break;
    case KEY_BEHIND:
        lv_obj_send_event(guider_ui.screen_4_btn_1, LV_EVENT_PRESSING, NULL);
        break;
    default:
        return;
    }
}

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
