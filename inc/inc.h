#ifndef __INC_H__
#define __INC_H__

#include "MKL46Z4.h"
#include "app.h"
#include "uart0_handle.h"
#include "queue.h"


#define QUEUE_SIZE                          (0x8)
uint32_t data_arr[QUEUE_SIZE] = {0};  
Queue_Types queue = 
{
  0, -1, QUEUE_SIZE, /* Front, Rear, Size */
  0,
  &data_arr[0]
};


#endif /* __INC_H__ */