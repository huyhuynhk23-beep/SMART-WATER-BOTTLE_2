/* common/board.h */

#ifndef BOARD_H_
#define BOARD_H_

#include "main.h"   // chứa stm32f1xx_hal.h, typedef… do CubeMX tạo

/* ---- Cấu hình chân HX711 ---- */
#define HX711_DT_GPIO_PORT   GPIOB
#define HX711_DT_PIN         GPIO_PIN_8

#define HX711_SCK_GPIO_PORT  GPIOB
#define HX711_SCK_PIN        GPIO_PIN_9

/* ---- Cấu hình LCD I2C ---- */
#define LCD_I2C_ADDRESS      0x4E   // như bạn đang dùng

/* ---- Cấu hình RTC DS1307 ---- */
#define DS1307_I2C_ADDR      0x68

#define DS1307_REG_SECOND    0x00
#define DS1307_REG_MINUTE    0x01
#define DS1307_REG_HOUR      0x02
#define DS1307_REG_DOW       0x03
#define DS1307_REG_DATE      0x04
#define DS1307_REG_MONTH     0x05
#define DS1307_REG_YEAR      0x06
#define DS1307_REG_CONTROL   0x07
#define DS1307_REG_UTC_HR    0x08
#define DS1307_REG_UTC_MIN   0x09
#define DS1307_REG_CENT      0x10

#define DS1307_TIMEOUT       1000

/* Struct thời gian đọc từ DS1307 */
typedef struct {
    uint8_t  sec;
    uint8_t  min;
    uint8_t  hour;
    uint8_t  dow;
    uint8_t  date;
    uint8_t  month;
    uint16_t year;
} DS1307_TimeTypeDef;

#endif /* BOARD_H_ */
