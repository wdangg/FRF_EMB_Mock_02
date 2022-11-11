#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "driver.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

/*** Define Queue Types ***/
typedef struct
{
    int8_t Front, Rear, Size;
    uint8_t Capacity;
    uint32_t *QueueArr;
} Queue_Types;

/*******************************************************************************
 * API
 ******************************************************************************/

/* Check Empty Of Queue */
Bool Queue_IsEmpty(Queue_Types *Queue);

/* Check full of Queue */
Bool Queue_IsFull(Queue_Types *Queue);

/* Pop Data From Queue */
uint32_t Queue_Pop(Queue_Types *Queue);

/* Hadling Push Data In Queue */
Bool Queue_PushData(Queue_Types *Queue, uint32_t InputData);

/* Super Data Push Function */
Bool Queue_ForcePushData(Queue_Types *Queue, uint32_t InputData);

/* Handling Peek Data In Queue */
uint32_t Queue_PeekData(Queue_Types *Queue);

/* Display Queue Data*/
void Queue_Display(Queue_Types *Queue);

#endif /* __QUEUE_H__ */
