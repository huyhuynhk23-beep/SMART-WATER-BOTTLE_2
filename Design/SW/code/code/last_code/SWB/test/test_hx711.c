/* test/test_hx711.c */

#include "test_hx711.h"
#include "hx711.h"
#include "bsp_lcd.h"
#include "main.h"   // HAL_Delay

#include <stdio.h>

/*
 * Đọc HX711 1 lần, hiển thị lên LCD rồi return.
 * Dùng khi bạn muốn test nhanh trong 1 đoạn code.
 */
void TEST_HX711_Once(void)
{
    char line1[20];
    char line2[20];

    int32_t weight_mg = HX711_WeighMg();
    int     weight_ml = weight_mg / 1000;    // giả sử nước => mg ~ ml

    snprintf(line1, sizeof(line1), "HX711 1-shot");
    snprintf(line2, sizeof(line2), "W:%4d ml", weight_ml);

    BSP_LCD_Show2Lines(line1, line2);

    HAL_Delay(1000);
}

/*
 * Đọc HX711 liên tục, hiển thị giá trị lên LCD,
 * giống như “live monitor”.
 */
void TEST_HX711_Loop(void)
{
    char line1[20];
    char line2[20];

    while (1)
    {
        int32_t weight_mg = HX711_WeighMg();
        int     weight_ml = weight_mg / 1000;  // mg -> ml (nước gần = 1g/ml)

        snprintf(line1, sizeof(line1), "HX711 Test");
        snprintf(line2, sizeof(line2), "W:%4d ml", weight_ml);

        BSP_LCD_Show2Lines(line1, line2);

        HAL_Delay(200);   // update ~5 lần/giây
    }
}
