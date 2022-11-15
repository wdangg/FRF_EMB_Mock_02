#include "pit_handle.h"

Bool attachPITInterrupts(PIT_Object_Type *pit_object)
{
  Bool retVal = e_TRUE;
  
  //   /* Disable Global Interrupts */
  // __disable_irq();

  /* Clock Gate Control For PIT Module */
  SIM->SCGC6 |= SIM_SCGC6_PIT(PIT_ACTIVE);

  /* Enable PIT Module */
  PIT->MCR &= (~PIT_MCR_MDIS_MASK);

  /* Timmer Are Stop/Continue In Debug Mode */
  if ((pit_object->freeze_option) == e_FALSE)
  {
    PIT->MCR |= PIT_MCR_FRZ_MASK;
  }
  else
  {
    PIT->MCR &= (~PIT_MCR_FRZ_MASK);
  }

  /* Turn On PIT */
  PIT->MCR = 0x00;

  /* Select Timeout Period For The Timmer Interrupts */
  (PIT->CHANNEL[pit_object->e_channel]).LDVAL = (pit_object->load_value);

  /* Enable Timmer Interrupts */
  PIT->CHANNEL[pit_object->e_channel].TCTRL |= PIT_TCTRL_TIE(pit_object->interrupts_option);

  /* Enable Timmer */
  PIT->CHANNEL[pit_object->e_channel].TCTRL |= PIT_TCTRL_TEN(PIT_TIMMER_ENABLE);

	/* Set Priority */
	NVIC_SetPriority(PIT_IRQn, pit_object->priority);

	/* Enable Interrupts NVIC */
  NVIC_EnableIRQ(PIT_IRQn);

  // /* Enable Global Interrupts */
  // __enable_irq();
  
  return retVal;
}

Bool checkPITFlag(PIT_Object_Type *pit_object_flag)
{
    Bool retVal = e_FALSE;

    if (((PIT->CHANNEL[pit_object_flag->e_channel]).TFLG) == e_TRUE)
    {
        retVal = e_TRUE;
    }

    return retVal;
}

Bool clearPITFlag(PIT_Object_Type *pit_flag)
{
    Bool retVal = e_TRUE;

    PIT->CHANNEL[pit_flag->e_channel].TFLG |= W1C;

    return retVal;
}

PIT_INT_Channel_Return checkPITInterruptChannelRequest()
{
	PIT_INT_Channel_Return pit_channel;

	if ((PIT->CHANNEL[e_PIT0_INT_Channel_Return].TFLG) == e_TRUE)
	{
		pit_channel = e_PIT0_INT_Channel_Return;
	}
	else if ((PIT->CHANNEL[e_PIT1_INT_Channel_Return].TFLG) == e_TRUE)
	{
		pit_channel = e_PIT1_INT_Channel_Return;
	}
	else
	{
		pit_channel = e_NONE_INT_PIT_RETURN;
	}

	return pit_channel;
}

Bool clearPITChannelFlag(PIT_INT_Channel_Return e_channel)
{
    Bool retVal = e_TRUE;

    PIT->CHANNEL[e_channel].TFLG |= W1C;

    return retVal;	
}

void PIT_IRQHandler()
{
	/* Check PIT Interrupt Source */
	PIT_INT_Channel_Return pit_channel_request =  checkPITInterruptChannelRequest();

	/* Clear Interrupts Flag */
	clearPITChannelFlag(pit_channel_request);

	/* NVIC Clear Pending */
	NVIC_ClearPendingIRQ(PIT_IRQn);

	/* Callback Function */
	PIT_IRQHandler_CallBack();
}

