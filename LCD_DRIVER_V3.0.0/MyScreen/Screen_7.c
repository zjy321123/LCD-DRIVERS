#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
extern current_screen_t current_screen, Pre_screen, Mem_Screen;
extern uint16_t Autotick;
extern uint8_t Scre43Cnt, Scre49Cnt, Index_Scr52, Scre49NOasync, Scre52NOasync, Scre46NOasync, Scre43NOasync;
void Screen_7_Movie(unsigned int Keyid)
{
    if (Keyid == KEYENTER_TRG) // 省内存重复利用
    {
        if (Pre_screen == SCREEN_6) // 对码 // 加载界面screen_8
        {

            if (((SCREEN_6AutoLoadtime / 3) < Autotick)) // 防止跳过加载动画时重复换
            {
                Pre_screen = SCREEN_NUll;
                lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_CLICKED, NULL);
                keyboard_SendCmdTime(CMD_PRG, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S);
                current_screen = SCREEN_8;
                Autotick = 0;
                Screen8date_preloading();
            }
        }
        else if (Pre_screen == SCREEN_22)
        {
            if (((SCREEN_22AutoLoadtime / 3) < Autotick))
            {
                Pre_screen = SCREEN_NUll;
                lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_PRESS_LOST, NULL);   // 加载界面screen_23
                keyboard_SendCmdTime(CMD_SET_2SEC, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 查询电机信息
                current_screen = SCREEN_23;
                Autotick = 0;
                Screen23date_preloading();
            }
        }
        else if (Pre_screen == SCREEN_24)
        {
            if (((SCREEN_24AutoLoadtime / 3) < Autotick))
            {
                Pre_screen = SCREEN_NUll;
                lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_RELEASED, NULL);     // 加载界面screen_25
                keyboard_SendCmdTime(CMD_SET_2SEC, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 查询电机信息
                current_screen = SCREEN_25;
                Autotick = 0;
                Screen25date_preloading();
            }
        }
        else if (Pre_screen == SCREEN_26)
        {
            if (((SCREEN_26AutoLoadtime / 3) < Autotick)) // 防止跳过加载动画时重复换
            {
                Pre_screen = SCREEN_NUll;
                lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_VALUE_CHANGED, NULL);     // 加载界面screen_27
                keyboard_SendCmdTime(CMD_SET_STOP_2SEC, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 最佳行程点
                current_screen = SCREEN_27;
                Autotick = 0;
                Screen27date_preloading();
            }
        }
        else if (Pre_screen == SCREEN_28)
        {
            // Pre_screen = SCREEN_NUll;
            // lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_LONG_PRESSED, NULL); // 加载界面screen_29
            // current_screen = SCREEN_29;
            // Autotick = 0;
            // Screen29date_preloading();
        }
        else if (Pre_screen == SCREEN_43)
        {
            if (((SCREEN_43AutoLoadtime / 3) < Autotick))
            {
                lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_DEFOCUSED, NULL); // 加载界面screen_44
                Scre43NOasync = 1;
                if (Scre43Cnt == 0)
                    keyboard_SendCmdTime(CMD_TURNON_LED, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 开灯
                else if (Scre43Cnt == 1)
                    keyboard_SendCmdTime(CMD_TURNOFF_LED, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 关灯
                // Pre_screen = SCREEN_NUll; // 借用判断灯光类型了，发射完处理
                current_screen = SCREEN_44;
                Autotick = 0;
                Screen44date_preloading();
            }
        }
        else if (Pre_screen == SCREEN_46)
        {
            if (((SCREEN_46AutoLoadtime / 3) < Autotick))
            {
                // Pre_screen = SCREEN_NUll;                                          // 判断遇阻类型了，发射完处理
                lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_LEAVE, NULL); // 加载界面screen_47
                Scre46NOasync = 1;
                keyboard_SendCmdTime(CMD_ONSTACLE_SWITCH, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻开关
                current_screen = SCREEN_47;
                Autotick = 0;
                Screen47date_preloading();
            }
        }
        else if (Pre_screen == SCREEN_49)
        {
            if (((SCREEN_49AutoLoadtime / 3) < Autotick)) // 防止跳过加载动画时重复发
            {
                // Pre_screen = SCREEN_NUll;                                             // 判断遇阻类型了，发射完处理
                lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_HIT_TEST, NULL); // 加载界面screen_50
                Scre49NOasync = 1;                                                    // 防止异步回调

                if (Scre49Cnt == 0)
                    keyboard_SendCmdTime(CMD_ONSTACLE_STOPMODE, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻停
                else if (Scre49Cnt == 1)
                    keyboard_SendCmdTime(CMD_ONSTACLE_REBOUNDMODE, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻回弹
                else if (Scre49Cnt == 2)
                    keyboard_SendCmdTime(CMD_ONSTACLE_UPMODE, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻上升

                current_screen = SCREEN_50;
                Autotick = 0;
                Screen50date_preloading();
            }
        }
        else if (Pre_screen == SCREEN_52) // 需Pre_screen，确保手动发送能发完完整数据，此时需要防止异步回调
        {
            if (((SCREEN_52AutoLoadtime / 3) < Autotick)) // 防止跳过加载动画时重复发
            {
                // Pre_screen = SCREEN_NUll;                                        // 判断遇阻类型了，发射完处理
                lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_KEY, NULL); // 加载界面screen_53
                Scre52NOasync = 1;                                               // 防止异步回调
                if (Index_Scr52 == 0)
                    keyboard_SendCmdTime(CMD_ONSTACLE_ONE, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻一档灵敏度
                else if (Index_Scr52 == 1)
                    keyboard_SendCmdTime(CMD_ONSTACLE_TWO, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻二档灵敏度
                else if (Index_Scr52 == 2)
                    keyboard_SendCmdTime(CMD_ONSTACLE_THREE, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻三档灵敏度
                else if (Index_Scr52 == 3)
                    keyboard_SendCmdTime(CMD_ONSTACLE_FOUR, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻四档灵敏度
                else if (Index_Scr52 == 4)
                    keyboard_SendCmdTime(CMD_ONSTACLE_FIVE, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 遇阻五档灵敏度
                current_screen = SCREEN_53;
                Autotick = 0;
                Screen53date_preloading();
            }
        }
        else if (Pre_screen == SCREEN_54)
        {
            if (Autotick < (SCREEN_54AutoLoadtime / 3)) // 长时间的操作取消快速按不然容易误触发
            {
                Pre_screen = SCREEN_NUll;
                lv_event_send(guider_ui.screen_7_label_6, LV_EVENT_CLICKED, NULL);         // 加载界面screen_55
                keyboard_SendCmdTime(CMD_RESET_ALL, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 出厂化
                current_screen = SCREEN_55;
                Autotick = 0;
                Screen55date_preloading();
            }
        }
        else if (Pre_screen == SCREEN_77)
        {
            extern uint8_t Scre77Cnt;
            if (Autotick < (SCREEN_77AutoLoadtime / 3))
            {
                Pre_screen = SCREEN_NUll;
                // Mem_Screen = SCREEN_NUll;
                lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_PRESSED, NULL); // 加载界面screen_15
                if (Scre77Cnt == 0)
                    keyboard_SendCmdTime(CMD_SET_UP_6SEC, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 删除上行程
                else if (Scre77Cnt == 1)
                    keyboard_SendCmdTime(CMD_SET_UP_2SEC, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 设置上行程
                current_screen = SCREEN_15;
                Autotick = 0;
                Screen15date_preloading();
            }
        }
        else if (Pre_screen == SCREEN_79)
        {
            extern uint8_t Scre79Cnt;
            if (Autotick < (SCREEN_79AutoLoadtime / 3))
            {
                Pre_screen = SCREEN_NUll;
                // Mem_Screen = SCREEN_NUll;
                lv_event_send(guider_ui.screen_7_spinner_1, LV_EVENT_PRESSING, NULL); // 加载界面screen_17
                if (Scre79Cnt == 0)
                    keyboard_SendCmdTime(CMD_SET_DOWN_6SEC, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 删除下行程
                else if (Scre79Cnt == 1)
                    keyboard_SendCmdTime(CMD_SET_DOWN_2SEC, KEY_DELAY_2SEC, E_KEY_DELAY_START_2S); // 设置下行程
                current_screen = SCREEN_17;
                Autotick = 0;
                Screen17date_preloading();
            }
        }
        // return;
    }
}
