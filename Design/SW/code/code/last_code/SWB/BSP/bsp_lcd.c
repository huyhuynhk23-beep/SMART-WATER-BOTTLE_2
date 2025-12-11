/* BSP/bsp_lcd.c */

#include "bsp_lcd.h"
#include "bsp_i2c.h"
#include "board.h"

static I2C_LCD_HandleTypeDef hlcd;

void BSP_LCD_Init(void)
{
    hlcd.hi2c    = &hi2c1;           // dùng I2C1
    hlcd.address = LCD_I2C_ADDRESS;  // 0x4E
    lcd_init(&hlcd);

    lcd_clear(&hlcd);
    lcd_puts(&hlcd, "STM32 I2C LCD");
    lcd_gotoxy(&hlcd, 0, 1);
    lcd_puts(&hlcd, "Library Demo");
}

/* Hàm tiện ích: hiển thị 2 dòng liền */
void BSP_LCD_Show2Lines(const char *line1, const char *line2)
{
    lcd_clear(&hlcd);
    lcd_gotoxy(&hlcd, 0, 0);
    lcd_puts(&hlcd, line1);
    lcd_gotoxy(&hlcd, 0, 1);
    lcd_puts(&hlcd, line2);
}
