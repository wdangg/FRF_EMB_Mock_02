#include "uart0_handle.h"

void UART0_OutString(uint8_t *s, uint8_t len)
{
  uint8_t i = 0;
  for (i = 0; i < len; i++)
  {
    UART0_Transmit(s[i]);
  }
  UART0_Transmit(10);
}

void UART0_SetupReceiveINT()
{
  /* Setup UART Interrupt */
  UART0->C2 |= UART0_C2_RIE(1);

  /* Enable UART Interrupt by NVIC */
  NVIC_EnableIRQ(UART0_IRQn);
}

void UART0_Clock48Mhz()
{
  /*Set UART0_Clock = MCGFLLCLK = 32768*640 = 20971520Hz*/

  /*set the slow internal reference clock: 32768kHz*/
  MCG->C1 |= MCG_C1_IREFS_MASK;

  /*set FLL*/
  MCG->C6 &= (~MCG_C6_PLLS_MASK);

  /*set FLL Factor = 640*/
  MCG->C4 &= (~MCG_C4_DMX32_MASK);
  MCG->C4 &= (~MCG_C4_DRST_DRS_MASK);

  /*set UART0_Clock = MCGFLLCLK*/
  SIM->SOPT2 &= (~SIM_SOPT2_PLLFLLSEL_MASK);
  SIM->SOPT2 = (SIM->SOPT2 & ~SIM_SOPT2_UART0SRC_MASK) | SIM_SOPT2_UART0SRC(1);
  // SIM->SOPT2 = (SIM->SOPT2 & ~SIM_SOPT2_UART0SRC_MASK) | SIM_SOPT2_UART0SRC(3);

  /* UART0_Clock = MCGIRCLK */
  // SIM->SOPT2 &= (~SIM_SOPT2_PLLFLLSEL_MASK);
  // SIM->SOPT2 = (SIM->SOPT2 & ~SIM_SOPT2_UART0SRC_MASK) | SIM_SOPT2_UART0SRC(1);

  /* Enable clock for PORTA & UART0 */
  SIM->SCGC5 |= SIM_SCGC5_PORTA(1);
  SIM->SCGC4 |= SIM_SCGC4_UART0(1);
}

void UART0_SetBaudrate()
{
  /*set Baudrate = 20971520 / (26*7) = 115228*/
  /*set OSR + 1 = 7*/
  UART0->C4 = (UART0->C4 & (~UART0_C4_OSR_MASK)) | UART0_C4_OSR(6);

  /*set BR = 26*/
  UART0->BDH &= (~UART0_BDH_SBR_MASK);
  UART0->BDL = (UART0->BDL & (~UART0_BDL_SBR_MASK)) | UART0_BDL_SBR(26);
}

void UART0_PortInit()
{
  /* Clear PCR_MUX Register */
  PORTA->PCR[UART0_RX_PIN] &= ~PORT_PCR_MUX_MASK;
  PORTA->PCR[UART0_TX_PIN] &= ~PORT_PCR_MUX_MASK;

  /* Set Pin PA1 & PA2 = ALT2 Mode */
  PORTA->PCR[UART0_RX_PIN] |= PORT_PCR_MUX(2);
  PORTA->PCR[UART0_TX_PIN] |= PORT_PCR_MUX(2);

  /* Set up Pull-up for UART Pin */
  PORTA->PCR[UART0_RX_PIN] |= PORT_PCR_PE(1) | PORT_PCR_PS(1);
  PORTA->PCR[UART0_TX_PIN] |= PORT_PCR_PE(1) | PORT_PCR_PS(1);
}

void UART0_SetFrame()
{
  /* Select number of Data Bits - 8 bits */
  UART0->C4 &= (~UART0_C4_M10_MASK);
  UART0->C1 &= (~UART0_C1_M_MASK);

  /* Select Parity Bit - None */
  UART0->C1 &= (~UART0_C1_PE_MASK);

  /*set mode LSB*/
  UART0->S2 &= (~UART0_S2_MSBF_MASK);

  /*set receive data not inverted*/
  UART0->S2 &= (~UART0_S2_RXINV_MASK);

  /*set transmit data not inverted*/
  UART0->C3 &= (~UART0_C3_TXINV_MASK);
}

void UART0_Init_115200(uint32_t priority)
{
  UART0_Clock48Mhz();
  SystemCoreClockUpdate();
  UART0_PortInit();
  UART0_SetBaudrate();
  UART0_SetFrame();

  /* Enable Transmitter & Receiver */
  UART0->C2 |= UART0_C2_TE_MASK;
  UART0->C2 |= UART0_C2_RE_MASK;

  /* Set Up Interrupts For UART0 */
  UART0_SetupReceiveINT();

  /* NVIC Set Priority */
  NVIC_SetPriority(UART0_IRQn, priority);

  uint8_t t[] = "start transfer";
  UART0_OutString(t, sizeof(t));
}

void UART0_Transmit(uint8_t u8Data)
{
  /* Wait Data Register is Empty */
  while ((UART0->S1 & UART0_S1_TDRE_MASK) == 0)
    ;

  /* Write to Data Register */
  UART0->D = u8Data;

  /*wait transmission activity complete*/
  while ((UART0->S1 & UART0_S1_TC_MASK) == 0)
    ;
}

uint8_t UART0_Receive(void)
{
  /* Wait Data Register is full */
  while (((UART0->S1 & UART0_S1_RDRF_MASK) == 0))
    ;

  /* Read and Return Data Register */
  return UART0->D;
}

void UART0_Init_9600(uint32_t priority)
{

  MCG->C4 |= MCG_C4_DMX32_MASK; // Lowest range and DMX32 = 1 -> 24MHZ
  SIM->SOPT2 = (1u << 26);      // SIM_SOPT2_UART0SRC(1);Clock to UART0 MCGFLLCLK 20.97152MHz as MCG_C4=0x00 low range 20Mhz

  // SIM_SOPT5=0x00;kar na kar
  // open-drain mode disabled(Clear bit 16) plus
  // connection to UART_TX and UART_RX(Module to module interconnect) Clear bit 2,1 and 0
  SIM->SCGC4 |= SIM_SCGC4_UART0_MASK; // Open UART0
  SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; // Open PORTA

  PORTA->PCR[1] = PORT_PCR_MUX(2); // PTA1 as UART1_RX (010  9 for alternative 2 which is UART0_RX)
  PORTA->PCR[2] = PORT_PCR_MUX(2); // PTA2 as UART1_RX (010  9 for alternative 2 which is UART0_TX)

  // TX and RX both disabled
  UART0->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK); // UART0->C2=0x00;

  // Baud = Clock/(OSR+1)*BR
  UART0->BDH = 0x00; // Baud rate =9600,24Mhz clock
  UART0->BDL = 0x9C; // BR=156

  UART0->C1 = 0x00;
  UART0->C3 = 0x00;
  UART0->C4 = 0x0F; // OSR=15
  UART0->C5 = 0x00; // not both edges 0x02;//as OSR=4, sampling on both edges of baud rate clock of received data

  UART0->C2 = UART_C2_TE_MASK | UART_C2_RE_MASK;

  /* Set Up Interrupts For UART0 */
  UART0_SetupReceiveINT();

  /* NVIC Set Priority */
  NVIC_SetPriority(UART0_IRQn, priority);

  uint8_t t[] = "start transfer";
  UART0_OutString(t, sizeof(t));
}
