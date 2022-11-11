#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "MKL46Z4.h"

#define PE_PS_MASK 										(0x3)
#define GPIO_MODE 										(0x1)
#define PIT_ACTIVE 										(0x1)
#define PIT_TIMMER_ENABLE 								(0x1)
#define W1C 											(0x1)

typedef enum Bool
{
	e_FALSE = 0U,
	e_TRUE = 1U
} Bool;

typedef enum __clock_port
{
	PORTA_CLOCK = 0U,
	PORTB_CLOCK = 1U,
	PORTC_CLOCK = 2U,
	PORTD_CLOCK = 3U,
	PORTE_CLOCK = 4U
} ClockPortType;

typedef enum INT_Trigger_Type
{
	RISING_EDGE = 0x9,
	FALLING_EDGE = 0xA,
	LOW_LOGIC = 0x8,
	HIGH_LOGIC = 0xC,
	CHANGE = 0xB,
} INT_Trigger_Type;

typedef enum Pin_Moding
{
	INPUT = 0U,
	OUTPUT = 1U,
	INPUT_PULLUP = 3U,
	INPUT_PULLDOWN = 2U
} Pin_Moding_Type;

typedef enum Push_Pull_Type
{
	__PULLUP = 0x3,
	__PULLDOWN = 0x2,
	__NOPULL = 0x0
} Push_Pull_Type;

typedef struct Object_Type
{
	PORT_Type *PORTx;
	FGPIO_Type *FGPIOx;
	uint8_t pin_number;
	Pin_Moding_Type pin_moding;
} Object_Type;

uint8_t getPortSource(PORT_Type *PORTx);
Bool PORT_Init(PORT_Type *PORTx);
Bool pinMode(Object_Type *s_pin_config);
Bool attachExternalInterrupt(IRQn_Type IRQn_x, Object_Type *digitalPinToInterrupts, INT_Trigger_Type trigger_type);
Bool isPinInterruptsRequest(Object_Type *check_pin_request);
Bool PORT_EXTI_ClearFlag(Object_Type *pin_flag);
void ERROR_Handler();

#endif /* __DRIVER_H__ */