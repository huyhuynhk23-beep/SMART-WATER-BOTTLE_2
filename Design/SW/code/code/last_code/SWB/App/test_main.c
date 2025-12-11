/* test/test_main.c
 *
 * Main dùng để test:
 *  - LCD
 *  - HX711
 *  - RTC (DS1307)
 */

#include "main.h"
#include <stdio.h>

#include "board.h"

/* BSP */
#include "bsp_clock.h"
#include "bsp_gpio.h"
#include "bsp_i2c.h"
#include "bsp_tim.h"
#include "bsp_lcd.h"

/* Drivers */
#include "ds1307.h"
#include "hx711.h"

/* Tests */
#include "test_lcd.h"
#include "test_hx711.h"
#include "test_rtc.h"

int main(void)
{
    HAL_Init();

    /* ---- BSP Init ---- */
    BSP_Clock_Init();
    BSP_GPIO_Init();
    BSP_I2C_Init();
    BSP_TIM_Init();
    BSP_LCD_Init();

    /* ---- Driver Init ---- */
    DS1307_Config();
    HX711_Init();

    /* Nếu muốn set giờ DS1307 lần đầu:
       DS1307_SetTime(0, 0, 12, 1, 1, 1, 2025);
       Sau khi set xong giờ đúng thì comment lại.
    */

    /* ----------------------------------------------------------------
     * Chọn TEST nào chạy:
     *  - Chỉ nên giữ lại 1 hàm test “loop” vì nó sẽ không return.
     *  - Hoặc gọi vài hàm "Once" rồi sau đó vào 1 vòng loop chính.
     * ---------------------------------------------------------------- */

    /* ====== Ví dụ 1: Test LCD ====== */
    // TEST_LCD_Basic();
    // TEST_LCD_ScrollText();    // vòng lặp vô hạn

    /* ====== Ví dụ 2: Test HX711 ====== */
    // TEST_HX711_Once();
    // TEST_HX711_Loop();        // vòng lặp vô hạn

    /* ====== Ví dụ 3: Test RTC ====== */
    // TEST_RTC_ShowTimeLoop();      // hiển thị giờ
    TEST_RTC_ShowDateTimeLoop();    // hiển thị ngày + giờ

    while (1)
    {
        /* Có thể thêm code test chung ở đây nếu cần */
    }
}
