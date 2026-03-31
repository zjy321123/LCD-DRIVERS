#ifndef __LCD_H
#define __LCD_H

#include <stdint.h>

/*******************************************************************************
 *                              功能配置
 ******************************************************************************/
#define LCD_USE_SPI_DMA 1
#define LCD_USE_MEM_DMA 1
#define LCD_REFRESH_RATE_TEST 0 // 帧数测试

/*******************************************************************************
 *                              GPIO 宏定义
 ******************************************************************************/
#define LCD_RES_SET() HAL_GPIO_WritePin(LCD_RESET_Pin_GPIO_Port, LCD_RESET_Pin_Pin, GPIO_PIN_SET)
#define LCD_RES_CLR() HAL_GPIO_WritePin(LCD_RESET_Pin_GPIO_Port, LCD_RESET_Pin_Pin, GPIO_PIN_RESET)
#define LCD_CS_CLR() HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define LCD_CS_SET() HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)
#define LCD_DC_CLR() HAL_GPIO_WritePin(LCD_DC_Pin_GPIO_Port, LCD_DC_Pin_Pin, GPIO_PIN_RESET)
#define LCD_DC_SET() HAL_GPIO_WritePin(LCD_DC_Pin_GPIO_Port, LCD_DC_Pin_Pin, GPIO_PIN_SET)

#define LCD_BL_SET() HAL_GPIO_WritePin(LCD_BACKLIGHT_Port, LCD_BACKLIGHT_PIN, GPIO_PIN_SET)
#define LCD_BL_CLR() HAL_GPIO_WritePin(LCD_BACKLIGHT_Port, LCD_BACKLIGHT_PIN, GPIO_PIN_RESET)

/*******************************************************************************
 *                              类型定义
 ******************************************************************************/
typedef enum
{
    LCD_STATE_IDLE,
    LCD_STATE_BUSY,
    LCD_STATE_READY,
    LCD_STATE_ERROR
} LCD_State_t;

typedef void (*LCD_Callback_t)(void);

/*******************************************************************************
 *                              函数声明
 ******************************************************************************/
void Lcd_Init(void);
void Lcd_EnterSleep(void);
void Lcd_ExitSleep(void);

void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_WR_UINT16_DATA(uint16_t data);

void SPI_Write_Byte(uint8_t *Byte, uint16_t Size);
void SPI_DMA_Write_Byte(uint8_t *Byte, uint16_t Size);

void LCD_Fill(uint16_t color);
void LCD_Fill_Red(void);
void LCD_Fill_Blue(void);

#if LCD_REFRESH_RATE_TEST
void LCD_RefreshRate_Test_Init(LCD_Callback_t callback);
void LCD_RefreshRate_Test_Start(void);
void LCD_RefreshRate_Test_Process(void);
LCD_State_t LCD_RefreshRate_Test_GetState(void);
void LCD_RefreshRate_Test_Run(void);
#endif

void Lcd_SleepProcess(void);
void LCD_ON(void);
void LCD_OFF(void);
void LCD_DMA_Process(void);

#endif
