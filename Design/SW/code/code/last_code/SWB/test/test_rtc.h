/* test/test_rtc.h */

#ifndef TEST_RTC_H_
#define TEST_RTC_H_

/*
 * Yêu cầu trước khi gọi:
 *  - HAL_Init()
 *  - BSP_Clock_Init()
 *  - BSP_GPIO_Init()
 *  - BSP_I2C_Init()
 *  - BSP_LCD_Init()
 *  - DS1307_Config()
 *  - (DS1307_SetTime() nếu bạn muốn set lại giờ)
 */

void TEST_RTC_ShowTimeLoop(void);   // hiển thị giờ liên tục lên LCD
void TEST_RTC_ShowDateTimeLoop(void); // hiển thị cả ngày + giờ

#endif /* TEST_RTC_H_ */
