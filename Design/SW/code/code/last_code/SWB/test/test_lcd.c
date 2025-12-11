/* test/test_lcd.c */

#include "test_lcd.h"
#include "bsp_lcd.h"
#include "main.h"    // để dùng HAL_Delay, nếu cần

#include <stdio.h>

/*
 * Test cơ bản:
 *  - Xóa màn hình
 *  - In 2 dòng cố định
 *  - Đợi 1s
 */
void TEST_LCD_Basic(void)
{
    BSP_LCD_Show2Lines("LCD Test", "Hello STM32");
    HAL_Delay(1000);
}

/*
 * Test nâng cao:
 *  - Hiển thị text đếm tăng dần
 *  - Cho bạn thấy LCD update liên tục OK
 */
void TEST_LCD_ScrollText(void)
{
    char line1[20];
    char line2[20];

    int counter = 0;

    while (1)
    {
        snprintf(line1, sizeof(line1), "Count: %5d", counter);
        snprintf(line2, sizeof(line2), "LCD OK      ");

        BSP_LCD_Show2Lines(line1, line2);

        counter++;
        HAL_Delay(500);  // mỗi 0.5s tăng 1
    }
}
