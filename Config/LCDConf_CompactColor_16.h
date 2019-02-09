/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2009  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.00 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : _InitController(void)
Purpose     : CompactColor driver configuration file
----------------------------------------------------------------------
*/

#ifndef LCDCONF_COMPACTCOLOR_16_H
#define LCDCONF_COMPACTCOLOR_16_H



/*********************************************************************
*       General configuration of LCD
*********************************************************************/
 
/* LCD Controller to use by emWin */
#define LCD_CONTROLLER      66702

/* LCD orientation settings */ 
#define LCD_MIRROR_X            0
#define LCD_MIRROR_Y            1
#define LCD_SWAP_XY             1

/* Color depths - bits per pixel and bit order */
#define LCD_BITSPERPIXEL        16
#define LCD_SWAP_RB             1

/* 16 bit parallel interface is used */
#define LCD_USE_PARALLEL_16     1


//
// Indirect interface configuration
//
void LCD_X_Write01_16 (unsigned short c);
void LCD_X_Write00_16 (unsigned short c);
void LCD_X_WriteM01_16(unsigned short * pData, int NumWords);
void LCD_X_WriteM00_16(unsigned short * pData, int NumWords);
void LCD_X_ReadM01_16 (unsigned short * pData, int NumWords);

#define LCD_WRITE_A1 (Word) LCD_X_Write01_16(Word)
#define LCD_WRITE_A0 (Word) LCD_X_Write00_16(Word)
#define LCD_WRITEM_A1(Word, NumWords) LCD_X_WriteM01_16(Word, NumWords)
#define LCD_WRITEM_A0(Word, NumWords) LCD_X_WriteM00_16(Word, NumWords)
#define LCD_READM_A1 (Word, NumWords) LCD_X_ReadM01_16 (Word, NumWords)
#endif /* LCDCONF_COMPACTCOLOR_16_H */

/*************************** End of file ****************************/
