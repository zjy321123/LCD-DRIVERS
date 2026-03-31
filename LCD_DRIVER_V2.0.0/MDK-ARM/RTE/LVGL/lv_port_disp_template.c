/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp_template.h"
#include <stdbool.h>
#include "spi.h"
#include "gpio.h"
#include "lcd.h"
/*********************
 *      DEFINES
 *********************/
#ifndef MY_DISP_HOR_RES
// #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
#define MY_DISP_HOR_RES 240
#endif

#ifndef MY_DISP_VER_RES
// #warning Please define or replace the macro MY_DISP_VER_RES with the actual screen height, default value 240 is used for now.
#define MY_DISP_VER_RES 320
#endif

#define BYTE_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565)) /*will be 2 for RGB565 */

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_display_t *disp_drv = NULL;
static volatile int frame_complete_flag = 0;
static volatile uint8_t dma_busy = 0;
static lv_area_t pending_area;
static uint8_t *pending_px_map = NULL;

int lv_port_get_frame_flag(void)
{
    return frame_complete_flag;
}

void lv_port_clear_frame_flag(void)
{
    frame_complete_flag = 0;
}

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*------------------------------------
     * Create a display and set a flush_cb
     * -----------------------------------*/
    lv_display_t *disp = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
    disp_drv = disp;
    lv_display_set_flush_cb(disp, disp_flush);

    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buf_1[MY_DISP_HOR_RES * 20 * BYTE_PER_PIXEL];
    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buf_2[MY_DISP_HOR_RES * 20 * BYTE_PER_PIXEL];
    lv_display_set_buffers(disp, buf_1, buf_2, sizeof(buf_1), LV_DISPLAY_RENDER_MODE_PARTIAL);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/*Flush the content of the internal buffer the specific area on the display.
 *`px_map` contains the rendered image as raw pixel map and it should be copied to `area` on the display.
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_display_flush_ready()' has to be called when it's finished.*/
extern SPI_HandleTypeDef hspi1;

static void start_transfer(const lv_area_t *area, uint8_t *px_map)
{
    uint32_t size = lv_area_get_size(area);

    LCD_CS_CLR();

    uint8_t addr_buf[15];
    uint8_t *p = addr_buf;

    *p++ = 0x2A;
    *p++ = (area->x1 >> 8) & 0xFF;
    *p++ = area->x1 & 0xFF;
    *p++ = (area->x2 >> 8) & 0xFF;
    *p++ = area->x2 & 0xFF;
    *p++ = 0x2B;
    *p++ = (area->y1 >> 8) & 0xFF;
    *p++ = area->y1 & 0xFF;
    *p++ = (area->y2 >> 8) & 0xFF;
    *p++ = area->y2 & 0xFF;
    *p++ = 0x2C;

    LCD_DC_CLR();
    HAL_SPI_Transmit(&hspi1, addr_buf, 1, 100);
    LCD_DC_SET();
    HAL_SPI_Transmit(&hspi1, addr_buf + 1, 4, 100);
    LCD_DC_CLR();
    HAL_SPI_Transmit(&hspi1, addr_buf + 5, 1, 100);
    LCD_DC_SET();
    HAL_SPI_Transmit(&hspi1, addr_buf + 6, 4, 100);
    LCD_DC_CLR();
    HAL_SPI_Transmit(&hspi1, addr_buf + 10, 1, 100);
    LCD_DC_SET();

    dma_busy = 1;
    HAL_SPI_Transmit_DMA(&hspi1, px_map, size * 2);
}

static void disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    if (dma_busy)
    {
        pending_area = *area;
        pending_px_map = px_map;
        return;
    }

    start_transfer(area, px_map);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi == &hspi1)
    {
        LCD_CS_SET();

        if (lv_display_flush_is_last(disp_drv))
        {
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
            frame_complete_flag = 1;
        }

        if (pending_px_map)
        {
            start_transfer(&pending_area, pending_px_map);
            pending_px_map = NULL;
            lv_display_flush_ready(disp_drv);
        }
        else
        {
            dma_busy = 0;
            lv_display_flush_ready(disp_drv);
        }
    }
}
#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
