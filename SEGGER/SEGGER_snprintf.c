/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2016  SEGGER Microcontroller GmbH & Co. KG        *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       emFile * File system for embedded applications               *
*                                                                    *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product for in-house use.         *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       emFile version: V4.04a                                       *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File        : SEGGER_snprintf.c
Purpose     : Replacement for snprintf()
---------------------------END-OF-HEADER------------------------------
*/

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/

#include <stdlib.h>
#include <stdarg.h>
#include "SEGGER.h"

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _vsnprintf
*/
static int _vsnprintf(char *pBuffer, int BufferSize, const char *sFormat, va_list *pParamList) {
  SEGGER_BUFFER_DESC BufferDesc;
  unsigned NumDigits;
  I32      v;
  char     c;

  BufferDesc.pBuffer    = pBuffer;
  BufferDesc.BufferSize = BufferSize;
  BufferDesc.Cnt        = 0;

  do {
    c = *sFormat++;
    if (c == 0) {
      break;
    }
    if (c == '%') {
      //
      // Filter out trailing dot
      //
      if (*sFormat == '.') {
        sFormat++;
      }
      //
      // Filter out width specifier (number of digits)
      //
      NumDigits = 0;
      do {
        c = *sFormat++;
        if (c < '0' || c > '9') {
          break;
        }
        NumDigits = NumDigits * 10 + (c - '0');
      } while (1);
      //
      // Handle different qualifiers
      //
      do {
        if (c == 'l') {
          c = *sFormat++;
          continue;
        }
        break;
      } while (1);
      //
      // handle different types
      //
      v = va_arg(*pParamList, int);
      switch (c) {
      case 'c': {
        char c0 = (char)v;
        SEGGER_StoreChar(&BufferDesc, c0);
        break;
      }
      case 'd':
        SEGGER_PrintInt(&BufferDesc, v, 10, NumDigits);
        break;
      case 'u':
        SEGGER_PrintUnsigned(&BufferDesc, v, 10, NumDigits);
        break;
      case 'x':
      case 'X':
        SEGGER_PrintUnsigned(&BufferDesc, v, 16, NumDigits);
        break;
      case 's':
        {
          const char * s = (const char *)v;
          do {
            c = *s++;
            if (c == 0) {
              break;
            }
           SEGGER_StoreChar(&BufferDesc, c);
          } while (1);
        }
        break;
      case 'p':
        SEGGER_PrintUnsigned(&BufferDesc, v, 16, 8);
        break;
      }
    } else {
      SEGGER_StoreChar(&BufferDesc, c);
    }
  } while (1);
  //
  // Add trailing 0 to string
  //
  *(pBuffer + BufferDesc.Cnt) = 0;
  return BufferDesc.Cnt;
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       SEGGER_StoreChar
*/
void SEGGER_StoreChar(SEGGER_BUFFER_DESC *p, char c) {
  int Cnt;

  Cnt = p->Cnt;
  if ((Cnt + 1) < p->BufferSize) {
    *(p->pBuffer + Cnt) = c;
    p->Cnt = Cnt + 1;
  }
}

/*********************************************************************
*
*       SEGGER_PrintUnsigned
*/
void SEGGER_PrintUnsigned(SEGGER_BUFFER_DESC *pBufferDesc, U32 v, unsigned Base, int NumDigits) {
  static const char _aV2C[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
  unsigned Div;
  U32      Digit;

  Digit = 1;
  //
  // Compute Digit.
  // Loop until Digit has the value of the highest digit required.
  // Example: If the output is 345 (Base 10), loop 2 times until Digit is 100.
  //
  while (1) {
    if (NumDigits > 1) {       // User specified a min number of digits to print? => Make sure we loop at least that often, before checking anything else (> 1 check avoids problems with NumDigits being signed / unsigned)
      NumDigits--;
    } else {
      Div = v / Digit;
      if (Div < Base) {        // Is our divider big enough to extract the highest digit from value? => Done
        break;
      }
    }
    Digit *= Base;
  }
  //
  // Output digits
  //
  do {
    Div = v / Digit;
    v -= Div * Digit;
    SEGGER_StoreChar(pBufferDesc, _aV2C[Div]);
    Digit /= Base;
  } while (Digit);
  //
  // Add trailing 0 to string
  //
  *(pBufferDesc->pBuffer + pBufferDesc->Cnt) = 0;
}

/*********************************************************************
*
*       SEGGER_PrintInt
*/
void SEGGER_PrintInt(SEGGER_BUFFER_DESC *pBufferDesc, I32 v, unsigned Base, unsigned NumDigits) {
  //
  // Handle sign
  //
  if (v < 0) {
    v = -v;
    SEGGER_StoreChar(pBufferDesc, '-');
  }
  SEGGER_PrintUnsigned(pBufferDesc, v, Base, NumDigits);
}

/*********************************************************************
*
*       SEGGER_snprintf
*/
int SEGGER_snprintf(char *pBuffer, int BufferSize, const char *sFormat, ...) {
  va_list ParamList;

  va_start(ParamList, sFormat);
  return _vsnprintf(pBuffer, BufferSize, sFormat, &ParamList);
}

/*************************** End of file ****************************/
