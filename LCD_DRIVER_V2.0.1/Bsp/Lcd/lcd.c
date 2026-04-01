/**
 * @file    lcd.c
 * @brief   LCD驱动程序
 * @author  zjy
 * @date    2025.04.16
 */

#include <string.h>
#include <stdint.h>
#include "lcd.h"
#include "spi.h"
#include "gpio.h"

/*******************************************************************************
 *                              LCD 参数配置
 ******************************************************************************/
#define LCD_WIDTH 240
#define LCD_HEIGHT 320 // 无视即可，LVGL参数区配置

/*******************************************************************************
 *                              驱动接口层
 ******************************************************************************/
extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_memtomem_dma1_channel3;

/**
 * @brief   SPI阻塞式数据发送
 * @author  zjy
 * @date    2025.04.16
 * @param   data: 待发送的数据指针
 * @param   size: 待发送的数据长度
 * @retval  无
 */
static inline void LCD_SPI_Transmit(uint8_t *data, uint16_t size)
{
    HAL_SPI_Transmit(&hspi1, data, size, 1000);
}

#if LCD_USE_SPI_DMA
/**
 * @brief   SPI DMA方式数据发送
 * @author  zjy
 * @date    2025.04.16
 * @param   data: 待发送的数据指针
 * @param   size: 待发送的数据长度
 * @retval  无
 */
static inline void LCD_SPI_Transmit_DMA(uint8_t *data, uint16_t size)
{
    HAL_SPI_Transmit_DMA(&hspi1, data, size);
}
#endif

/*******************************************************************************
 *                              基础函数
 ******************************************************************************/

/**
 * @brief   LCD写数据函数
 * @author  zjy
 * @date    2025.04.16
 * @param   Byte: 待发送的数据指针
 * @param   Size: 待发送的数据长度
 * @retval  无
 */
void LCD_WR_DATAS(uint8_t *Byte, uint16_t Size)
{
    LCD_DC_SET();
    LCD_CS_CLR();
#if LCD_USE_SPI_DMA
    LCD_SPI_Transmit_DMA(Byte, Size);
#else
    LCD_SPI_Transmit(Byte, Size);
#endif
    LCD_CS_SET();
}

/**
 * @brief   LCD写命令函数
 * @author  zjy
 * @date    2025.04.16
 * @param   data: 待写入的命令字节
 * @retval  无
 */
static void LCD_WR_CMD(uint8_t data)
{
    LCD_DC_CLR();
    LCD_CS_CLR();
    LCD_SPI_Transmit(&data, 1);
    LCD_CS_SET();
    LCD_DC_SET();
}
/**
 * @brief   LCD写单字节数据函数
 * @author  zjy
 * @date    2025.04.16
 * @param   data: 待写入的数据字节
 * @retval  无
 */
static void LCD_WR_DATA(uint8_t data)
{
    LCD_DC_SET();
    LCD_CS_CLR();
    LCD_SPI_Transmit(&data, 1);
    LCD_CS_SET();
}

/**
 * @brief   LCD写16位数据函数
 * @author  zjy
 * @date    2025.04.17
 * @param   data: 待写入的16位数据
 * @retval  无
 */
void LCD_WR_UINT16_DATA(uint16_t data)
{
    uint8_t DATA8[2];
    DATA8[0] = data >> 8;
    DATA8[1] = data;
    LCD_WR_DATAS(DATA8, 2);
}

/**
 * @brief   LCD初始化函数
 * @author  zjy
 * @date    2025.04.16
 * @param   无
 * @retval  无
 */
