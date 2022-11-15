#ifndef __INC_H__
#define __INC_H__

#include "MKL46Z4.h"
#include "app.h"
#include "uart0_handle.h"
#include "queue.h"
#include "pit_handle.h"

#define GREEN_LED_PIN 					    (0x5) 
#define RED_LED_PIN                         (0x1D)
#define PIT_LOAD_VALUE                      (0xA00000/2)
#define BUTTON2_PIN 					    (0xC)	
#define BUTTON_PIN 						    (0x3)	

Object_Type output_led_PE29 = 
{
  PORTE,
  FGPIOE,
  RED_LED_PIN,
  OUTPUT
};

Object_Type ouput_led_PD5 = {
	PORTD,
	FGPIOD,
	GREEN_LED_PIN,
	OUTPUT
};

PIT_Object_Type pit_object_0 =
{
  e_PIT_CHANNEL_0,
  PIT_LOAD_VALUE,
  e_TRUE, 
  e_TRUE,
  5
};

Object_Type input_pin_PC3 = {
	PORTC,
	FGPIOC,
	BUTTON_PIN,
	INPUT_PULLUP
};

Object_Type input_pin_PC12 = {
	PORTC,
	FGPIOC,
	BUTTON2_PIN,
	INPUT_PULLUP
};

#endif /* __INC_H__ */