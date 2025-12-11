/* driver/devices/hx711.c */

#include "hx711.h"
#include "bsp_tim.h"

/* các tham số calibrate, bạn giữ nguyên */
static uint32_t tare          = 7831281;
static float    knownOriginal = 110000.0f;  // mg
static float    knownHX711    = 42386.0f;

static int32_t HX711_ReadRaw(void);

void HX711_Init(void)
{
    /* Đưa SCK về known state + “đánh thức” HX711 */
    HAL_GPIO_WritePin(HX711_SCK_GPIO_PORT, HX711_SCK_PIN, GPIO_PIN_SET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(HX711_SCK_GPIO_PORT, HX711_SCK_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);
}

int32_t HX711_WeighMg(void)
{
    int32_t total   = 0;
    int32_t samples = 50;
    float   coeff   = knownOriginal / knownHX711;

    for (uint16_t i = 0; i < samples; i++)
    {
        total += HX711_ReadRaw();
    }

    int32_t average   = total / samples;
    int32_t milligram = (int32_t)((average - (int32_t)tare) * coeff);

    return milligram;
}

static int32_t HX711_ReadRaw(void)
{
    uint32_t data      = 0;
    uint32_t startTime = HAL_GetTick();

    /* Chờ DT kéo xuống LOW (có dữ liệu) */
    while (HAL_GPIO_ReadPin(HX711_DT_GPIO_PORT, HX711_DT_PIN) == GPIO_PIN_SET)
    {
        if (HAL_GetTick() - startTime > 200)
        {
            return 0; // timeout
        }
    }

    /* Đọc 24 bit */
    for (int8_t i = 0; i < 24; i++)
    {
        HAL_GPIO_WritePin(HX711_SCK_GPIO_PORT, HX711_SCK_PIN, GPIO_PIN_SET);
        BSP_DelayUs(1);
        data <<= 1;
        HAL_GPIO_WritePin(HX711_SCK_GPIO_PORT, HX711_SCK_PIN, GPIO_PIN_RESET);
        BSP_DelayUs(1);

        if (HAL_GPIO_ReadPin(HX711_DT_GPIO_PORT, HX711_DT_PIN) == GPIO_PIN_SET)
        {
            data++;
        }
    }

    /* Chuyển sang dạng signed 24-bit */
    data ^= 0x800000;

    /* Thêm 1 xung clock để chọn gain/channel (mặc định) */
    HAL_GPIO_WritePin(HX711_SCK_GPIO_PORT, HX711_SCK_PIN, GPIO_PIN_SET);
    BSP_DelayUs(1);
    HAL_GPIO_WritePin(HX711_SCK_GPIO_PORT, HX711_SCK_PIN, GPIO_PIN_RESET);
    BSP_DelayUs(1);

    return (int32_t)data;
}