void Lcd_Init(void)
{
    LCD_RES_SET();
    HAL_Delay(120);
    LCD_RES_CLR();
    HAL_Delay(100);
    LCD_RES_SET();
    HAL_Delay(120);

    LCD_WR_CMD(0x00);
    HAL_Delay(10);
    LCD_WR_CMD(0x11);
    HAL_Delay(120);

    LCD_WR_CMD(0x36);
    LCD_WR_DATA(0x00);
    LCD_WR_CMD(0x3a);
    LCD_WR_DATA(0x05);

    LCD_WR_CMD(0xb2);
    LCD_WR_DATA(0x0c);
    LCD_WR_DATA(0x0c);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x33);
    LCD_WR_DATA(0x33);
    LCD_WR_CMD(0xb7);
    LCD_WR_DATA(0x35);

    LCD_WR_CMD(0xbb);
    LCD_WR_DATA(0x36);
    LCD_WR_CMD(0xc0);
    LCD_WR_DATA(0x2c);
    LCD_WR_CMD(0xc2);
    LCD_WR_DATA(0x01);
    LCD_WR_CMD(0xc3);
    LCD_WR_DATA(0x12);
    LCD_WR_CMD(0xc4);
    LCD_WR_DATA(0x20);
    LCD_WR_CMD(0xc6);
    LCD_WR_DATA(0x0f);
    LCD_WR_CMD(0xd0);
    LCD_WR_DATA(0xa4);
    LCD_WR_DATA(0xa1);

    LCD_WR_CMD(0xe0);
    LCD_WR_DATA(0xd0);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x05);
    LCD_WR_DATA(0x0e);
    LCD_WR_DATA(0x15);
    LCD_WR_DATA(0x0d);
    LCD_WR_DATA(0x37);
    LCD_WR_DATA(0x43);
    LCD_WR_DATA(0x47);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x15);
    LCD_WR_DATA(0x12);
    LCD_WR_DATA(0x16);
    LCD_WR_DATA(0x19);

    LCD_WR_CMD(0xe1);
    LCD_WR_DATA(0xd0);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x05);
    LCD_WR_DATA(0x0d);
    LCD_WR_DATA(0x0c);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x2d);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x40);
    LCD_WR_DATA(0x0e);
    LCD_WR_DATA(0x1c);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x16);
    LCD_WR_DATA(0x19);

    LCD_WR_CMD(0x29);
    HAL_Delay(120);
    LCD_WR_CMD(0x2c);
    HAL_Delay(120);
}

/*******************************************************************************
 *                              休眠控制
 ******************************************************************************/

/**
 * @brief   LCD进入休眠模式
 * @author  zjy
 * @date    2025.04.17
 * @param   无
 * @retval  无
 */
void Lcd_EnterSleep(void)
{
    LCD_WR_CMD(0xfe);
    LCD_WR_CMD(0xef);
    LCD_WR_CMD(0x28);
    HAL_Delay(120);
    LCD_WR_CMD(0x10);
    HAL_Delay(150);
}

/**
 * @brief   LCD退出休眠模式
 * @author  zjy
 * @date    2025.04.17
 * @param   无
 * @retval  无
 */
void Lcd_ExitSleep(void)
{
    LCD_WR_CMD(0x00);
    LCD_WR_CMD(0xfe);
    LCD_WR_CMD(0xef);
    LCD_WR_CMD(0x11);
    HAL_Delay(120);
    LCD_WR_CMD(0x29);
}
/**
 * @brief   LCD显示开启，打开背光
 * @author  zjy
 * @date    2025.04.16
 * @param   无
 * @retval  无
 */
void LCD_ON(void)
{
    LCD_BL_SET();
    Lcd_ExitSleep();
}
/**
 * @brief   LCD显示关闭，关闭背光
 * @author  zjy
 * @date    2025.04.16
 * @param   无
 * @retval  无
 */
void LCD_OFF(void)
{
    LCD_BL_CLR();
    Lcd_EnterSleep();
}
/*******************************************************************************
 *                              地址设置
 ******************************************************************************/

