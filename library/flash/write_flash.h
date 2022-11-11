#ifndef _WRITE_FLASH_H_
#define _WRITE_FLASH_H_
#include "MKL46Z4.h"

/*write flash*/
void Flash_Program(uint32_t Address, uint32_t Data);

/* Erase flash*/
void Erase_Program(uint32_t Address);

#endif /*WRITE_FLASH_H_*/
