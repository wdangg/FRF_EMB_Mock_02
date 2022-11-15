#include "inc.h"

#define RAM_ADDRESS                                     (0x20004000)
#define BOOT_ADDRESS_BASE                               (0x00000000)

#define APP_BASE                                        (0x0003F800)
#define ADR_BASE_SECTOR_4                               (uint32_t)(0x0003E800UL)
#define ADR_BASE_SECTOR_2                               (0x0003E800)


volatile uint32_t adrres_Sector4 = ADR_BASE_SECTOR_4;
volatile uint32_t adrres_Sector2 = ADR_BASE_SECTOR_2;
typedef void (*pFunction)(void);

/* Copy Boot's Vector Table To SRam */
void CopyFlashFuncToRam(void);

/*Jump To Application */
void Bootloader_jump_AppBilinkLed();

volatile uint32_t address = ADR_BASE_SECTOR_2;


volatile uint8_t data;
volatile uint32_t temp = 0;
volatile uint8_t count = 0;

volatile uint8_t enable_flash = 0;
volatile uint8_t enable_timeout_counting = 0;

volatile uint32_t system_timeout = 0;

int main()
{
  /* Disable Global Interrupts */
  __disable_irq();

	/* Config Clock, PORT Function and GPIO Moding */
	pinMode(&input_pin_PC3);
	pinMode(&ouput_led_PD5);
	pinMode(&ouput_led_PE29);
	pinMode(&input_pin_PC12);

	/* Attach Interrupts For Pin PC3 */
	attachExternalInterrupt(PORTC_PORTD_IRQn, &input_pin_PC3, FALLING_EDGE, 0);

	/* Attach Interrupts For Pin PC12 */
	attachExternalInterrupt(PORTC_PORTD_IRQn, &input_pin_PC12, FALLING_EDGE, 0);

  CopyFlashFuncToRam();

  /* Update system clock */
  SystemCoreClockUpdate();

  /* Attach PIT Interrupt */
  attachPITInterrupts(&pit_object_0);

  UART0_Init_115200(10);

  /* Erase Sector 4th */
  // Erase_Program(address);

  /* Enable Global Interrupts */
  __enable_irq();

  // Bootloader_jump_AppBilinkLed();


  while (1)
  {
    if (system_timeout > 20)
    {
      Bootloader_jump_AppBilinkLed();
    }

  }
  return 0;
}

void UART0_DriverIRQHandler()
{
  if ((UART0->S1 & (UART0_S1_RDRF_MASK)) == UART0_S1_RDRF_MASK)
  { 
    system_timeout = 0;

    data = CharToDec(UART0->D);

    if (data == '/')
    {
      enable_flash++;
      if (enable_flash > 1)
      {
        enable_flash = 0;
      }
      
    }    
    else if (data == 'c')
    {
      Erase_Program(address);
    }
    else if (data == 'j')
    {
      Bootloader_jump_AppBilinkLed();
    }
    else if ((data != INVALID_CHARACTER))
    {
      if (count == 0)
      {
        temp |= ((data*16) << 24);
        count++;
      }
      else if (count == 1)
      {
        temp |= (data << 24);
        count++;
      }
      else if (count == 2)
      {
        temp |= ((data*16) << 16);
        count++;
      }
      else if (count == 3)
      {
        temp |= (data << 16);
        count++;
      }
      else if (count == 4)
      {
        temp |= ((data*16) << 8);
        count++;
      }
      else if (count == 5)
      {
        temp |= (data << 8);
        count++;
      }
      else if (count == 6)
      {
        temp |= (data*16);
        count++;
      }
      else if (count == 7)
      {
        temp |= (data);
        if (enable_flash == 1)
        {
          Flash_Program(address, temp);
        }
        temp = 0;
        address += 0x4;
        count = 0;
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
    SCB->VTOR = 0x20004000;
    // NVIC_SystemReset();
}

void Bootloader_jump_AppBilinkLed()
{

    // typedef void (*pFunction)(void);// t?o 1 fn porter
    __disable_irq();
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;

    SCB->SHCSR &= ~(SCB_SHCSR_SVCALLPENDED_Msk);

    // SCB->VTOR =ADR_BASE_SECTOR_4;

    // pFunction l� bi?n ch? d�n h�m jumto

    pFunction JumpToApplication = NULL;
    uint32_t Application_Main_Stack_porter = 0;
    uint32_t Application_Reset_Handler = 0; // handler
    /* ----*/
    /* Initialize the user application Stack Pointe*/
    Application_Main_Stack_porter = *(volatile uint32_t *)(ADR_BASE_SECTOR_4);
    __set_MSP(Application_Main_Stack_porter);

    __DMB();
    //
    SCB->VTOR = ADR_BASE_SECTOR_4;
    __DSB();
    // SCB->SHCSR &= ~( SCB_SHCSR_SVCALLPENDED_Msk) ;
    /*jump to app*/

    void (*resethandler)(void) = (void (*)(void))(*(volatile uint32_t *)(ADR_BASE_SECTOR_4 + 0x04U));
    resethandler();
}

void PIT_IRQHandler_CallBack()
{
  GPIO_TogglePin(&ouput_led_PD5);
  system_timeout++;
}

void PORTC_PORTD_IRQHandler_Callback()
{
  GPIO_TogglePin(&ouput_led_PE29);
  delay_ms(100);
}