/**
 * @brief   LCD显示区域设置
 * @author  zjy
 * @date    2025.04.16
 * @param   x1: 起始X坐标
 * @param   y1: 起始Y坐标
 * @param   x2: 结束X坐标
 * @param   y2: 结束Y坐标
 * @retval  无
 */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_WR_CMD(0x2a);
    LCD_WR_UINT16_DATA(x1);
    LCD_WR_UINT16_DATA(x2);
    LCD_WR_CMD(0x2b);
    LCD_WR_UINT16_DATA(y1);
    LCD_WR_UINT16_DATA(y2);
    LCD_WR_CMD(0x2c);
}

/**
 * @brief   LCD显示区域设置(快速版本)
 * @author  zjy
 * @date    2025.04.16
 * @param   x1: 起始X坐标
 * @param   y1: 起始Y坐标
 * @param   x2: 结束X坐标
 * @param   y2: 结束Y坐标
 * @retval  无
 */
static void LCD_Address_Set_Fast(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint8_t buf[4];

    LCD_CS_CLR();

    LCD_DC_CLR();
    buf[0] = 0x2a;
    LCD_SPI_Transmit(buf, 1);

    LCD_DC_SET();
    buf[0] = x1 >> 8;
    buf[1] = x1 & 0xFF;
    buf[2] = x2 >> 8;
    buf[3] = x2 & 0xFF;
    LCD_SPI_Transmit(buf, 4);

    LCD_DC_CLR();
    buf[0] = 0x2b;
    LCD_SPI_Transmit(buf, 1);

    LCD_DC_SET();
    buf[0] = y1 >> 8;
    buf[1] = y1 & 0xFF;
    buf[2] = y2 >> 8;
    buf[3] = y2 & 0xFF;
    LCD_SPI_Transmit(buf, 4);

    LCD_DC_CLR();
    buf[0] = 0x2c;
    LCD_SPI_Transmit(buf, 1);

    LCD_CS_SET();
}

/*******************************************************************************
 *                              兼容接口
 ******************************************************************************/

/**
 * @brief   SPI写字节函数(阻塞方式)
 * @author  zjy
 * @date    2025.04.16
 * @param   Byte: 待发送的数据指针
 * @param   Size: 待发送的数据长度
 * @retval  无
 */
void SPI_Write_Byte(uint8_t *Byte, uint16_t Size)
{
    LCD_SPI_Transmit(Byte, Size);
}

/**
 * @brief   SPI写字节函数(DMA方式)
 * @author  zjy
 * @date    2025.04.16
 * @param   Byte: 待发送的数据指针
 * @param   Size: 待发送的数据长度
 * @retval  无
 */
void SPI_DMA_Write_Byte(uint8_t *Byte, uint16_t Size)
{
#if LCD_USE_SPI_DMA
    LCD_SPI_Transmit_DMA(Byte, Size);
#else
    LCD_SPI_Transmit(Byte, Size);
#endif
}

/*******************************************************************************
 *                              填充函数
 ******************************************************************************/

/**
 * @brief   LCD填充指定颜色
 * @author  zjy
 * @date    2025.03.30
 * @param   color: 要填充的颜色(RGB565格式)
 * @retval  无
 */
