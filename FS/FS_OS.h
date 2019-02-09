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
File        : FS_OS.h
Purpose     : File system's OS Layer header file
---------------------------END-OF-HEADER------------------------------
*/

#ifndef FS_OS_H             // Avoid recursive and multiple inclusion
#define FS_OS_H

#include "FS_ConfDefaults.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

void FS_X_OS_Lock   (unsigned LockIndex);
void FS_X_OS_Unlock (unsigned LockIndex);

void FS_X_OS_Init   (unsigned NumLocks);
void FS_X_OS_DeInit (void);

U32  FS_X_OS_GetTime(void);
void FS_X_OS_Delay  (int ms);

int  FS_X_OS_Wait   (int Timeout);
void FS_X_OS_Signal (void);

#if defined(__cplusplus)
}                /* Make sure we have C-declarations in C++ programs */
#endif

#endif // __FS_OS_H__

/*************************** End of file ****************************/
