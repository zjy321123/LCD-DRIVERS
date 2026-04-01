#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Syscfg.h"
#include "Customdate.h"
#include "Rfsend.h"
extern current_screen_t current_screen, Pre_screen;
static uint32_t Scr74_Ticks = 0;
static uint8_t Click_Combo = 0;
void Screen_74_Movie(unsigned int Keyid)
{
    if (Keyid == KEYENTER_TRG)
    {
        lv_event_send(guider_ui.screen_74_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_1
        current_screen = SCREEN_1;
        Screen1date_preloading();
        Click_Combo = Scr74_Ticks = 0;
        return;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        lv_event_send(guider_ui.screen_74_btn_2, LV_EVENT_SHORT_CLICKED, NULL); // 加载界面screen_1
        current_screen = SCREEN_1;
        Screen1date_preloading();
        Click_Combo = Scr74_Ticks = 0;
        return;
    }
    if (Keyid == KEYMENU_TRG)
    {
        Scr74_Ticks = 0;
        if (Click_Combo < 10)
            Click_Combo++;
    }
}
void Screen74date_preloading(void)
{

    Scr74_Ticks = 0;
    Click_Combo = 0;
    // Display version
    lv_label_set_text_fmt(guider_ui.screen_74_label_5, "version:%s", SOFTWARE_VERSION);

    // Generate MAC address: 87 bytes total
    // Odd positions (1,3,5): random bytes
    // Even positions (2,4,6,7): System_Addr[0,1,2,3]
    uint8_t mac[8];
    mac[0] = (uint8_t)(rand() & 0xFF); // Position 1: random
    mac[1] = System_Addr[0];           // Position 2: System_Addr[0]
    mac[2] = (uint8_t)(rand() & 0xFF); // Position 3: random
    mac[3] = System_Addr[1];           // Position 4: System_Addr[1]
    mac[4] = (uint8_t)(rand() & 0xFF); // Position 5: random
    mac[5] = System_Addr[2];           // Position 6: System_Addr[2]
    mac[6] = System_Addr[3];           // Position 7: System_Addr[3]

    // Display MAC address in format: XX:XX:XX:XX:XX:XX:XX:XX
    lv_label_set_text_fmt(guider_ui.screen_74_label_4, "MAC:%02X:%02X:%02X:%02X:%02X:%02X:%02X",
                          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], mac[6]);
}
void Screen74_AutoLoadProcess(void)
{
    static uint8_t fired_3 = 0;
    if (current_screen != SCREEN_74)
        return;

    // 超过连击时间窗口则清零
    if (Scr74_Ticks >= SCREEN_74AutoLoadtime)
    {
        Scr74_Ticks = 0;
        Click_Combo = 0;
        fired_3 = 0;
    }

    // 先判断 5 连击（优先级最高）
    if (Click_Combo >= 5)
    {
       extern uint8_t S_default_mode;
       S_default_mode = g_MsgSave.RadioMode;
        Scr74_Ticks = 0;
        Click_Combo = 0;
        lv_event_send(guider_ui.screen_74_btn_2, LV_EVENT_CLICKED, NULL); // 加载界面screen_labtest
        current_screen = SCREEN_LAB_TEST;
        Screen_LabTest_preloading();
        return;
    }

    // 再判断 3 连击：只触发一次，但不影响后续再继续连击到 5
    if (Click_Combo >= 3)
    {
 
        if (fired_3 == 0)
        {
            CustomDatePrint();
            fired_3 = 1;
        }
    }
    else
    {

    }

    Scr74_Ticks++;

    // 当时间窗口结束时，重置 3 连击触发锁
    if (Scr74_Ticks == 0)
    {
        // no-op
    }
}