void LCD_Fill(uint16_t color)
{
    uint32_t i;
    uint32_t total_pixels = LCD_WIDTH * LCD_HEIGHT;

    LCD_Address_Set(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

    for (i = 0; i < total_pixels; i++)
    {
        LCD_WR_UINT16_DATA(color);
    }
}

/**
 * @brief   LCD全屏显示红色
 * @author  zjy
 * @date    2025.03.30
 * @param   无
 * @retval  无
 */
void LCD_Fill_Red(void)
{
    LCD_Fill(0xF800);
}

/**
 * @brief   LCD全屏显示蓝色
 * @author  zjy
 * @date    2025.03.30
 * @param   无
 * @retval  无
 */
void LCD_Fill_Blue(void)
{
    LCD_Fill(0x001F);
}

/*******************************************************************************
 *                              刷新率测试 - 非阻塞回调模式
 ******************************************************************************/
#if LCD_REFRESH_RATE_TEST

#define ROW_BYTES (LCD_WIDTH * 2)
#define CHUNK_ROWS 64
#define CHUNK_BYTES (ROW_BYTES * CHUNK_ROWS)
#define TOTAL_CHUNKS (LCD_HEIGHT / CHUNK_ROWS)

typedef enum
{
    TEST_STATE_IDLE,
    TEST_STATE_INIT_MEM,
    TEST_STATE_READY,
    TEST_STATE_TRANSFER,
    TEST_STATE_COMPLETE
} TestState_t;

static uint8_t red_chunk_buf[CHUNK_BYTES];
static uint8_t blue_chunk_buf[CHUNK_BYTES];
static uint8_t red_row_template[ROW_BYTES];
static uint8_t blue_row_template[ROW_BYTES];

static volatile TestState_t test_state = TEST_STATE_IDLE;
static volatile uint8_t is_red_frame = 1;
static LCD_Callback_t test_frame_done_cb = NULL;

#if LCD_USE_MEM_DMA
static volatile uint16_t mem_dma_fill_row = 0;

/**
 * @brief   DMA内存搬运完成回调函数
 * @author  zjy
 * @date    2025.03.30
 * @param   hdma: DMA句柄指针
 * @retval  无
 */
static void LCD_Mem_DMA_Init_Callback(DMA_HandleTypeDef *hdma)
{
    mem_dma_fill_row++;

    if (mem_dma_fill_row < CHUNK_ROWS)
    {
        HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel3,
                         (uint32_t)red_row_template,
                         (uint32_t)&red_chunk_buf[mem_dma_fill_row * ROW_BYTES],
                         ROW_BYTES);
    }
    else if (mem_dma_fill_row < CHUNK_ROWS * 2)
    {
        HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel3,
                         (uint32_t)blue_row_template,
                         (uint32_t)&blue_chunk_buf[(mem_dma_fill_row - CHUNK_ROWS) * ROW_BYTES],
                         ROW_BYTES);
    }
    else
    {
        test_state = TEST_STATE_READY;
    }
}
#endif

#if LCD_USE_SPI_DMA
static volatile uint16_t spi_dma_chunk_sent = 0;
static volatile uint16_t spi_dma_chunk_done = 0;
static uint8_t *spi_dma_current_buf = NULL;

// /**
//  * @brief   SPI DMA发送完成回调函数
//  * @author  zjy
//  * @date    2025.03.30
//  * @param   hspi: SPI句柄指针
//  * @retval  无
//  */
// void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
// {
//     if (hspi == &hspi1)
//     {
//         spi_dma_chunk_done++;
//         if (spi_dma_chunk_done >= TOTAL_CHUNKS)
//         {
//             test_state = TEST_STATE_COMPLETE;
//         }
//     }
// }
#endif

/**
 * @brief   刷新率测试初始化函数
 * @author  zjy
 * @date    2025.03.30
 * @param   callback: 帧传输完成回调函数指针
 * @retval  无
 */
void LCD_RefreshRate_Test_Init(LCD_Callback_t callback)
{
    test_frame_done_cb = callback;
    test_state = TEST_STATE_INIT_MEM;

#if LCD_USE_MEM_DMA
    mem_dma_fill_row = 0;

    for (uint16_t i = 0; i < LCD_WIDTH; i++)
    {
        red_row_template[i * 2] = 0xF8;
        red_row_template[i * 2 + 1] = 0x00;
        blue_row_template[i * 2] = 0x00;
        blue_row_template[i * 2 + 1] = 0x1F;
    }

    HAL_DMA_RegisterCallback(&hdma_memtomem_dma1_channel3,
                             HAL_DMA_XFER_CPLT_CB_ID,
                             LCD_Mem_DMA_Init_Callback);

    HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel3,
                     (uint32_t)red_row_template,
                     (uint32_t)red_chunk_buf,
                     ROW_BYTES);
