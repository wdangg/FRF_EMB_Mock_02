#ifndef __MIDDLEWARE_H__
#define __MIDDLEWARE_H__

#include "driver.h"

void delay_ms(uint32_t us_time);
Bool digitalRead(Object_Type *object_reading);
void GPIO_TogglePin(Object_Type *object_toggle);
void GPIO_ClearPin(Object_Type *object_clear);
void GPIO_SetPin(Object_Type *object_set);

#endif /* __MIDDLEWARE_H__ */
