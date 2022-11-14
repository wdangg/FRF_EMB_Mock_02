#ifndef __UART0_HANDLE_H__
#define __UART0_HANDLE_H__

#include "MKL46Z4.h"

#define UART0_RX_PIN                                1
#define UART0_TX_PIN                                2

/* Function Prototypes */
void UART0_Clock48Mhz();
void UART0_PortInit();
void UART0_SetBaudRate(uint32_t clockSource, uint32_t baudRate);
void UART0_SetFrame();
void UART0_Init();
void UART0_Transmit(uint8_t u8Data);
uint8_t UART0_Receive(void);
/* Setup for Reveive Interrupt */
void UART0_SetupReceiveINT();
void UART0_OutString(uint8_t *s, uint8_t len);

void UART0_Init_9600();

#endif /* __UART0_HANDLE_H__ */

