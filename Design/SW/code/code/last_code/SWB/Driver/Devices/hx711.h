/* driver/devices/hx711.h */

#ifndef HX711_H_
#define HX711_H_

#include "board.h"

void     HX711_Init(void);
int32_t  HX711_WeighMg(void);   // trả về mg

#endif /* HX711_H_ */
