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
File        : FS_Debug.h
Purpose     : Debug macros
----------------------------------------------------------------------
   Debug macros for logging

 In the FS Simulation, all output is transferred into the log window.
*/

#ifndef __FS_DEBUG_H__               // Avoid recursive and multiple inclusion
#define __FS_DEBUG_H__


#include <stdarg.h>
#include "SEGGER.h"
#include "FS_ConfDefaults.h"


#define FS_DEBUG_LEVEL_NOCHECK       0  // No run time checks are performed
#define FS_DEBUG_LEVEL_CHECK_PARA    1  // Parameter checks are performed to avoid crashes
#define FS_DEBUG_LEVEL_CHECK_ALL     2  // Parameter checks and consistency checks are performed
#define FS_DEBUG_LEVEL_LOG_ERRORS    3  // Errors are recorded
#define FS_DEBUG_LEVEL_LOG_WARNINGS  4  // Errors & Warnings are recorded
#define FS_DEBUG_LEVEL_LOG_ALL       5  // Errors, Warnings and Messages are recorded.

#ifndef FS_DEBUG_LEVEL
  #ifdef WIN32
    #define FS_DEBUG_LEVEL FS_DEBUG_LEVEL_LOG_WARNINGS    // Simulation should log all warnings
  #else
    #define FS_DEBUG_LEVEL FS_DEBUG_LEVEL_CHECK_PARA      // For most targets, min. size is important
  #endif
#endif

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Logging (for debugging primarily)
*
**********************************************************************
*/

void FS_AddWarnFilter (U32 FilterMask);
void FS_SetWarnFilter (U32 FilterMask);
U32  FS_GetWarnFilter (void);

void FS_AddLogFilter  (U32 FilterMask);
void FS_SetLogFilter  (U32 FilterMask);
U32  FS_GetLogFilter  (void);

void FS_AddErrorFilter(U32 FilterMask);
void FS_SetErrorFilter(U32 FilterMask);
U32  FS_GetErrorFilter(void);

void FS_PrintfSafe(char * pBuffer, const char * sFormat, int BufferSize, va_list * pParamList);
void FS_ErrorOutf(U32 Type, const char * sFormat, ...);
void FS_Logf     (U32 Type, const char * sFormat, ...);
void FS_Warnf    (U32 Type, const char * sFormat, ...);

/*******************************************************************
*
*       Error macros
*
********************************************************************
*/
#if FS_DEBUG_LEVEL >= FS_DEBUG_LEVEL_LOG_ERRORS
  #define FS_DEBUG_ERROROUT(s)              FS_ErrorOutf s
#else
  #define FS_DEBUG_ERROROUT(s)
#endif

/*******************************************************************
*
*       Warning macros
*
********************************************************************
*/
#if FS_DEBUG_LEVEL >= FS_DEBUG_LEVEL_LOG_WARNINGS
  #define FS_DEBUG_WARN(s)                  FS_Warnf s
#else
  #define FS_DEBUG_WARN(s)
#endif

/*******************************************************************
*
*       Logging macros
*
********************************************************************
*/
#if FS_DEBUG_LEVEL >= FS_DEBUG_LEVEL_LOG_ALL
  #define FS_DEBUG_LOG(s)                   FS_Logf s
#else
  #define FS_DEBUG_LOG(s)
#endif

/*******************************************************************
*
*       Asserts
*
********************************************************************
*/
#if FS_DEBUG_LEVEL >= FS_DEBUG_LEVEL_LOG_ERRORS
  #define FS_DEBUG_ASSERT(MType, Expr)        \
    do {                                      \
      if (!(Expr)) {                          \
        FS_DEBUG_ERROROUT ((MType, (#Expr))); \
        FS_X_Panic(FS_ERRCODE_ASSERT_FAILURE);\
      }                                       \
    } while (0)
#else
  #define FS_DEBUG_ASSERT(MType, Expr)
#endif

#if defined(__cplusplus)
  }              /* Make sure we have C-declarations in C++ programs */
#endif

#endif // __FS_DEBUG_H__

/*************************** End of file ****************************/
