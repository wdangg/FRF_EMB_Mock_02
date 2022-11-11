#include "MKL46Z4.h"

#include "write_flash.h"


// flash Program
#define FTFA_CLEAR_ERROR                (0x30UL)
#define CMD_PROGRAM_LONGWORD            (0x06U)
#define CMD_ERASE_SECTOR                (0x09U)
#define START_COMMAND                    (0x80)


// flash erase

#define CMD_PROGRAM_ERSSCR              (0x09U)




/*write flash with 4 byte-*/
void Flash_Program(uint32_t Address, uint32_t Data){
  /* wait previous command complate*/
  while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0 );
    
  
  /*Check Error from previous command*/
  if((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK)==FTFA_FSTAT_ACCERR_MASK 
     || (FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK)==FTFA_FSTAT_FPVIOL_MASK)
  {
        FTFA->FSTAT = FTFA_CLEAR_ERROR ;
  }
  /*write command and parameter to FCCOB */
  FTFA->FCCOB0 = CMD_PROGRAM_LONGWORD;
  /*set address*/
  FTFA->FCCOB1 = (uint8_t) (Address >>16);
  FTFA->FCCOB2 = (uint8_t) (Address >>8);
  FTFA->FCCOB3 = (uint8_t) (Address);
  
  /*set data*/
  FTFA->FCCOB4 = (uint8_t) (Data);
  FTFA->FCCOB5 = (uint8_t) (Data >>8);
  FTFA->FCCOB6 = (uint8_t) (Data >>16);
  FTFA->FCCOB7 = (uint8_t) (Data >>24);
  /*clear CCIF Flag to launch command */
  FTFA->FSTAT = START_COMMAND   ;
  /*FsTAT =0x80*/
  
}

void Erase_Program(uint32_t Address)
{
   /* wait previous command complate*/
  while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0 );
  /*write command and parameter to FCCOB0*/
  FTFA->FCCOB0 = CMD_PROGRAM_ERSSCR ;
  /*set address*/
  FTFA->FCCOB1 = (uint8_t) (Address >>16);
  FTFA->FCCOB2 = (uint8_t) (Address >>8);
  FTFA->FCCOB3 = (uint8_t) (Address);
  
  //clear Clear CCIF;
  FTFA->FSTAT = START_COMMAND   ;
  
}







