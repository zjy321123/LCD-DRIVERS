#ifndef __MYGUIGUIDERUIS_H__
#define __MYGUIGUIDERUIS_H__

#include "lvgl.h"
#include "gui_guider.h"

typedef enum
{
    KEY_NONE = 0,
    KEY_FORWARD,
    KEY_RIGHT,
    KEY_MENU,
    KEY_ENTER,
    KEY_BEHIND,
    KEY_RETURN,
    KEY_LEFT
} KeyCode_t;

typedef enum
{
    GUIDER_SCREEN,
    GUIDER_SCREEN_1,
    GUIDER_SCREEN_2,
    GUIDER_SCREEN_3,
    GUIDER_SCREEN_4,
    GUIDER_SCREEN_NULL
} guider_screen_t;

extern guider_screen_t current_guider_screen;

void GuiderScreen_Init(void);
void GuiderScreen_SetupAfterCreate(guider_screen_t screen_type);
void screen_monies(void);
KeyCode_t GetKeyPressed(void);
void GuiderScreen_KeysProcess(KeyCode_t key);
uint8_t GuiderScreen_IsLoading(void);
uint8_t GuiderScreen_IsObjValid(lv_obj_t *obj);

void GuiderScreen_Movie(KeyCode_t key);
void GuiderScreen_1_Movie(KeyCode_t key);
void GuiderScreen_2_Movie(KeyCode_t key);
void GuiderScreen_3_Movie(KeyCode_t key);
void GuiderScreen_4_Movie(KeyCode_t key);

#endif
