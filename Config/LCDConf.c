/*********************************************************************
*                SEGGER MICROCONTROLLER GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*               arvin2009
**                                 http://www.didbansanat.com                                                       *
*        (c) 2003-2012     SEGGER Microcontroller GmbH & Co KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "GUIDRV_FlexColor.h"

//#include "touch/Touch.h"


#define XSIZE_PHYS  240
#define YSIZE_PHYS  320


#define COLOR_CONVERSION  GUICC_M565


#define DISPLAY_DRIVER  GUIDRV_FLEXCOLOR


/*********************************************************************
*
*     
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION
  #error Color conversion not defined!
#endif
#ifndef   DISPLAY_DRIVER
  #error No display driver defined!
#endif

/*********************************************************************

 *****************************************************************************************************************/
static void _LcdSetReg(U16 Data) {
LCD_WriteIndex(Data);
}

/**************************************************************************************************************
 
 *****************************************************************************************************************/
static void _LcdWriteData(U16 Data) {
LCD_WriteData(Data);
}
/**************************************************************************************************************
 
 *****************************************************************************************************************/
static void _LcdWriteDataMultiple(U16 * pData, int NumItems) {
  while (NumItems--) {
    LCD_WriteData(*pData);
		pData++;
  }
}

/**************************************************************************************************************
  
 *****************************************************************************************************************/
static void _LcdReadDataMultiple(U16 * pData, int NumItems) {
  while (NumItems--) {
   *pData = LCD_ReadData();
		pData++;
  }
}

/**************************************************************************************************************
       
 *****************************************************************************************************************/
static void _WriteReg(U16 Reg, U16 Data) {
  _LcdSetReg   (Reg);
  _LcdWriteData(Data);
}

/**************************************************************************************************************
     
 *****************************************************************************************************************/
static void _InitLcdController(void) {
  LCD_Initializtion();
	while(0){_WriteReg(4,4);} //make compiler happy
}

/**************************************************************************************************************
 
 *****************************************************************************************************************/
static void _Init(void) {
		//touch_SPI_Configuration();
  _InitLcdController();
}


/**************************************************************************************************************
  
 *****************************************************************************************************************/
void LCD_X_Config(void) {
  GUI_DEVICE * pDevice;
  CONFIG_FLEXCOLOR Config = {0};
  GUI_PORT_API PortAPI = {0};
  //
  // 
  //
  pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  //
  // 
  //
  LCD_SetSizeEx (0, XSIZE_PHYS , YSIZE_PHYS);
  LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);
  //
  //
  Config.Orientation =  GUI_SWAP_XY |GUI_MIRROR_Y;// GUI_MIRROR_X;//
  Config.RegEntryMode=0x6870;
//	Config.FirstSEG=0;
//	Config.FirstCOM=0;
  GUIDRV_FlexColor_Config(pDevice, &Config);
  //
  //
  PortAPI.pfWrite16_A0  = _LcdSetReg;
  PortAPI.pfWrite16_A1  = _LcdWriteData;
  PortAPI.pfWriteM16_A1 = _LcdWriteDataMultiple;
  PortAPI.pfReadM16_A1  = _LcdReadDataMultiple;
  GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66702, GUIDRV_FLEXCOLOR_M16C0B16);
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Function description:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  (void) LayerIndex;
  (void) pData;

  switch (Cmd) {
  //
  // Required
  //
  case LCD_X_INITCONTROLLER: {
    //
    // Called during the initialization process in order to set up the
    // display controller and put it into operation. If the display
    // controller is not initialized by any external routine this needs
    // to be adapted by the customer...
    //
    // ...
    _Init();
		

    return 0;
  }
  default:
    r = -1;
  }
  return r;
}

/*************************** End of file ****************************/

