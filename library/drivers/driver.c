#include "driver.h"

uint8_t getPortSource(PORT_Type *PORTx)
{
    uint8_t source = (((uint32_t)PORTx - PORTA_BASE) / (PORTB_BASE - PORTA_BASE));
    return source;
}

Bool PORT_Init(PORT_Type *PORTx)
{
    Bool retVal = e_FALSE;

    if ((PORTA_BASE <= (uint32_t)PORTx) && ((uint32_t)PORTx <= PORTE_BASE))
    {
        retVal = e_TRUE;
        uint8_t port_source = getPortSource(PORTx);

        switch (port_source)
        {
        case PORTA_CLOCK:
            if ((SIM->SCGC5 & SIM_SCGC5_PORTA_MASK) == 0)
            {
                SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
            }
            break;
        case PORTB_CLOCK:
            if ((SIM->SCGC5 & SIM_SCGC5_PORTB_MASK) == 0)
            {
                SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
            }
            break;
        case PORTC_CLOCK:
            if ((SIM->SCGC5 & SIM_SCGC5_PORTC_MASK) == 0)
            {
                SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
            }
            break;
        case PORTD_CLOCK:
            if ((SIM->SCGC5 & SIM_SCGC5_PORTD_MASK) == 0)
            {
                SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
            }
            break;
        case PORTE_CLOCK:
            if ((SIM->SCGC5 & SIM_SCGC5_PORTE_MASK) == 0)
            {
                SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
            }
            break;
        default:
            retVal = e_FALSE;
            break;
        }
    }
    if (retVal == e_FALSE)
    {
        ERROR_Handler();
    }
    return retVal;
}

Bool pinMode(Object_Type *s_pin_config)
{
    Bool retVal = e_TRUE;

    /* Enable Clock For PORTx */
    PORT_Init(s_pin_config->PORTx);

    /* Set Direction For Pin */
    switch (s_pin_config->pin_moding)
    {
    case INPUT_PULLUP:
        /* Config Pin As GPIO Mode */
        (s_pin_config->PORTx)->PCR[s_pin_config->pin_number] &= (~PORT_PCR_MUX_MASK);
        (s_pin_config->PORTx)->PCR[s_pin_config->pin_number] |= PORT_PCR_MUX(GPIO_MODE);

        /* Config Pin Direction As Input */
        (s_pin_config->FGPIOx)->PDDR &= (~(1 << (s_pin_config->pin_number)));

        /* Pull Enable and Pull Up Pin */
        (s_pin_config->PORTx)->PCR[s_pin_config->pin_number] &= (~PE_PS_MASK);
        (s_pin_config->PORTx)->PCR[s_pin_config->pin_number] |= (s_pin_config->pin_moding);
        break;

    case OUTPUT:
        /* Config Pin As GPIO */
        (s_pin_config->PORTx)->PCR[s_pin_config->pin_number] &= (~PORT_PCR_MUX_MASK);
        (s_pin_config->PORTx)->PCR[s_pin_config->pin_number] |= PORT_PCR_MUX(OUTPUT);

        /* Config Pin Direction As Output */
        (s_pin_config->FGPIOx)->PDDR &= (~(1 << s_pin_config->pin_number));
        (s_pin_config->FGPIOx)->PDDR |= (1 << s_pin_config->pin_number);

        /* Set Initial State */
        (s_pin_config->FGPIOx)->PCOR |= (1 << (s_pin_config->pin_number));
        break;

    default:
        retVal = e_FALSE;
        break;
    }

    if (retVal == e_FALSE)
    {
        ERROR_Handler();
    }
    return retVal;
}

Bool attachExternalInterrupt(IRQn_Type IRQn_x, Object_Type *digitalPinToInterrupts, INT_Trigger_Type trigger_type)
{
    Bool retVal = e_TRUE;

    /* Disable Global Interrupts */
    __disable_irq();

    /* Setup Trigger Type For Interrupts */
    (digitalPinToInterrupts->PORTx)->PCR[digitalPinToInterrupts->pin_number] &= (~PORT_PCR_IRQC_MASK);
    (digitalPinToInterrupts->PORTx)->PCR[digitalPinToInterrupts->pin_number] |= PORT_PCR_IRQC(trigger_type);

    /* Enable IRQn_x Interrupts */
    NVIC_EnableIRQ(IRQn_x);

    /* Enable Global Interrupts */
    __enable_irq();

    return retVal;
}

Bool isPinInterruptsRequest(Object_Type *check_pin_request)
{
    Bool retVal = e_FALSE;

    if (((check_pin_request->PORTx)->ISFR) & (1 << (check_pin_request->pin_number)))
    {
        retVal = e_TRUE;
    }

    return retVal;
}

Bool PORT_EXTI_ClearFlag(Object_Type *pin_flag)
{
    Bool retVal = e_TRUE;

    ((pin_flag->PORTx)->ISFR) |= (1 << (pin_flag->pin_number));

    return retVal;
}

void ERROR_Handler()
{
    while (1)
    {
        /* Process Has Been Die */
    }
}
