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
File    : SEGGER_memcpy.c
Purpose : Optimized memcpy replacement, since some memcpy routines in libraries
          are extremely poor.
          This code is not optimized for a particular processor;
          in most cases, a routine optimized for the processor used and hand-coded
          in assembly language will offer even better performance.
---------------------------END-OF-HEADER------------------------------
*/

#include "SEGGER.h"


/*********************************************************************
*
*       Public code
*
**********************************************************************
*/


/*********************************************************************
*
*       SEGGER_memcpy();
*
* Function description
*   Optimized replacement for memcpy
*/
void SEGGER_memcpy(void * pDest, const void * pSrc, int NumBytes) {
  char * pd;
  char * ps;
  pd = (char*)pDest;
  ps = (char*)pSrc;


  //
  // Copy bytes until Source is word aligned
  //
  do {
    if (NumBytes == 0) {
      return;
    }
    if (((int)ps & 3) == 0) {
      break;
    }
    *(char*)pd++ = *(char*)ps++;
    NumBytes--;
  } while (1);
  //
  // Copy words if possible (destination is also word aligned)
  //
  if (((int)pd & 3) == 0) {
    unsigned NumWords = NumBytes >> 2;
    while (NumWords >= 4) {
      *(U32*)pd = *(U32*)ps;
      pd += 4;
      ps += 4;
      *(U32*)pd = *(U32*)ps;
      pd += 4;
      ps += 4;
      *(U32*)pd = *(U32*)ps;
      pd += 4;
      ps += 4;
      *(U32*)pd = *(U32*)ps;
      pd += 4;
      ps += 4;
      NumWords -= 4;
    }
    if (NumWords) {
      do {
        *(U32*)pd = *(U32*)ps;
        pd += 4;
        ps += 4;
      } while (--NumWords);
    }
    NumBytes &= 3;
  }
  //
  // Copy half-words if possible (destination is also half-word aligned)
  //
  if (((int)pd & 1) == 0) {
    unsigned NumItems = NumBytes >> 1;
    while (NumItems >= 4) {
      *(U16*)pd = *(U16*)ps;
      pd += 2;
      ps += 2;
      *(U16*)pd = *(U16*)ps;
      pd += 2;
      ps += 2;
      *(U16*)pd = *(U16*)ps;
      pd += 2;
      ps += 2;
      *(U16*)pd = *(U16*)ps;
      pd += 2;
      ps += 2;
      NumItems -= 4;
    }
    if (NumItems) {
      do {
      *(U16*)pd = *(U16*)ps;
      pd += 2;
      ps += 2;
      } while (--NumItems);
    }
    NumBytes &= 1;
  }

  //
  // Copy bytes, bulk
  //
  while (NumBytes >= 4) {
    *(char*)pd++ = *(char*)ps++;
    *(char*)pd++ = *(char*)ps++;
    *(char*)pd++ = *(char*)ps++;
    *(char*)pd++ = *(char*)ps++;
    NumBytes -= 4;
  };
  //
  // Copy bytes, one at a time
  //
  if (NumBytes) {
    do {
      *(char*)pd++ = *(char*)ps++;
    } while (--NumBytes);
  };
}


/*************************** end of file ****************************/
