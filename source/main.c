#include "inc.h"

#define RAM_ADDRESS                         (0x20004000)
#define BOOT_ADDRESS_BASE                   (0x00000000)

#define QUEUE_SIZE                          4
#define APP_BASE                            (0x0003F800)

volatile uint32_t address = APP_BASE;

volatile uint16_t value;
uint32_t data_arr[QUEUE_SIZE] = {0};  
volatile uint8_t data;
volatile uint32_t temp = 0;
volatile uint8_t count = 0;

Queue_Types queue = 
{
  0, -1, QUEUE_SIZE, /* Front, Rear, Size */
  0,
  &data_arr[0]
};

int main()
{
    /* Disable Global Interrupts */
  __disable_irq();

  CopyFlashFuncToRam();

  /* Update system clock */
	SystemCoreClockUpdate();

  UART0_Init_9600();
  

  uint8_t t[] = "start transfer";

  /* Enable Global Interrupts */
  __enable_irq();

  UART0_OutString(t, sizeof(t));

  /* Erase Sector 4th */
  Erase_Program(0x0003F800);


  while(1)
  {
  }
  return 0;
}

void UART0_DriverIRQHandler()
{
    /* Application Code */
    if ( (UART0->S1 & (UART0_S1_RDRF_MASK)) == UART0_S1_RDRF_MASK)
    { /* ] */
      data = Char2Dec(UART0->D);
      if ( count == 0)
      {
        temp |= (data<<24);
        count++;
        UART0_Transmit(data);
        Queue_ForcePushData(&queue, (uint32_t)data);
      }
      else if (count == 1)
      {
        temp |= (data<<16);
        count++;
        UART0_Transmit(data);
        Queue_ForcePushData(&queue, (uint32_t)data);
      }
      else if (count == 2)
      {
        temp |= (data<<8);
        count++;
        UART0_Transmit(data);
        Queue_ForcePushData(&queue, (uint32_t)data);
      }
      else if (count == 3)
      {
        temp |= (data<<0);
        UART0_Transmit(data);
        Queue_ForcePushData(&queue, (uint32_t)data );
        Flash_Program(address, temp);
        address += 0x4;

        count= 0;
        temp = 0;
      }
    }
    /* Clear Pending Interrupt */
    NVIC_ClearPendingIRQ(UART0_IRQn);
}

void CopyFlashFuncToRam(void)
{
   uint32_t size = 0x100, cntr;
   uint32_t *pDst = (uint32_t*)RAM_ADDRESS ;
   uint32_t *pSrc = (uint32_t*)BOOT_ADDRESS_BASE;
   
   
   //func_size = (uint32_t)dummy_endfunc - (uint32_t)func_to_be_copied;
   for(cntr = 0; cntr < size; cntr++)
   {
     
      *pDst++ = *pSrc++; 
   }   
   SCB->VTOR = RAM_ADDRESS; 
   //NVIC_SystemReset();
}