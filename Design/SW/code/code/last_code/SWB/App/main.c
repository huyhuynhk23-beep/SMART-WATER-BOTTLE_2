/* app/main.c */

#include "main.h"
#include <stdio.h>

#include "board.h"

#include "bsp_clk.h"
#include "bsp_gpio.h"
#include "bsp_i2c.h"
#include "bsp_tim.h"
#include "bsp_lcd.h"

#include "ds1307.h"
#include "hx711.h"
#include "app_main.h"

/* Biến app */
static int32_t last_weight_mg = 0;
static float   daily_total_ml = 0.0f;
static uint8_t prev_date      = 0;

int app_main(void)
{
    HAL_Init();

    /* BSP init */
    BSP_Clock_Init();
    BSP_GPIO_Init();
    BSP_I2C_Init();
    BSP_TIM_Init();
    BSP_LCD_Init();

    /* Driver init */
    DS1307_Config();
    // DS1307_SetTime(...);  // chỉ dùng 1 lần để set giờ, sau đó comment
    HX711_Init();

    /* Đọc lần đầu làm mốc */
    DS1307_TimeTypeDef now;
    DS1307_GetTime(&now);
    last_weight_mg = HX711_WeighMg();
    prev_date      = now.date;

    char line1[20];
    char line2[20];

    while (1)
    {
        /* 1. Đọc thời gian */
        DS1307_GetTime(&now);
        weight = weigh();
        /* 2. Đọc loadcell */
        int32_t current_weight_mg = HX711_WeighMg();

        /* 3. Sang ngày mới -> reset tổng */
        if (now.date != prev_date)
        {
            daily_total_ml = 0.0f;
            prev_date      = now.date;
        }

        /* 4. Tính lượng nước đã uống */
        int32_t diff = last_weight_mg - current_weight_mg;
        if (diff > 0)
        {
            daily_total_ml += (float)diff / 1000.0f;  // mg -> ml
        }
        last_weight_mg = current_weight_mg;

        /* 5. Thể tích hiện tại và tổng trong ngày */
        int current_volume_ml = current_weight_mg / 1000;
        int daily_ml          = (int)daily_total_ml;

        /* 6. Hiển thị lên LCD */
        sprintf(line1, "%02d:%02d:%02d", now.hour, now.min, now.sec);
        sprintf(line2, "B:%3d T:%3dml", current_volume_ml, daily_ml);

        BSP_LCD_Show2Lines(line1, line2);

        HAL_Delay(100);   // đọc / update mỗi 100ms
    }
}
