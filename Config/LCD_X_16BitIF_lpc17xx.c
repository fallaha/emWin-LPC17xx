/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2012  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.16 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCD_X_16BitIF_STM32.c
Purpose     : Port routines for STM32 16-bit Interface
----------------------------------------------------------------------
*/

#include "GUI.h"

/*********************************************************************
*
*       Hardware configuration
*
**********************************************************************
*/

#include <stm32f10x.h>

#ifndef FSMC_Bank1_NORSRAM1     /* defined in ST Peripheral Library */
  #define FSMC_Bank1_NORSRAM1      ((uint32_t)0x00000000)
#endif

#define LCD_BASE        (0x60000000UL | 0x00000000UL)
#define LCD_REG16  (*((volatile unsigned short *)(LCD_BASE  ))) 
#define LCD_DAT16  (*((volatile unsigned short *)(LCD_BASE | 0x00020000UL)))

/*********************************************************************
*
*       Exported code
*
*********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Init
*
* Purpose:
*   This routine should be called from your application program
*   to set port pins to their initial values
*/
void LCD_X_Init(void) {

/* Configure the LCD Control pins --------------------------------------------*/
  RCC->APB2ENR |= 0x000001ED;                         /* enable GPIOA,D..G, AFIO clock */

  /* PD.00(D2),  PD.01(D3),  PD.04(NOE), PD.05(NWE), PD.07(NE4 (LCD/CS)) - CE3(LCD /CS) */ 
GPIOD->CRL &= ~0xF0FF00FF;                          /* clear Bits */
GPIOD->CRL |=  0xB0BB00BB;                          /* alternate function output Push-pull 50MHz */
  /* PD.08(D13), PD.09(D14), PD.10(D15), PD.11(RS) PD.14(D0), PD.15(D1) */
GPIOD->CRH &= ~0xFF00FFFF;                          /* clear Bits */
GPIOD->CRH |=  0xBB00BBBB;                          /* alternate function output Push-pull 50MHz */
   
  /* PE.07(D4) */ 
GPIOE->CRL &= ~0xF0000000;                          /* clear Bits */
GPIOE->CRL |=  0xB0000000;                          /* alternate function output Push-pull 50MHz */
  /* PE.08(D5), PE.09(D6),  PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10), PE.14(D11), PE.15(D12) */
GPIOE->CRH &= ~0xFFFFFFFF;                          /* clear Bits */
GPIOE->CRH |=  0xBBBBBBBB;                          /* alternate function output Push-pull 50MHz */

 

  /* PB.05(LCD Backlight */
  GPIOB->BRR |=  0x00000020;                          /* Backlight off */
  GPIOB->CRL &= ~0x00F00000;                          /* clear Bits */
  GPIOB->CRL |=  0x00300000;                          /*                    output Push-pull 50MHz */

/*-- FSMC Configuration ------------------------------------------------------*/
/*----------------------- SRAM Bank 4 ----------------------------------------*/
  RCC->AHBENR  |= (1<<8);                             /* enable FSMC clock */

  FSMC_Bank1->BTCR[FSMC_Bank1_NORSRAM1+1] =           /* Bank1 NOR/SRAM timing register configuration */
                                         (0 << 28) |  /* FSMC AccessMode A */
                                         (0 << 24) |  /* Data Latency */
                                         (0 << 20) |  /* CLK Division */
                                         (0 << 16) |  /* Bus Turnaround Duration */
                                         (2 <<  10) |  /* Data SetUp Time */
                                         (0 <<  4) |  /* Address Hold Time */
                                         (2 <<  10);   /* Address SetUp Time */
  FSMC_Bank1->BTCR[FSMC_Bank1_NORSRAM1  ] =           /* Control register */
                                         (0 << 19) |  /* Write burst disabled */
                                         (0 << 15) |  /* Async wait disabled */
                                         (0 << 14) |  /* Extended mode disabled */
                                         (0 << 13) |  /* NWAIT signal is disabled */ 
                                         (1 << 12) |  /* Write operation enabled */
                                         (0 << 11) |  /* NWAIT signal is active one data cycle before wait state */
                                         (0 << 10) |  /* Wrapped burst mode disabled */
                                         (1 <<  9) |  /* Wait signal polarity active low */
                                         (0 <<  8) |  /* Burst access mode disabled */
                                         (1 <<  4) |  /* Memory data  bus width is 16 bits */
                                         (0 <<  2) |  /* Memory type is SRAM */
                                         (0 <<  1) |  /* Address/Data Multiplexing disable */
                                         (1 <<  0);   /* Memory Bank enable */
 GPIOB->BSRR = 0x00000020;             /* Turn backlight on */
}

/*********************************************************************
*
*       LCD_X_Write00_16
*
* Purpose:
*   Write to controller, with A0 = 0
*/
void LCD_X_Write00_16(U16 c) {
  LCD_REG16 = c;
}

/*********************************************************************
*
*       LCD_X_Write01_16
*
* Purpose:
*   Write to controller, with A0 = 1
*/
void LCD_X_Write01_16(U16 c) {
  LCD_DAT16 = c;
}

/*********************************************************************
*
*       LCD_X_Read01_16
*
* Purpose:
*   Read from controller, with A0 = 1
*/
U16 LCD_X_Read01_16(void) {
  return (LCD_DAT16);
}

/*********************************************************************
*
*       LCD_X_WriteM01_16
*
* Purpose:
*   Write multiple bytes to controller, with A0 = 1
*/
void LCD_X_WriteM01_16(U16 * pData, int NumWords) {
  for (; NumWords; NumWords--) {
    LCD_DAT16 = *pData++;
  }
}
/*********************************************************************
*
*       LCD_X_WriteM00_16
*
* Purpose:
*   Write multiple bytes to controller, with A0 = 0
*/
void LCD_X_WriteM00_16(U16 * pData, int NumWords) {
  for (; NumWords; NumWords--) {
    LCD_REG16 = *pData++;
  }
}
/*********************************************************************
*
*       LCD_X_ReadM01_16
*
* Purpose:
*   Read multiple bytes from controller, with A0 = 1
*/
void LCD_X_ReadM01_16(U16 * pData, int NumWords) {
  for (; NumWords; NumWords--) {
    *pData++ = LCD_DAT16;
  }
}

/*************************** End of file ****************************/
