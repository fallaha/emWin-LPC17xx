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
File        : FS_ConfigIO.c
Purpose     : I/O configuration routines for file system
---------------------------END-OF-HEADER------------------------------
*/

#include <stdio.h>
#include "FS.h"

#if defined (__CROSSWORKS_ARM)
  #include "__putchar.h"
#endif

/*********************************************************************
*
*       Defines, configurable
*/
#ifndef   USE_RTT
  #define USE_RTT             0   // I/O via SEGGER's Real Time Transfer: https://www.segger.com/jlink-real-time-terminal.html
#endif

#ifndef   USE_SYSTEMVIEW
  #define USE_SYSTEMVIEW      0   // I/O via SEGGER's System View: https://www.segger.com/systemview.html
#endif

#ifndef   USE_DCC
  #define USE_DCC             0   // I/O via SEGGER's J-Link DCC.
#endif

#ifndef   USE_EMBOS_VIEW
  #define USE_EMBOS_VIEW      0   // I/O via SEGGER's embOSView PC tool.
#endif

/*********************************************************************
*
*       Includes, conditional
*/
#if USE_RTT
  #include "SEGGER_RTT.h"
#elif USE_SYSTEMVIEW
  #include "SEGGER_SYSVIEW.h"
#elif USE_DCC
  #include "JLINKDCC.h"
#elif USE_EMBOS_VIEW
  #include "RTOS.h"
#endif

/*********************************************************************
*
*       _puts
*
*  Function description
*    Local (static) replacement for puts.
*    The reason why puts is not used is that puts always appends a NL
*    character, which screws up our formatting.
*/
static void _puts(const char * s) {
#if USE_EMBOS_VIEW
  OS_SendString(s);
#elif USE_RTT
  SEGGER_RTT_WriteString(0, s);
#elif USE_SYSTEMVIEW
  SEGGER_SYSVIEW_Print(s);
#else
  for (;;) {
    char c;

    c = *s++;
    if (c == 0) {
      break;
    }
#if USE_DCC
    JLINKDCC_SendChar(c);
#else
    putchar(c);
#endif
  }
#endif
}

/*********************************************************************
*
*       FS_X_Panic
*
*  Function description
*    Referred in debug builds of the file system only and
*    called only in case of fatal, unrecoverable errors.
*/
void FS_X_Panic(int ErrorCode) {
  char ac[20];
  SEGGER_BUFFER_DESC BufferDesc;

  BufferDesc.pBuffer    = ac;
  BufferDesc.BufferSize = sizeof(ac);
  BufferDesc.Cnt        = 0;
  SEGGER_PrintInt(&BufferDesc, ErrorCode, 10, 0);
  _puts("FS panic: ");
  _puts(ac);
  _puts("\n");
  while (1) {
    ;
  }
}

/*********************************************************************
*
*      Logging: OS dependent
*
*  Note
*       Logging is used in higher debug levels only. The typical target
*       build does not use logging and does therefore not require any of
*       the logging routines below. For a release build without logging
*       the routines below may be eliminated to save some space.
*       (If the linker is not function aware and eliminates unreferenced
*       functions automatically)
*/
void FS_X_Log(const char * s) {
  _puts(s);
}

void FS_X_Warn(const char * s) {
  _puts("FS warning: ");
  _puts(s);
  _puts("\n");
}

void FS_X_ErrorOut(const char * s) {
  _puts("FS error: ");
  _puts(s);
  _puts("\n");
}

/*************************** End of file ****************************/
