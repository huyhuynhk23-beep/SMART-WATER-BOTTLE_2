/* driver/devices/ds1307.h */

#ifndef DS1307_H_
#define DS1307_H_

#include "board.h"

void DS1307_Config(void);
void DS1307_SetTime(uint8_t sec, uint8_t min, uint8_t hour_24mode,
                    uint8_t dayOfWeek, uint8_t date, uint8_t month, uint16_t year);
void DS1307_GetTime(DS1307_TimeTypeDef *t);

#endif /* DS1307_H_ */
