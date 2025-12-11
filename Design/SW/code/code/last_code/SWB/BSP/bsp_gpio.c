/* BSP/bsp_gpio.c */

#include "bsp_gpio.h"
#include "board.h"

void BSP_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Bật clock GPIO */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* SCK PB9 output, default LOW */
    HAL_GPIO_WritePin(HX711_SCK_GPIO_PORT, HX711_SCK_PIN, GPIO_PIN_RESET);

    /* DT PB8 input */
    GPIO_InitStruct.Pin  = HX711_DT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(HX711_DT_GPIO_PORT, &GPIO_InitStruct);

    /* SCK PB9 output push-pull */
    GPIO_InitStruct.Pin   = HX711_SCK_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(HX711_SCK_GPIO_PORT, &GPIO_InitStruct);


}
