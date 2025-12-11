/* test/test_rtc.c */

#include "test_rtc.h"
#include "ds1307.h"
#include "bsp_lcd.h"
#include "main.h"      // để dùng HAL_Delay
#include <stdio.h>
#include <string.h>

/*
 * Test RTC cơ bản:
 *  - Đọc thời gian từ DS1307
 *  - Hiển thị hh:mm:ss lên LCD
 */
void TEST_RTC_ShowTimeLoop(void)
{
    DS1307_TimeTypeDef now;
    char line1[20];
    char line2[20];

    /* Nếu muốn set giờ cố định để test, có thể dùng 1 lần:
       DS1307_SetTime(0, 0, 12, 1, 1, 1, 2025);
       Sau khi đã đúng giờ thì comment lại để RTC không bị reset khi reset MCU.
    */

    while (1)
    {
        DS1307_GetTime(&now);

        /* Dòng 1: hh:mm:ss */
        snprintf(line1, sizeof(line1), "%02d:%02d:%02d",
                 now.hour, now.min, now.sec);

        /* Dòng 2: thông báo đơn giản */
        snprintf(line2, sizeof(line2), "RTC Test");

        BSP_LCD_Show2Lines(line1, line2);

        HAL_Delay(500); // cập nhật 2 lần/giây
    }
}

/*
 * Test RTC hiển thị đầy đủ ngày + giờ:
 *  - Dòng 1: dd/mm/yyyy
 *  - Dòng 2: hh:mm:ss
 */
void TEST_RTC_ShowDateTimeLoop(void)
{
    DS1307_TimeTypeDef now;
    char line1[20];
    char line2[20];

    while (1)
    {
        DS1307_GetTime(&now);

        snprintf(line1, sizeof(line1), "%02d/%02d/%04d",
                 now.date, now.month, now.year);

        snprintf(line2, sizeof(line2), "%02d:%02d:%02d",
                 now.hour, now.min, now.sec);

        BSP_LCD_Show2Lines(line1, line2);

        HAL_Delay(500);
    }
}
