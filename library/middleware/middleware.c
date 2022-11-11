#include "middleware.h"

void delay_ms(uint32_t us_time)
{
    uint32_t i, j;
    for (i = 0; i < us_time; i++)
    {
        for (j = 0; j < 6000; j++)
        {
        }
    }
}

Bool digitalRead(Object_Type *object_reading)
{
    Bool retVal = e_TRUE;

    int valRead = (((object_reading->FGPIOx)->PDIR) & (1 << (object_reading->pin_number)));

    if (valRead == 0)
    {
        retVal = e_FALSE;
    }
    else
    {
        retVal = e_TRUE;
    }

    return retVal;
}

void GPIO_TogglePin(Object_Type *object_toggle)
{
    ((object_toggle->FGPIOx)->PTOR) |= (1 << (object_toggle->pin_number));
}

void GPIO_ClearPin(Object_Type *object_clear)
{
    ((object_clear->FGPIOx)->PCOR) |= (1 << (object_clear->pin_number));
}

void GPIO_SetPin(Object_Type *object_set)
{
    ((object_set->FGPIOx)->PSOR) |= (1 << (object_set->pin_number));
}
