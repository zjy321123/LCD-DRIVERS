#include "Screens.h"
#include "gui_guider.h"
#include "Customdate.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
#include "power.h"
#include "Lms6ds3tr.h"

#define Factory_checkidems 13
static const uint8_t all_zero[8] = {0x00, 0x00, 0x00, 0x00};
static const uint8_t all_ff[4] = {0xFF, 0xFF, 0xFF, 0xFF};
static uint8_t RebootTick = 5;
extern current_screen_t current_screen, Pre_screen;
extern uint16_t Autotick;
extern uint8_t System_FactoryTestFlag;
extern S_LOCALTIMEDATA g_LocalTime;
uint8_t testoknums;
typedef struct
{
    uint8_t BheindKeytest : 1;
    uint8_t ForwardKeytest : 1;
    uint8_t LeftKeytest : 1;
    uint8_t RightKeytest : 1;
    uint8_t EnterKeytest : 1;
    uint8_t RetuenKeytest : 1;
    uint8_t MenuKeytest : 1;
    uint8_t Addrtest : 1;
    uint8_t Rtcdatetest : 1;
    uint8_t lsm6d3tr_IDtestok : 1;
    uint8_t lsm6d3tr_waketestok : 1;
    uint8_t si4460_testok : 1;
    uint8_t w25q64_testok : 1;
    uint8_t batcharge_testok : 1;
} S_Factory_Test;
S_Factory_Test Factory_test;

