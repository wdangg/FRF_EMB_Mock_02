#include "inc.h"

void CopyFlashFuncToRam(void);

#define RAM_ADDRESS                         (0x20004000)
#define BOOT_ADDRESS_BASE                   (0x00000000)

#define APP_BASE                            (0x0003F800)


volatile uint32_t address = APP_BASE;


volatile uint8_t data;
volatile uint32_t temp = 0;
volatile uint8_t count = 0;

volatile int enable_flash = 0;


int main()
{
  /* Disable Global Interrupts */
  __disable_irq();

  CopyFlashFuncToRam();

  /* Update system clock */
  SystemCoreClockUpdate();

  UART0_Init_9600();

  /* Erase Sector 4th */
  Erase_Program(0x0003F800);

  /* Enable Global Interrupts */
  __enable_irq();

  while (1)
  {


  }
  return 0;
}

void UART0_DriverIRQHandler()
{
  if ((UART0->S1 & (UART0_S1_RDRF_MASK)) == UART0_S1_RDRF_MASK)
  { 
    data = Char2Dec(UART0->D);

    if (data == '/')
    {
      enable_flash++;
      if (enable_flash > 1)
      {
        enable_flash = 0;
      }
    }    
    else if ((data != INVALID_CHARACTER))
    {
      if (count == 0)
      {
        temp |= ((data*16) << 24);
        count++;
        // Queue_ForcePushData(&queue, data);
      }
      else if (count == 1)
      {
        temp |= (data << 24);
        count++;
        // Queue_ForcePushData(&queue, data);
      }
      else if (count == 2)
      {
        temp |= ((data*16) << 16);
        count++;
        // Queue_ForcePushData(&queue, data);
      }
      else if (count == 3)
      {
        temp |= (data << 16);
        count++;
        // Queue_ForcePushData(&queue, data);
      }
      else if (count == 4)
      {
        temp |= ((data*16) << 8);
        count++;
        // Queue_ForcePushData(&queue, data);
      }
      else if (count == 5)
      {
        temp |= (data << 8);
        count++;
        // Queue_ForcePushData(&queue, data);
      }
      else if (count == 6)
      {
        temp |= (data*16);
        count++;
        // Queue_ForcePushData(&queue, data);
      }
      else if (count == 7)
      {
        temp |= (data);
        temp = 0;
        address += 0x4;
        count = 0;
        // Queue_ForcePushData(&queue, data);
      }
      if (enable_flash == 1)
      {
        Flash_Program(address, temp);
      }
    }

  }
  /* Clear Pending Interrupt */
  NVIC_ClearPendingIRQ(UART0_IRQn);
}

void CopyFlashFuncToRam(void)
{
  uint32_t size = 0x100, cntr;
  uint32_t *pDst = (uint32_t *)RAM_ADDRESS;
  uint32_t *pSrc = (uint32_t *)BOOT_ADDRESS_BASE;

  // func_size = (uint32_t)dummy_endfunc - (uint32_t)func_to_be_copied;
  for (cntr = 0; cntr < size; cntr++)
  {

    *pDst++ = *pSrc++;
  }
  SCB->VTOR = RAM_ADDRESS;
  // NVIC_SystemReset();
}