#include "Screens.h"
#include "gui_guider.h"
#include <stdlib.h>
#include "lvgl.h"
#include "Si446x.h"
#include "keyboard.h"
#include "Rfsend.h"
#include "ProtocolCmd.h"
#include "Power.h"
extern lv_indev_t *indev_keypad;
extern lv_ui guider_ui;
static uint16_t Powerr_MoveTimes;
extern current_screen_t current_screen;
// 全局UI实例
Power_ui_t Power_ui;
uint8_t RfMode = 0;
char RfMode_buffer[5];
// 初始化函数
void Power_ui_init(void)
{
    // 创建独立于屏幕的顶层容器
    Power_ui.top_layer = lv_obj_create(lv_layer_top());
    lv_obj_remove_style_all(Power_ui.top_layer); // 移除所有样式
    lv_obj_set_size(Power_ui.top_layer, LV_HOR_RES, LV_VER_RES);
    lv_obj_clear_flag(Power_ui.top_layer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(Power_ui.top_layer, LV_OBJ_FLAG_FLOATING | LV_OBJ_FLAG_IGNORE_LAYOUT);

    // 初始隐藏整个顶层
    // lv_obj_add_flag(Power_ui.top_layer, LV_OBJ_FLAG_HIDDEN);

    // 初始化点数组
    Power_ui.screen_line_7_points[0] = (lv_point_t){10, 10};
    Power_ui.screen_line_7_points[1] = (lv_point_t){0, 0};

    Power_ui.screen_line_6_points[0] = (lv_point_t){0, 0};
    Power_ui.screen_line_6_points[1] = (lv_point_t){10, 0};

    Power_ui.screen_line_5_points[0] = (lv_point_t){0, 0};
    Power_ui.screen_line_5_points[1] = (lv_point_t){10, 10};

    // 创建UI元素 - 使用top_layer作为父容器
    Power_ui.screen_line_7 = lv_line_create(Power_ui.top_layer);
    lv_line_set_points(Power_ui.screen_line_7, Power_ui.screen_line_7_points, 2);
    lv_obj_set_pos(Power_ui.screen_line_7, 208, 17);
    lv_obj_set_size(Power_ui.screen_line_7, 10, 10);
    lv_obj_add_flag(Power_ui.screen_line_7, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_line_width(Power_ui.screen_line_7, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(Power_ui.screen_line_7, lv_color_hex(0x2FDA64), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(Power_ui.screen_line_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(Power_ui.screen_line_7, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    Power_ui.screen_line_6 = lv_line_create(Power_ui.top_layer);
    lv_line_set_points(Power_ui.screen_line_6, Power_ui.screen_line_6_points, 2);
    lv_obj_set_pos(Power_ui.screen_line_6, 207, 16);
    lv_obj_set_size(Power_ui.screen_line_6, 10, 10);
    lv_obj_add_flag(Power_ui.screen_line_6, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_line_width(Power_ui.screen_line_6, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(Power_ui.screen_line_6, lv_color_hex(0x2FDA64), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(Power_ui.screen_line_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(Power_ui.screen_line_6, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    Power_ui.screen_line_5 = lv_line_create(Power_ui.top_layer);
    lv_line_set_points(Power_ui.screen_line_5, Power_ui.screen_line_5_points, 2);
    lv_obj_set_pos(Power_ui.screen_line_5, 208, 6);
    lv_obj_set_size(Power_ui.screen_line_5, 10, 10);
    lv_obj_add_flag(Power_ui.screen_line_5, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_line_width(Power_ui.screen_line_5, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(Power_ui.screen_line_5, lv_color_hex(0x2FDA64), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(Power_ui.screen_line_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(Power_ui.screen_line_5, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    Power_ui.screen_bar_10 = lv_bar_create(Power_ui.top_layer);
    lv_obj_set_style_anim_time(Power_ui.screen_bar_10, 1000, 0);
    lv_bar_set_mode(Power_ui.screen_bar_10, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(Power_ui.screen_bar_10, 0, 100);
    lv_bar_set_value(Power_ui.screen_bar_10, 100, LV_ANIM_OFF); // 初始不显示动画
    lv_obj_set_pos(Power_ui.screen_bar_10, 50, 145);
    lv_obj_set_size(Power_ui.screen_bar_10, 140, 70);
    lv_obj_add_flag(Power_ui.screen_bar_10, LV_OBJ_FLAG_HIDDEN);

    // 样式设置
    lv_obj_set_style_bg_opa(Power_ui.screen_bar_10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(Power_ui.screen_bar_10, lv_color_hex(0x3A3A3A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(Power_ui.screen_bar_10, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(Power_ui.screen_bar_10, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(Power_ui.screen_bar_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_opa(Power_ui.screen_bar_10, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(Power_ui.screen_bar_10, lv_color_hex(0x2FDA64), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(Power_ui.screen_bar_10, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(Power_ui.screen_bar_10, 10, LV_PART_INDICATOR | LV_STATE_DEFAULT);
}

/*******************************************************************************
 * 函数名称: Power_HideChargeUI
 * 函数功能: 隐藏充电UI元素
 * 参    数: 无
 * 返 回 值: 无
 * 说    明: 当USB拔出时立即隐藏充电图标和进度条
 *           使用三重机制确保UI完全隐藏：HIDDEN标志 + 透明度0 + 强制刷新
 * 修改记录: 2025.11.03, 创建函数
 ******************************************************************************/
void Power_HideChargeUI(void)
{
    if (Power_ui.top_layer != NULL)
    {
        // 方法1：设置HIDDEN标志
        lv_obj_add_flag(Power_ui.top_layer, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(Power_ui.screen_line_5, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(Power_ui.screen_line_6, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(Power_ui.screen_line_7, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(Power_ui.screen_bar_10, LV_OBJ_FLAG_HIDDEN);

        // 方法2：设置透明度为0（双重保险）
        lv_obj_set_style_opa(Power_ui.top_layer, 0, LV_PART_MAIN);
        lv_obj_set_style_opa(Power_ui.screen_line_5, 0, LV_PART_MAIN);
        lv_obj_set_style_opa(Power_ui.screen_line_6, 0, LV_PART_MAIN);
        lv_obj_set_style_opa(Power_ui.screen_line_7, 0, LV_PART_MAIN);
        lv_obj_set_style_opa(Power_ui.screen_bar_10, 0, LV_PART_MAIN);

        // 重置进度条值
        lv_bar_set_value(Power_ui.screen_bar_10, 0, LV_ANIM_OFF);

        // 方法3：强制刷新屏幕
        lv_obj_invalidate(lv_scr_act());
    }
}

/*******************************************************************************
 * 函数名称: Power_SetBatteryColor
 * 函数功能: 根据电池电量设置充电动画进度条颜色
 * 参    数: battery_percent - 电池电量百分比(0-100)
 * 返 回 值: 无
 * 说    明: 低于10%显示红色，10%-20%显示棕色，20%-100%显示绿色
 * 修改记录: 2025.11.03, 创建函数
 ******************************************************************************/
void Power_SetBatteryColor(uint8_t battery_percent)
{

    // 设置电池条的颜色
}

void Power_Movie(void)
{
    extern uint8_t System_FactoryTestFlag;
    if (System_FactoryTestFlag == 0xFF)
        return;
    if (PowesSystem.Status == STATRTCHARGE)
    {
        // 显示充电动画：移除隐藏标志并恢复透明度
        lv_obj_clear_flag(Power_ui.top_layer, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(Power_ui.screen_line_5, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(Power_ui.screen_line_6, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(Power_ui.screen_line_7, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(Power_ui.screen_bar_10, LV_OBJ_FLAG_HIDDEN);

        // 恢复透明度
        lv_obj_set_style_opa(Power_ui.top_layer, 255, LV_PART_MAIN);
        lv_obj_set_style_opa(Power_ui.screen_line_5, 255, LV_PART_MAIN);
        lv_obj_set_style_opa(Power_ui.screen_line_6, 255, LV_PART_MAIN);
        lv_obj_set_style_opa(Power_ui.screen_line_7, 255, LV_PART_MAIN);
        lv_obj_set_style_opa(Power_ui.screen_bar_10, 255, LV_PART_MAIN);

        // 确保置顶
        lv_obj_move_foreground(Power_ui.screen_line_5);
        lv_obj_move_foreground(Power_ui.screen_line_6);
        lv_obj_move_foreground(Power_ui.screen_line_7);
        lv_obj_move_foreground(Power_ui.screen_bar_10);

        // 根据当前电量设置进度条颜色
        uint8_t current_level = Power_GetBatteryLevel(PowesSystem.BatteryVoltage_mV);
        Power_SetBatteryColor(current_level);

        // 启动进度条动画
        lv_obj_set_style_anim_time(Power_ui.screen_bar_10, 1000, 0);
        lv_bar_set_value(Power_ui.screen_bar_10, 100, LV_ANIM_ON);

        // 切换到充电状态
        PowesSystem.Status = CHARGING;
        Powerr_MoveTimes = 130; // 1.3秒后隐藏进度条（10ms × 130）
    }
    else if (PowesSystem.Status == CHARGING)
    {
        // 充电期间保持充电图标显示（防止被其他UI操作隐藏）
        lv_obj_clear_flag(Power_ui.top_layer, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(Power_ui.screen_line_5, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(Power_ui.screen_line_6, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(Power_ui.screen_line_7, LV_OBJ_FLAG_HIDDEN);

        // 处理进度条动画倒计时
        if (Powerr_MoveTimes)
        {
            Powerr_MoveTimes--;
            if (Powerr_MoveTimes == 0)
            {
                // 1.3秒后隐藏进度条，充电图标继续显示
                lv_obj_add_flag(Power_ui.screen_bar_10, LV_OBJ_FLAG_HIDDEN);
                lv_bar_set_value(Power_ui.screen_bar_10, 0, LV_ANIM_OFF);
            }
        }
    }
    else
    {
        // 非充电状态，隐藏所有充电UI
        Power_HideChargeUI();
        Powerr_MoveTimes = 0;
    }
}

/**
 * @brief  更新所有屏幕的电池电量显示
 */
void Screens_PowerShow(void)
{
    static current_screen_t Cache_Screen;
    static uint8_t last_battery_level = 0xFF; // 上次的电池电量
    static uint8_t display_level = 0;         // 当前显示的电量（5%档位）

    bool screen_changed = (Cache_Screen != current_screen);
    Cache_Screen = current_screen;

    // 读取当前电池电量
    uint8_t current_level = Power_GetBatteryLevel(PowesSystem.BatteryVoltage_mV);

    // 计算5%档位（0, 5, 10, 15...100）
    uint8_t battery_5percent = (current_level / 5) * 5;
    uint8_t last_5percent = (last_battery_level / 5) * 5;
    lv_color_t color;

    // 只在跨越5%边界、首次更新或屏幕切换时更新显示
    if (battery_5percent != last_5percent || last_battery_level == 0xFF || screen_changed)
    {
        display_level = battery_5percent;
        last_battery_level = current_level;
    }
    else
    {
        // 电量未跨越5%边界，不更新显示，直接返回
        return;
    }

    // 使用5%档位的电量值进行显示
    current_level = display_level;

    if (current_level < 10)
    {
        // 低于10%：红色
        color = lv_color_hex(0xFF0000);
    }
    else if (current_level < 30)
    {
        // 10%-30%：棕色/橙色
        color = lv_color_hex(0xFF8000);
    }
    else if (current_level < 95)
    {
        color = lv_color_hex(0xFFFFFF);
    }
    else
    {
        // 95%-100%：绿色（原色）
        color = lv_color_hex(0x2FDA64);
    }
    switch (current_screen)
    {
    case SCREEN:
        lv_bar_set_value(guider_ui.screen_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_1:
        lv_bar_set_value(guider_ui.screen_1_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_1_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_2:
        lv_bar_set_value(guider_ui.screen_2_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_2_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_3:
        lv_bar_set_value(guider_ui.screen_3_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_3_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_4:
        lv_bar_set_value(guider_ui.screen_4_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_4_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_5:
        lv_bar_set_value(guider_ui.screen_5_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_5_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_6:
        lv_bar_set_value(guider_ui.screen_6_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_6_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_7:
        lv_bar_set_value(guider_ui.screen_7_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_7_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_8:
        lv_bar_set_value(guider_ui.screen_8_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_8_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_9:
        lv_bar_set_value(guider_ui.screen_9_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_9_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_10:
        lv_bar_set_value(guider_ui.screen_10_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_10_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_11:
        lv_bar_set_value(guider_ui.screen_11_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_11_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_12:
        lv_bar_set_value(guider_ui.screen_12_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_12_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_13:
        lv_bar_set_value(guider_ui.screen_13_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_13_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_14:
        lv_bar_set_value(guider_ui.screen_14_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_14_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_15:
        lv_bar_set_value(guider_ui.screen_15_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_15_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_16:
        lv_bar_set_value(guider_ui.screen_16_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_16_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_17:
        lv_bar_set_value(guider_ui.screen_17_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_17_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_18:
        lv_bar_set_value(guider_ui.screen_18_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_18_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_19:
        lv_bar_set_value(guider_ui.screen_19_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_19_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_20:
        lv_bar_set_value(guider_ui.screen_20_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_20_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_21:
        lv_bar_set_value(guider_ui.screen_21_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_21_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_22:
        lv_bar_set_value(guider_ui.screen_22_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_22_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_23:
        lv_bar_set_value(guider_ui.screen_23_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_23_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_24:
        lv_bar_set_value(guider_ui.screen_24_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_24_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_25:
        lv_bar_set_value(guider_ui.screen_25_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_25_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_26:
        lv_bar_set_value(guider_ui.screen_26_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_26_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_27:
        lv_bar_set_value(guider_ui.screen_27_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_27_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_28:
        lv_bar_set_value(guider_ui.screen_28_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_28_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_29:
        lv_bar_set_value(guider_ui.screen_29_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_29_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_30:
        lv_bar_set_value(guider_ui.screen_30_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_30_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_31:
        lv_bar_set_value(guider_ui.screen_31_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_31_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_32:
        lv_bar_set_value(guider_ui.screen_32_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_32_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_33:
        lv_bar_set_value(guider_ui.screen_33_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_33_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_34:
        lv_bar_set_value(guider_ui.screen_34_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_34_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_35:
        lv_bar_set_value(guider_ui.screen_35_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_35_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_36:
        lv_bar_set_value(guider_ui.screen_36_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_36_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_37:
        lv_bar_set_value(guider_ui.screen_37_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_37_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_38:
        lv_bar_set_value(guider_ui.screen_38_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_38_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_39:
        lv_bar_set_value(guider_ui.screen_39_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_39_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_40:
        lv_bar_set_value(guider_ui.screen_40_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_40_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_41:
        lv_bar_set_value(guider_ui.screen_41_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_41_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_42:
        lv_bar_set_value(guider_ui.screen_42_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_42_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_43:
        lv_bar_set_value(guider_ui.screen_43_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_43_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_44:
        lv_bar_set_value(guider_ui.screen_44_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_44_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_45:
        lv_bar_set_value(guider_ui.screen_45_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_45_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_46:
        lv_bar_set_value(guider_ui.screen_46_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_46_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_47:
        lv_bar_set_value(guider_ui.screen_47_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_47_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_48:
        lv_bar_set_value(guider_ui.screen_48_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_48_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_49:
        lv_bar_set_value(guider_ui.screen_49_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_49_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_50:
        lv_bar_set_value(guider_ui.screen_50_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_50_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_51:
        lv_bar_set_value(guider_ui.screen_51_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_51_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_52:
        lv_bar_set_value(guider_ui.screen_52_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_52_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_53:
        lv_bar_set_value(guider_ui.screen_53_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_53_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_54:
        lv_bar_set_value(guider_ui.screen_54_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_54_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_55:
        lv_bar_set_value(guider_ui.screen_55_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_55_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_56:
        lv_bar_set_value(guider_ui.screen_56_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_56_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_57:
        lv_bar_set_value(guider_ui.screen_57_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_57_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_58:
        lv_bar_set_value(guider_ui.screen_58_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_58_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_59:
        lv_bar_set_value(guider_ui.screen_59_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_59_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_60:
        lv_bar_set_value(guider_ui.screen_60_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_60_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_61:
        lv_bar_set_value(guider_ui.screen_61_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_61_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_62:
        lv_bar_set_value(guider_ui.screen_62_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_62_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_63:
        lv_bar_set_value(guider_ui.screen_63_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_63_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_64:
        lv_bar_set_value(guider_ui.screen_64_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_64_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_65:
        lv_bar_set_value(guider_ui.screen_65_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_65_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_66:
        lv_bar_set_value(guider_ui.screen_66_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_66_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_67:
        lv_bar_set_value(guider_ui.screen_67_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_67_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_68:
        lv_bar_set_value(guider_ui.screen_68_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_68_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_69:
        lv_bar_set_value(guider_ui.screen_69_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_69_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_70:
        lv_bar_set_value(guider_ui.screen_70_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_70_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_71:
        lv_bar_set_value(guider_ui.screen_71_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_71_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_72:
        lv_bar_set_value(guider_ui.screen_72_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_72_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_73:
        lv_bar_set_value(guider_ui.screen_73_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_73_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_74:
        lv_bar_set_value(guider_ui.screen_74_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_74_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_75:
        lv_bar_set_value(guider_ui.screen_75_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_75_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_76:
        lv_bar_set_value(guider_ui.screen_76_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_76_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_77:
        lv_bar_set_value(guider_ui.screen_77_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_77_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_78:
        lv_bar_set_value(guider_ui.screen_78_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_78_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_79:
        lv_bar_set_value(guider_ui.screen_79_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_79_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_80:
        lv_bar_set_value(guider_ui.screen_80_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_80_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    case SCREEN_81:
        lv_bar_set_value(guider_ui.screen_81_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.screen_81_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    #ifdef LAB_TEST
    case SCREEN_LAB_TEST:
        lv_bar_set_value(guider_ui.lab_test_Batter_Percent, current_level, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(guider_ui.lab_test_Batter_Percent, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        break;
    #endif
    default:
        break;
    }
}

void Screens_RFModeShow(void)
{
    static current_screen_t lastCache_curcent = SCREEN_NUll;
    extern U_SAVEMSG g_MsgSave;
    RfMode = g_MsgSave.RadioMode;
    if (lastCache_curcent == current_screen)
        return;
    if (RfMode == RM_FSK)
        strcpy(RfMode_buffer, "FSK");
    else if (RfMode == RM_OOK)
        strcpy(RfMode_buffer, "OOK");
    switch (current_screen)
    {
    case SCREEN:
        lv_label_set_text(guider_ui.screen_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_1:
        lv_label_set_text(guider_ui.screen_1_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_2:
        lv_label_set_text(guider_ui.screen_2_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_3:
        lv_label_set_text(guider_ui.screen_3_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_4:
        lv_label_set_text(guider_ui.screen_4_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_5:
        lv_label_set_text(guider_ui.screen_5_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_6:
        lv_label_set_text(guider_ui.screen_6_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_7:
        lv_label_set_text(guider_ui.screen_7_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_8:
        lv_label_set_text(guider_ui.screen_8_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_9:
        lv_label_set_text(guider_ui.screen_9_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_10:
        lv_label_set_text(guider_ui.screen_10_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_11:
        lv_label_set_text(guider_ui.screen_11_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_12:
        lv_label_set_text(guider_ui.screen_12_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_13:
        lv_label_set_text(guider_ui.screen_13_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_14:
        lv_label_set_text(guider_ui.screen_14_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_15:
        lv_label_set_text(guider_ui.screen_15_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_16:
        lv_label_set_text(guider_ui.screen_16_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_17:
        lv_label_set_text(guider_ui.screen_17_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_18:
        lv_label_set_text(guider_ui.screen_18_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_19:
        lv_label_set_text(guider_ui.screen_19_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_20:
        lv_label_set_text(guider_ui.screen_20_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_21:
        lv_label_set_text(guider_ui.screen_21_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_22:
        lv_label_set_text(guider_ui.screen_22_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_23:
        lv_label_set_text(guider_ui.screen_23_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_24:
        lv_label_set_text(guider_ui.screen_24_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_25:
        lv_label_set_text(guider_ui.screen_25_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_26:
        lv_label_set_text(guider_ui.screen_26_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_27:
        lv_label_set_text(guider_ui.screen_27_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_28:
        lv_label_set_text(guider_ui.screen_28_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_29:
        lv_label_set_text(guider_ui.screen_29_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_30:
        lv_label_set_text(guider_ui.screen_30_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_31:
        lv_label_set_text(guider_ui.screen_31_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_32:
        lv_label_set_text(guider_ui.screen_32_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_33:
        lv_label_set_text(guider_ui.screen_33_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_34:
        lv_label_set_text(guider_ui.screen_34_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_35:
        lv_label_set_text(guider_ui.screen_35_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_36:
        lv_label_set_text(guider_ui.screen_36_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_37:
        lv_label_set_text(guider_ui.screen_37_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_38:
        lv_label_set_text(guider_ui.screen_38_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_39:
        lv_label_set_text(guider_ui.screen_39_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_40:
        lv_label_set_text(guider_ui.screen_40_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_41:
        lv_label_set_text(guider_ui.screen_41_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_42:
        lv_label_set_text(guider_ui.screen_42_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_43:
        lv_label_set_text(guider_ui.screen_43_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_44:
        lv_label_set_text(guider_ui.screen_44_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_45:
        lv_label_set_text(guider_ui.screen_45_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_46:
        lv_label_set_text(guider_ui.screen_46_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_47:
        lv_label_set_text(guider_ui.screen_47_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_48:
        lv_label_set_text(guider_ui.screen_48_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_49:
        lv_label_set_text(guider_ui.screen_49_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_50:
        lv_label_set_text(guider_ui.screen_50_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_51:
        lv_label_set_text(guider_ui.screen_51_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_52:
        lv_label_set_text(guider_ui.screen_52_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_53:
        lv_label_set_text(guider_ui.screen_53_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_54:
        lv_label_set_text(guider_ui.screen_54_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_55:
        lv_label_set_text(guider_ui.screen_55_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_56:
        lv_label_set_text(guider_ui.screen_56_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_57:
        lv_label_set_text(guider_ui.screen_57_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_58:
        lv_label_set_text(guider_ui.screen_58_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_59:
        lv_label_set_text(guider_ui.screen_59_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_60:
        lv_label_set_text(guider_ui.screen_60_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_61:
        lv_label_set_text(guider_ui.screen_61_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_62:
        lv_label_set_text(guider_ui.screen_62_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_63:
        lv_label_set_text(guider_ui.screen_63_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_64:
        lv_label_set_text(guider_ui.screen_64_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_65:
        lv_label_set_text(guider_ui.screen_65_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_66:
        lv_label_set_text(guider_ui.screen_66_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_67:
        lv_label_set_text(guider_ui.screen_67_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_68:
        lv_label_set_text(guider_ui.screen_68_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_69:
        lv_label_set_text(guider_ui.screen_69_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_70:
        lv_label_set_text(guider_ui.screen_70_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_71:
        lv_label_set_text(guider_ui.screen_71_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_72:
        lv_label_set_text(guider_ui.screen_72_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_73:
        lv_label_set_text(guider_ui.screen_73_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_74:
        lv_label_set_text(guider_ui.screen_74_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_75:
        lv_label_set_text(guider_ui.screen_75_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_76:
        lv_label_set_text(guider_ui.screen_76_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_77:
        lv_label_set_text(guider_ui.screen_77_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_78:
        lv_label_set_text(guider_ui.screen_78_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_79:
        lv_label_set_text(guider_ui.screen_79_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_80:
        lv_label_set_text(guider_ui.screen_80_RF_MODE, RfMode_buffer);
        break;
    case SCREEN_81:
        lv_label_set_text(guider_ui.screen_81_RF_MODE, RfMode_buffer);
        break;
    #ifdef LAB_TEST
    case SCREEN_LAB_TEST:
        lv_label_set_text(guider_ui.lab_test_RF_MODE, RfMode_buffer);
        break;
    #endif
    default:
        break;
    }
    lastCache_curcent = current_screen;
}