static uint8_t Factory_CountPassed(void)
{
    uint8_t cnt = 0;
    cnt += (Factory_test.BheindKeytest != 0);
    cnt += (Factory_test.ForwardKeytest != 0);
    cnt += (Factory_test.LeftKeytest != 0);
    cnt += (Factory_test.RightKeytest != 0);
    cnt += (Factory_test.EnterKeytest != 0);
    cnt += (Factory_test.RetuenKeytest != 0);
    cnt += (Factory_test.MenuKeytest != 0);
    cnt += (Factory_test.Addrtest != 0);
    cnt += (Factory_test.Rtcdatetest != 0);
    // LSM6D3TR作为一个测试项：ID和Wake都OK才算1项通过
    cnt += ((Factory_test.lsm6d3tr_IDtestok != 0) && (Factory_test.lsm6d3tr_waketestok != 0)) ? 1 : 0;
    cnt += (Factory_test.si4460_testok != 0);
    cnt += (Factory_test.w25q64_testok != 0);
    cnt += ((Factory_test.batcharge_testok) && (PowesSystem.BatteryLevel > 0)) ? 1 : 0;
    return cnt;
}
void Factory_SetSi4460TestOk(uint8_t v)
{
    Factory_test.si4460_testok = v;
}
void Factory_SetLsm6d3trIDTestOk(uint8_t v)
{
    Factory_test.lsm6d3tr_IDtestok = v;
}
void Factory_SetLsm6d3trWakeTestOk(uint8_t v)
{
    Factory_test.lsm6d3tr_waketestok = v;
}
void Factory_SetW25q64TestOk(uint8_t v)
{
    Factory_test.w25q64_testok = v;
}
void Factory_SetBatchargeTestOk(uint8_t v)
{
    Factory_test.batcharge_testok = v;
}
void Screen_FactoryTest_Movie(unsigned int Keyid)
{
    if (Keyid == KEYBEHIND_TRG)
    {
        keyboard_SendCmdTime(CMD_STOP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        Factory_test.BheindKeytest = 1;
    }
    else if (Keyid == KEYFORWARD_TRG)
    {
        keyboard_SendCmdTime(CMD_STOP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        Factory_test.ForwardKeytest = 1;
    }
    else if (Keyid == KEYLEFT_TRG)
    {
        keyboard_SendCmdTime(CMD_STOP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        Factory_test.LeftKeytest = 1;
    }
    else if (Keyid == KEYRIGHT_TRG)
    {
        keyboard_SendCmdTime(CMD_STOP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        Factory_test.RightKeytest = 1;
    }
    else if (Keyid == KEYRETURN_TRG)
    {
        keyboard_SendCmdTime(CMD_STOP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        Factory_test.RetuenKeytest = 1;
    }
    else if (Keyid == KEYENTER_TRG)
    {
        keyboard_SendCmdTime(CMD_STOP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        Factory_test.EnterKeytest = 1;
    }
    else if (Keyid == KEYMENU_TRG)
    {
        keyboard_SendCmdTime(CMD_STOP, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
        Factory_test.MenuKeytest = 1;
    }
    Screen_FactoryTest_preloading();
}

void Screen_FactoryTest_preloading(void)
{
    static uint8_t presec;
    static uint8_t presec_inited = 0;
    static uint8_t date_done = 0;
    if (current_screen != SCREEN_FACTORY_TEST)
        return;
    Factory_test.Addrtest = (memcmp(System_Addr, all_zero, 4) != 0) && (memcmp(System_Addr, all_ff, 4) != 0);
    if (!date_done)
    {
        if (!presec_inited)
        {
            presec = g_LocalTime.Seconds;
            presec_inited = 1;
        }
        else if (g_LocalTime.Seconds != presec)
        {
            Factory_test.Rtcdatetest = 1;
            date_done = 1;
            presec = g_LocalTime.Seconds;
        }
    }

    if (Factory_test.Addrtest)
    {
        lv_obj_set_style_text_color(guider_ui.Factory_Test_label_3, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_fmt(guider_ui.Factory_Test_label_3, "SystemAddr: %x%x%x%x", System_Addr[0], System_Addr[1], System_Addr[2], System_Addr[3]);
    }
    else
    {
        lv_obj_set_style_text_color(guider_ui.Factory_Test_label_3, lv_color_hex(0xf90026), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_fmt(guider_ui.Factory_Test_label_3, "SystemAddr: %x%x%x%x", System_Addr[0], System_Addr[1], System_Addr[2], System_Addr[3]);
    }
    if (Factory_test.Rtcdatetest)
    {
        lv_obj_set_style_text_color(guider_ui.Factory_Test_label_4, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_fmt(guider_ui.Factory_Test_label_4, "Date: %d : %d : %d", g_LocalTime.Hours, g_LocalTime.Minutes, g_LocalTime.Seconds);
    }
    else
    {
        lv_obj_set_style_text_color(guider_ui.Factory_Test_label_4, lv_color_hex(0xf90026), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_fmt(guider_ui.Factory_Test_label_4, "Date: %d : %d : %d", g_LocalTime.Hours, g_LocalTime.Minutes, g_LocalTime.Seconds);
    }
    if (Factory_test.si4460_testok)
    {
        lv_obj_set_style_text_color(guider_ui.Factory_Test_label_5, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_fmt(guider_ui.Factory_Test_label_5, "IC_RF:Success");
    }
    else
    {
        lv_obj_set_style_text_color(guider_ui.Factory_Test_label_5, lv_color_hex(0xf90026), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_fmt(guider_ui.Factory_Test_label_5, "IC_RF:Fail");
    }
    if (Factory_test.lsm6d3tr_IDtestok && Factory_test.lsm6d3tr_waketestok)
    {
        lv_obj_set_style_text_color(guider_ui.Factory_Test_label_9, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_fmt(guider_ui.Factory_Test_label_9, "IC_WAKE:Success");
    }
    else
    {
        lv_obj_set_style_text_color(guider_ui.Factory_Test_label_9, lv_color_hex(0xf90026), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_fmt(guider_ui.Factory_Test_label_9, "IC_WAKE:Fail");
    }
    if (Factory_test.w25q64_testok)
    {
        lv_obj_set_style_text_color(guider_ui.Factory_Test_label_10, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_fmt(guider_ui.Factory_Test_label_10, "IC_STORAGE:Success");
    }
    else
    {
        lv_obj_set_style_text_color(guider_ui.Factory_Test_label_10, lv_color_hex(0xf90026), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_fmt(guider_ui.Factory_Test_label_10, "IC_STORAGE:Fail");
    }
    if ((Factory_test.batcharge_testok) && (PowesSystem.BatteryLevel > 0))
    {
        lv_obj_set_style_text_color(guider_ui.Factory_Test_label_11, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_fmt(guider_ui.Factory_Test_label_11, "IC_CHARGE:%d%%  charge ok", PowesSystem.BatteryLevel);
    }
    else
    {
        lv_obj_set_style_text_color(guider_ui.Factory_Test_label_11, lv_color_hex(0xf90026), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_fmt(guider_ui.Factory_Test_label_11, "IC_CHARGE:%d%% Ready", PowesSystem.BatteryLevel);
    }

    if (Factory_test.BheindKeytest)
        lv_obj_set_style_bg_color(guider_ui.Factory_Test_btn_3, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    else
        lv_obj_set_style_bg_color(guider_ui.Factory_Test_btn_3, lv_color_hex(0xf90026), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Factory_test.ForwardKeytest)
        lv_obj_set_style_bg_color(guider_ui.Factory_Test_btn_1, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    else
        lv_obj_set_style_bg_color(guider_ui.Factory_Test_btn_1, lv_color_hex(0xf90026), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Factory_test.LeftKeytest)
        lv_obj_set_style_bg_color(guider_ui.Factory_Test_btn_6, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    else
        lv_obj_set_style_bg_color(guider_ui.Factory_Test_btn_6, lv_color_hex(0xf90026), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Factory_test.RightKeytest)
        lv_obj_set_style_bg_color(guider_ui.Factory_Test_btn_7, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    else
        lv_obj_set_style_bg_color(guider_ui.Factory_Test_btn_7, lv_color_hex(0xf90026), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Factory_test.RetuenKeytest)
        lv_obj_set_style_bg_color(guider_ui.Factory_Test_btn_4, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    else
        lv_obj_set_style_bg_color(guider_ui.Factory_Test_btn_4, lv_color_hex(0xf90026), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Factory_test.EnterKeytest)
        lv_obj_set_style_bg_color(guider_ui.Factory_Test_btn_2, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    else
        lv_obj_set_style_bg_color(guider_ui.Factory_Test_btn_2, lv_color_hex(0xf90026), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (Factory_test.MenuKeytest)
        lv_obj_set_style_bg_color(guider_ui.Factory_Test_btn_5, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    else
        lv_obj_set_style_bg_color(guider_ui.Factory_Test_btn_5, lv_color_hex(0xf90026), LV_PART_MAIN | LV_STATE_DEFAULT);
    testoknums = Factory_CountPassed();
    if (testoknums != Factory_checkidems)
    {
        lv_bar_set_value(guider_ui.Factory_Test_bar_1, (testoknums * 100) / Factory_checkidems, LV_ANIM_OFF);
        lv_obj_set_style_text_color(guider_ui.Factory_Test_bar_1, lv_color_hex(0xf90026), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_bar_set_value(guider_ui.Factory_Test_bar_1, (testoknums * 100) / Factory_checkidems, LV_ANIM_OFF);
        lv_obj_set_style_text_color(guider_ui.Factory_Test_bar_1, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    if (testoknums != Factory_checkidems)
    {
        lv_obj_set_style_text_color(guider_ui.Factory_Test_label_2, lv_color_hex(0xf90026), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_fmt(guider_ui.Factory_Test_label_2, "%d%%", (testoknums * 100) / Factory_checkidems);
    }
    else
    {
        lv_obj_set_style_text_color(guider_ui.Factory_Test_label_2, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text_fmt(guider_ui.Factory_Test_label_2, "%d%%", (testoknums * 100) / Factory_checkidems);
    }
    if (testoknums == Factory_checkidems)
    {
        lv_obj_clear_flag(guider_ui.Factory_Test_label_13, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text_fmt(guider_ui.Factory_Test_label_13, "%dS", RebootTick);
        RebootTick--;
        if (RebootTick == 0)
        {
            System_FactoryTestFlag = 0XAF; // 出厂完成
            Lsm6d3trsystem.status = DEFAULT_GYROSWITCH;
            SaveDataToFlash();
            __set_FAULTMASK(1);     // 关闭所有中断
            HAL_NVIC_SystemReset(); // 执行系统复位
        }
    }
    else
    {
        lv_obj_add_flag(guider_ui.Factory_Test_label_13, LV_OBJ_FLAG_HIDDEN);
    }
}
