/* test/test_hx711.h */
#ifndef TEST_HX711_H_
#define TEST_HX711_H_

/*
 * Test HX711:
 *  Yêu cầu đã:
 *   - HAL_Init()
 *   - BSP_Clock_Init()
 *   - BSP_GPIO_Init()
 *   - BSP_TIM_Init()
 *   - BSP_I2C_Init()      (nếu in ra LCD)
 *   - BSP_LCD_Init()      (nếu in ra LCD)
 *   - HX711_Init()
 */

void TEST_HX711_Once(void);   // Đọc 1 lần rồi return
void TEST_HX711_Loop(void);   // Đọc liên tục và hiển thị lên LCD

#endif /* TEST_HX711_H_ */
