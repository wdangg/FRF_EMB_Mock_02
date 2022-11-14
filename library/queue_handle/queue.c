#include "Queue.h"

/* Check Empty Of Queue */
Bool Queue_IsEmpty(Queue_Types *Queue)
{
    Bool isEmpty;
    if (Queue->Capacity == 0)
    {
        isEmpty = e_TRUE;
    }
    else
    {
        isEmpty = e_FALSE;
    }

    return isEmpty;
}

/* Check Full Of Queue */
Bool Queue_IsFull(Queue_Types *Queue)
{
    Bool isFull;
    if ((Queue->Size) == (Queue->Capacity))
    {
        isFull = e_TRUE;
    }
    else
    {
        isFull = e_FALSE;
    }
    return isFull;
}

/* Hadling Push Data In Queue */
Bool Queue_PushData(Queue_Types *Queue, uint32_t InputData)
{
    Bool retVal = e_TRUE;
    if (!Queue_IsFull(Queue))
    {
        Queue->QueueArr[Queue->Rear + 1] = InputData;
        Queue->Rear++;
        Queue->Capacity++;
    }
    else
    {
        retVal = e_FALSE;
        // printf("[INFO] Queue is already Full\n");
    }
    return retVal;
}

/* Super Data Push Function */
Bool Queue_ForcePushData(Queue_Types *Queue, uint32_t InputData)
{
    Bool retVal = e_TRUE;
    if (!Queue_IsFull(Queue))
    {
        Queue->QueueArr[Queue->Rear + 1] = InputData;
        Queue->Rear++;
        Queue->Capacity++;
    }
    else
    {
        Queue_Pop(Queue);
        Queue_PushData(Queue, InputData);
    }
    return retVal;
}

/* Pop Data From Queue */
uint32_t Queue_Pop(Queue_Types *Queue)
{
    uint32_t pop_data = 0;
    if (!Queue_IsEmpty(Queue))
    {
        /* Assign Queue Rear Value */
        pop_data = (Queue->QueueArr[Queue->Front]);

        uint8_t count = ((Queue->Rear) - (Queue->Front));

        uint8_t i = 0;
        for (i = 0; i < count; i++)
        {
            Queue->QueueArr[i] = Queue->QueueArr[(Queue->Front) + i + 1];
        }

        Queue->Front = 0;
        Queue->Rear = (count - 1);
        Queue->Capacity = count;
    }
    else
    {
        // printf("\n[ERROR] Queue Empty\n");
    }
    /* Option Clear Value */
    // uint8_t i = 0;
    // for (i = 0; i < (Queue->Size); i++)
    // {
    //     if ( (i<(Queue->Front)) || (i>(Queue->Rear)) )
    //     {
    //         Queue->QueueArr[i] = 0;
    //     }
    //     else
    //     {
    //         continue;
    //     }
    // }

    // printf("\n[INFO] Pop Data: %d\n", pop_data);
    return pop_data;
}

/* Handling Peek Data In Queue */
uint32_t Queue_PeekData(Queue_Types *Queue)
{
    uint32_t peek_data = 0;

    if (!Queue_IsEmpty(Queue))
    {
        peek_data = Queue->QueueArr[Queue->Front];

        if (Queue->Capacity != 1)
        {
            Queue->Front++;
            Queue->Capacity--;
        }
        else
        {
            Queue->Front = 0;
            Queue->Rear = -1;
            Queue->Capacity = 0;
        }

        // printf("\n[INFO] Peek Data: %d\n", peek_data);
    }
    else
    {
        while (1)
        {
            /* ERROR */
            // printf("[ERROR] Error Handler\n");
            // break;
        }
    }

    /* Option Clear Value */
    // uint8_t i = 0;
    // for (i = 0; i < (Queue->Size); i++)
    // {
    //     if ( (i<(Queue->Front)) || (i>(Queue->Rear)) )
    //     {
    //         Queue->QueueArr[i] = 0;
    //     }
    //     else
    //     {
    //         continue;
    //     }
    // }

    return peek_data;
}

/* Display Queue Data*/
void Queue_Display(Queue_Types *Queue)
{
    uint8_t i = 0;
    printf("\n[INFO] Size: %d\tCapacity: %d\tFront: %d\tRear: %d\n", (Queue->Size), (Queue->Capacity),
           (Queue->Front), (Queue->Rear));
    for (i = 0; i < (Queue->Size); i++)
    {
        printf("[INFO] Index %d:  %d\n", i, (Queue->QueueArr[i]));
    }
}

/* Clear Queue Data */
void Queue_FormatData(Queue_Types *Queue)
{
    uint8_t i = 0;
    for (i = 0; i < (Queue->Size); i++)
    {
        Queue->QueueArr[i] = 0;
    }
    Queue->Front = 0;
    Queue->Rear = -1;
}

