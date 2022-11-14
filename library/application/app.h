#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "middleware.h"
#include "write_flash.h"
#include "string_handle.h"

/* Process The Uint32_t "temp" Value And Uint8_t "data"  */
void APP_Temp_Processing(uint32_t *temp, uint8_t *data, uint8_t *count, uint8_t *enable_flash);


#endif /* __APPLICATION_H__ */