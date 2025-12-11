/* driver/devices/ds1307.c */

#include "ds1307.h"
#include "bsp_i2c.h"

static uint8_t DS1307_DecodeBCD(uint8_t bin);
static uint8_t DS1307_EncodeBCD(uint8_t dec);
static void    DS1307_SetRegByte(uint8_t regAddr, uint8_t val);
static uint8_t DS1307_GetRegByte(uint8_t regAddr);
static uint8_t DS1307_GetClockHalt(void);
static void    DS1307_SetClockHalt(uint8_t halt);
static void    DS1307_SetTimeZone(int8_t hr, uint8_t min);

void DS1307_Config(void)
{
    DS1307_SetClockHalt(0);
    DS1307_SetTimeZone(+8, 0);
}

void DS1307_GetTime(DS1307_TimeTypeDef *t)
{
    uint16_t cen;

    t->sec   = DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_SECOND) & 0x7f);
    t->min   = DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_MINUTE));
    t->hour  = DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_HOUR) & 0x3f);
    t->dow   = DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_DOW));
    t->date  = DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_DATE));
    t->month = DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_MONTH));
    cen      = DS1307_GetRegByte(DS1307_REG_CENT) * 100;
    t->year  = DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_YEAR)) + cen;
}

void DS1307_SetTime(uint8_t sec, uint8_t min, uint8_t hour_24mode,
                    uint8_t dayOfWeek, uint8_t date, uint8_t month, uint16_t year)
{
    uint8_t ch = DS1307_GetClockHalt() << 7;

    DS1307_SetRegByte(DS1307_REG_SECOND, ch | DS1307_EncodeBCD(sec));
    DS1307_SetRegByte(DS1307_REG_MINUTE, DS1307_EncodeBCD(min));
    DS1307_SetRegByte(DS1307_REG_HOUR,   DS1307_EncodeBCD(hour_24mode & 0x3f));
    DS1307_SetRegByte(DS1307_REG_DOW,    DS1307_EncodeBCD(dayOfWeek));
    DS1307_SetRegByte(DS1307_REG_DATE,   DS1307_EncodeBCD(date));
    DS1307_SetRegByte(DS1307_REG_MONTH,  DS1307_EncodeBCD(month));
    DS1307_SetRegByte(DS1307_REG_CENT,   year / 100);
    DS1307_SetRegByte(DS1307_REG_YEAR,   DS1307_EncodeBCD(year % 100));
}

/* ---- static function ---- */
static void DS1307_SetRegByte(uint8_t regAddr, uint8_t val)
{
    uint8_t bytes[2] = { regAddr, val };
    HAL_I2C_Master_Transmit(&hi2c2, DS1307_I2C_ADDR << 1, bytes, 2, DS1307_TIMEOUT);
}

static uint8_t DS1307_GetRegByte(uint8_t regAddr)
{
    uint8_t val;
    HAL_I2C_Master_Transmit(&hi2c2, DS1307_I2C_ADDR << 1, &regAddr, 1, DS1307_TIMEOUT);
    HAL_I2C_Master_Receive(&hi2c2, DS1307_I2C_ADDR << 1, &val, 1, DS1307_TIMEOUT);
    return val;
}

static uint8_t DS1307_GetClockHalt(void)
{
    return (DS1307_GetRegByte(DS1307_REG_SECOND) & 0x80) >> 7;
}

static void DS1307_SetClockHalt(uint8_t halt)
{
    uint8_t ch = (halt ? 1 << 7 : 0);
    DS1307_SetRegByte(DS1307_REG_SECOND,
                      ch | (DS1307_GetRegByte(DS1307_REG_SECOND) & 0x7f));
}

static void DS1307_SetTimeZone(int8_t hr, uint8_t min)
{
    DS1307_SetRegByte(DS1307_REG_UTC_HR,  (uint8_t)hr);
    DS1307_SetRegByte(DS1307_REG_UTC_MIN, min);
}

static uint8_t DS1307_DecodeBCD(uint8_t bin)
{
    return (((bin & 0xf0) >> 4) * 10) + (bin & 0x0f);
}

static uint8_t DS1307_EncodeBCD(uint8_t dec)
{
    return (dec % 10 + ((dec / 10) << 4));
}
