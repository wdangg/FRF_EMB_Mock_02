#ifndef __PIT_HANDLE_H__
#define __PIT_HANDLE_H__

#include "MKL46Z4.h"
#include "driver.h"

typedef enum PIT_Channel_n
{
	e_PIT_CHANNEL_0 = 0U,
	e_PIT_CHANNEL_1 = 1U 
} PIT_Channel_n;

typedef struct PIT_Object_Type
{
	PIT_Channel_n e_channel;
	uint32_t load_value;
	Bool freeze_option;
	Bool interrupts_option;
	uint8_t priority;
} PIT_Object_Type;

typedef enum PIT_INT_Channel_Return
{
    e_PIT0_INT_Channel_Return = 0,
    e_PIT1_INT_Channel_Return = 1,
    e_NONE_INT_PIT_RETURN     = 2
} PIT_INT_Channel_Return;

Bool attachPITInterrupts(PIT_Object_Type *pit_object);
Bool checkPITFlag(PIT_Object_Type *pit_object_flag);
Bool clearPITFlag(PIT_Object_Type *pit_flag);
PIT_INT_Channel_Return checkPITInterruptChannelRequest();
Bool clearPITChannelFlag(PIT_INT_Channel_Return e_channel);

__weak void PIT_IRQHandler_CallBack();

#endif /* __PIT_HANDLE_H__ */