#else
    uint8_t *ptr;

    ptr = red_chunk_buf;
    for (uint32_t i = 0; i < LCD_WIDTH * CHUNK_ROWS; i++)
    {
        *ptr++ = 0xF8;
        *ptr++ = 0x00;
    }

    ptr = blue_chunk_buf;
    for (uint32_t i = 0; i < LCD_WIDTH * CHUNK_ROWS; i++)
    {
        *ptr++ = 0x00;
        *ptr++ = 0x1F;
    }

    test_state = TEST_STATE_READY;
#endif
}

/**
 * @brief   刷新率测试启动函数
 * @author  zjy
 * @date    2025.03.30
 * @param   无
 * @retval  无
 */
void LCD_RefreshRate_Test_Start(void)
{
    uint8_t *current_buf;

    if (test_state != TEST_STATE_READY)
        return;

    current_buf = is_red_frame ? red_chunk_buf : blue_chunk_buf;

    LCD_Address_Set_Fast(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

    LCD_DC_SET();
    LCD_CS_CLR();

    test_state = TEST_STATE_TRANSFER;

#if LCD_USE_SPI_DMA
    spi_dma_chunk_sent = 0;
    spi_dma_chunk_done = 0;
    spi_dma_current_buf = current_buf;
    spi_dma_chunk_sent++;
    HAL_SPI_Transmit_DMA(&hspi1, current_buf, CHUNK_BYTES);
#else
    for (uint16_t chunk = 0; chunk < TOTAL_CHUNKS; chunk++)
    {
        LCD_SPI_Transmit(current_buf, CHUNK_BYTES);
    }
    test_state = TEST_STATE_COMPLETE;
#endif
}

/**
 * @brief   刷新率测试状态处理函数
 * @author  zjy
 * @date    2025.03.30
 * @param   无
 * @retval  无
 */
void LCD_RefreshRate_Test_Process(void)
{
#if LCD_USE_SPI_DMA
    if (test_state == TEST_STATE_TRANSFER)
    {
        if (spi_dma_chunk_sent < TOTAL_CHUNKS &&
            spi_dma_chunk_done >= spi_dma_chunk_sent &&
            HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_READY)
        {
            spi_dma_chunk_sent++;
            HAL_SPI_Transmit_DMA(&hspi1, spi_dma_current_buf, CHUNK_BYTES);
        }
    }
    else
#endif
        if (test_state == TEST_STATE_COMPLETE)
    {
        LCD_CS_SET();

        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
        is_red_frame = !is_red_frame;

        test_state = TEST_STATE_READY;

        if (test_frame_done_cb)
        {
            test_frame_done_cb();
        }
    }
}

/**
 * @brief   获取刷新率测试状态
 * @author  zjy
 * @date    2025.03.30
 * @param   无
 * @retval  LCD状态枚举值
 */
LCD_State_t LCD_RefreshRate_Test_GetState(void)
{
    switch (test_state)
    {
    case TEST_STATE_IDLE:
    case TEST_STATE_INIT_MEM:
        return LCD_STATE_IDLE;
    case TEST_STATE_TRANSFER:
        return LCD_STATE_BUSY;
    case TEST_STATE_READY:
        return LCD_STATE_READY;
    default:
        return LCD_STATE_ERROR;
    }
}

/**
 * @brief   刷新率测试运行函数(主循环调用)
 * @author  zjy
 * @date    2025.03.30
 * @param   无
 * @retval  无
 */
void LCD_RefreshRate_Test_Run(void)
{
    switch (test_state)
    {
    case TEST_STATE_IDLE:
        LCD_RefreshRate_Test_Init(NULL);
        break;

    case TEST_STATE_INIT_MEM:
        break;

    case TEST_STATE_READY:
        LCD_RefreshRate_Test_Start();
        break;

    case TEST_STATE_TRANSFER:
    case TEST_STATE_COMPLETE:
        LCD_RefreshRate_Test_Process();
        break;
    }
}

#endif
