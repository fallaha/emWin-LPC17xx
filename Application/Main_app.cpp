#include "lpc17xx.h"
#include <stdlib.h>
#include "DIALOG.h"
#include "DIALOG.h"
#include "..\MouseKeyboard\KeyboardHost.h"

	WM_HWIN CreateUART(void);
	WM_HWIN Createtools(void);
	WM_HWIN CreateSDcard(void);
	WM_HWIN CreateShowFile(void);
	WM_HWIN Createexplorer(void);
	WM_HWIN CreateSdwin(void);
	/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0         (GUI_ID_USER + 0x00)
#define ID_BUTTON_0         (GUI_ID_USER + 0x01)
#define ID_BUTTON_1         (GUI_ID_USER + 0x02)
#define ID_BUTTON_2         (GUI_ID_USER + 0x03)
#define ID_TEXT_0         (GUI_ID_USER + 0x04)


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Firstpage", ID_FRAMEWIN_0, 21, 17, 277, 185, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED Tool", ID_BUTTON_0, 116, 36, 144, 26, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Work With UART", ID_BUTTON_1, 117, 68, 143, 28, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "SD card Work", ID_BUTTON_2, 117, 100, 144, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Operating System", ID_TEXT_0, 78, 6, 114, 20, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Firstpage'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleHeight(hItem, 14);
    //
    // Initialization of 'Operating System'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, GUI_FONT_13HB_1);
    TEXT_SetTextColor(hItem, 0x00808040);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'LED Tool'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Createtools();
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'Work With UART'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				CreateUART();
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'SD card Work'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				CreateSDcard();
				
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateFirstpage
*/
WM_HWIN CreateFirstpage(void);
WM_HWIN CreateFirstpage(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	FRAMEWIN_AddCloseButton(hWin,FRAMEWIN_BUTTON_RIGHT,1);
	FRAMEWIN_AddMaxButton(hWin,FRAMEWIN_BUTTON_RIGHT,1);
	FRAMEWIN_AddMinButton(hWin,FRAMEWIN_BUTTON_RIGHT,1);

	return hWin;
}



/*********************************************************************
*
*       _cbBkWindow
*/
static void _cbBkWindow(WM_MESSAGE * pMsg) {
  int xSize;
  int ySize;

  switch (pMsg->MsgId) {
  case WM_PAINT:
    xSize = LCD_GetXSize();
    ySize = LCD_GetYSize();
    GUI_SetColor(GUI_WHITE);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_DrawGradientV(0, 0, xSize, ySize, GUI_LIGHTBLUE, GUI_LIGHTRED);
	  GUI_SetFont(GUI_FONT_16B_1);
	GUI_DispStringAt("Ali Fallah :)",120,222);
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       CreateELECTRO 2014
*/
WM_HWIN CreateELECTRO2014(void) {
  WM_HWIN hWin;
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	FRAMEWIN_SetResizeable(hWin,1);
	FRAMEWIN_SetMoveable(hWin,1);
	FRAMEWIN_AddCloseButton(hWin,FRAMEWIN_BUTTON_RIGHT,1);
	FRAMEWIN_AddMaxButton(hWin,FRAMEWIN_BUTTON_RIGHT,1);
	FRAMEWIN_AddMinButton(hWin,FRAMEWIN_BUTTON_RIGHT,1);
  return hWin;
}



static void _Mous(WM_MESSAGE * pMsg) {
static int xPos, yPos,select=0;
  GUI_PID_STATE   State = {0};
  WM_HWIN         hWin;
  hWin  = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_TIMER:{
		
	static char ix=0;
	static char iy=0;

		
	static uint8_t ox;
	static uint8_t oy;
	static char but;
	
	uint16_t countetme=0;
	for (countetme=0;countetme<10;countetme++){
	KeyboardHost_Task(&ix,&iy,&but);
	USB_USBTask();
		}
	if (ox!=ix)
		{
			ox=ix;
	if (ix>128){
		ix=256-ix;
		xPos-=ix;
	}
	else 
		xPos+=ix;
	
}

if (oy!=iy)
	{
		oy=iy;
	if (iy>128){
		iy=256-iy; 
		yPos-=iy;
	}
	else 
		yPos+=iy;


}	
	   
		 if(xPos<0)xPos=0;
		 if(yPos<0)yPos=0;
		 if(xPos>320)xPos=320;
		 if(yPos>240)yPos=240;


		 if(but == 1)select=1;


		 else select=0;
    State.x = xPos;
    State.y = yPos;
    State.Pressed = select;
    GUI_PID_StoreState(&State);
    WM_RestartTimer(pMsg->Data.v,5);
    break;
  case WM_CREATE:
    WM_CreateTimer(hWin, 0,5, 0);
    break;}
  }
}
void MainTask(void);
void MainTask(void) {
  WM_HWIN hWin;
	WM_SetCreateFlags(WM_CF_MEMDEV);
 GUI_Init();
	GUI_CURSOR_Show();
	FRAMEWIN_SetDefaultFont(&GUI_Font13_1);
	
	 BUTTON_SetDefaultSkin   (BUTTON_SKIN_FLEX);
  CHECKBOX_SetDefaultSkin (CHECKBOX_SKIN_FLEX);
  DROPDOWN_SetDefaultSkin (DROPDOWN_SKIN_FLEX);
  FRAMEWIN_SetDefaultSkin (FRAMEWIN_SKIN_FLEX);
  HEADER_SetDefaultSkin   (HEADER_SKIN_FLEX);
  PROGBAR_SetDefaultSkin  (PROGBAR_SKIN_FLEX);
	
	
  RADIO_SetDefaultSkin    (RADIO_SKIN_FLEX);
  SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
  SLIDER_SetDefaultSkin   (SLIDER_SKIN_FLEX);
	SPINBOX_SetDefaultSkin  (&SPINBOX_SKIN_FLEX);
	
  WM_SetCallback(WM_HBKWIN, &_cbBkWindow);
  WM_CreateWindowAsChild(0, 0, 1, 1, WM_HBKWIN, 0, _Mous, 0);

// 
	CreateSDcard();
	CreateFirstpage();
  while (1) {
		GUI_Delay(100);	
  }
}

/**************************** end of file ***************************/
