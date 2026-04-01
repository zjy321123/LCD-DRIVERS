/**
 * @file    mygui.c
 * @brief   GUI LVGL驱动测试程序
 * @author  zjy
 * @date    2025.04.16
 */
#include "mygui.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include <stdlib.h>

static lv_obj_t *blocks[9];
static lv_obj_t *labels[9];
static lv_color_t color_palette[15];
static int color_initialized = 0;
static lv_obj_t *fps_test_obj = NULL;

static void init_color_palette(void)
{
    if (color_initialized)
        return;
    color_palette[0] = lv_color_hex(0xFF0000);
    color_palette[1] = lv_color_hex(0x00FF00);
    color_palette[2] = lv_color_hex(0x0000FF);
    color_palette[3] = lv_color_hex(0xFFFF00);
    color_palette[4] = lv_color_hex(0xFF00FF);
    color_palette[5] = lv_color_hex(0x00FFFF);
    color_palette[6] = lv_color_hex(0xFF8000);
    color_palette[7] = lv_color_hex(0x8000FF);
    color_palette[8] = lv_color_hex(0x0080FF);
    color_palette[9] = lv_color_hex(0xFF0080);
    color_palette[10] = lv_color_hex(0x80FF00);
    color_palette[11] = lv_color_hex(0x0040FF);
    color_palette[12] = lv_color_hex(0xFFFFFF);
    color_palette[13] = lv_color_hex(0x808080);
    color_palette[14] = lv_color_hex(0x004080);
    color_initialized = 1;
}

static void random_update_cb(lv_timer_t *timer)
{
    for (int i = 0; i < 9; i++)
    {
        lv_color_t new_color = color_palette[rand() % 15];
        lv_obj_set_style_bg_color(blocks[i], new_color, 0);

        int new_num = (rand() % 9) + 1;
        lv_label_set_text_fmt(labels[i], "%d", new_num);
    }
}

static void fps_refresh_cb(lv_timer_t *timer)
{
    static uint32_t color_idx = 0;
    lv_color_t colors[4] = {
        lv_color_hex(0xFF0000),
        lv_color_hex(0x00FF00),
        lv_color_hex(0x0000FF),
        lv_color_hex(0xFFFF00)};

    lv_obj_set_style_bg_color(fps_test_obj, colors[color_idx % 4], 0);
    color_idx++;
}

void lv_fps_test_start(void)
{
    lv_obj_clean(lv_scr_act());

    fps_test_obj = lv_obj_create(lv_scr_act());
    lv_obj_set_size(fps_test_obj, 240, 320);
    lv_obj_set_pos(fps_test_obj, 0, 0);
    lv_obj_set_style_bg_color(fps_test_obj, lv_color_hex(0xFF0000), 0);
    lv_obj_set_style_bg_opa(fps_test_obj, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(fps_test_obj, 0, 0);
    lv_obj_set_style_radius(fps_test_obj, 0, 0);
    lv_obj_set_style_pad_all(fps_test_obj, 0, 0);

    lv_timer_create(fps_refresh_cb, 1, NULL);
}

void lv_ex_label(void)
{
    init_color_palette();

    int block_w = 80;
    int block_h = 107;

    for (int i = 0; i < 9; i++)
    {
        int row = i / 3;
        int col = i % 3;
        int x = col * block_w;
        int y = row * block_h;

        blocks[i] = lv_obj_create(lv_scr_act());
        lv_obj_set_size(blocks[i], block_w, block_h);
        lv_obj_set_pos(blocks[i], x, y);
        lv_obj_set_style_bg_color(blocks[i], color_palette[i], 0);
        lv_obj_set_style_bg_opa(blocks[i], LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(blocks[i], 0, 0);
        lv_obj_set_style_radius(blocks[i], 0, 0);
        lv_obj_set_style_pad_all(blocks[i], 0, 0);

        labels[i] = lv_label_create(blocks[i]);
        lv_label_set_text_fmt(labels[i], "%d", i + 1);
        lv_obj_set_style_text_color(labels[i], lv_color_hex(0xFFFFFF), 0);
        lv_obj_set_style_text_font(labels[i], &lv_font_montserrat_24, 0);
        lv_obj_center(labels[i]);
    }

    lv_timer_create(random_update_cb, 1000, NULL);
}
