/* BSP/bsp_lcd.h */

#ifndef BSP_LCD_H_
#define BSP_LCD_H_

#include "main.h"
#include "i2c_lcd.h"

void BSP_LCD_Init(void);
void BSP_LCD_Show2Lines(const char *line1, const char *line2);

#endif /* BSP_LCD_H_ */
