/* BSP/bsp_tim.h */

#ifndef BSP_TIM_H_
#define BSP_TIM_H_

#include "main.h"

extern TIM_HandleTypeDef htim2;

void BSP_TIM_Init(void);

/* delay us dùng cho HX711 */
void BSP_DelayUs(uint16_t us);

#endif /* BSP_TIM_H_ */
