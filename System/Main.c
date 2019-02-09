/*********************************************************************
*                SEGGER MICROCONTROLLER GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 2003-2012     SEGGER Microcontroller GmbH & Co KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : Main.c
Purpose : Calls hardware initialization and application.
--------  END-OF-HEADER  ---------------------------------------------
*/
#include "..\MouseKeyboard\KeyboardHost.h"
#include "lpc17xx.h"
#ifndef _WINDOWS
#include "HWConf.h"
#endif

#ifdef __CROSSWORKS_ARM
extern void __low_level_init(); // hwconf.c
#endif

void MainTask(void);  // Defined in SEGGERDEMO.c

/*********************************************************************
*
*       main
*/
int main(void) {
LPC_GPIO1->FIODIR = 0x00000000;
LPC_GPIO2->FIODIR = 0x000000ff;

	SetupHardware();

//LPC_GPIO2->FIOSET=(1<<0);
  #ifdef __CROSSWORKS_ARM
  __low_level_init();
  #endif
  #ifndef _WINDOWS
  HW_X_Config();      // Initialization of Hardware
  #endif
  MainTask();         // emWin application
}




