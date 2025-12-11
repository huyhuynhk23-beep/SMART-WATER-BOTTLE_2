/* test/test_lcd.h */
#ifndef TEST_LCD_H_
#define TEST_LCD_H_

/*
 * Các hàm test LCD.
 * Lưu ý: trước khi gọi,
 * cần đảm bảo đã gọi:
 *   HAL_Init();
 *   BSP_Clock_Init();
 *   BSP_GPIO_Init();
 *   BSP_I2C_Init();
 *   BSP_LCD_Init();
 */

void TEST_LCD_Basic(void);
void TEST_LCD_ScrollText(void);

#endif /* TEST_LCD_H_ */
