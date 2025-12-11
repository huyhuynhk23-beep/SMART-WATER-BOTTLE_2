/* BSP/bsp_i2c.h */

#ifndef BSP_I2C_H_
#define BSP_I2C_H_

#include "main.h"

/* expose handle để driver khác dùng */
extern I2C_HandleTypeDef hi2c1;   // cho LCD
extern I2C_HandleTypeDef hi2c2;   // cho DS1307

void BSP_I2C_Init(void);

#endif /* BSP_I2C_H_ */
