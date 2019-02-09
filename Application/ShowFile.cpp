/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.26                          *
*        Compiled Aug 18 2014, 17:12:05                              *
*        (c) 2014 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END
#include <RTL.h>                      /* RTL kernel functions & defines      */
#include <stdio.h>                    /* standard I/O .h-file                */
#include <string.h>                   /* string and memory functions         */

#include "DIALOG.h"



/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0    (GUI_ID_USER + 0x00)
#define ID_LISTVIEW_0    (GUI_ID_USER + 0x02)


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

	WM_HWIN ID_LISTVIEW_0_hItem;
	

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "ShowFile", ID_FRAMEWIN_0, 0, 0, 320, 240, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 0, 0, 310, 225, 0, 0x0, 0 },
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
    // Initialization of 'Listview'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
		ID_LISTVIEW_0_hItem=hItem;
	
    LISTVIEW_AddColumn(hItem, 120, "Name", GUI_TA_LEFT | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 60, "Size", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 60, "type", GUI_TA_HCENTER | GUI_TA_VCENTER);

    LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_AddRow(hItem, NULL);	
	LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_AddRow(hItem, NULL);
    LISTVIEW_SetGridVis(hItem, 1);
    LISTVIEW_AddColumn(hItem, 70, "Date", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_SetAutoScrollV(hItem, 1);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_LISTVIEW_0: // Notifications sent by 'Listview'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
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
*       CreateShowFile
*/
WM_HWIN CreateShowFile(void);
WM_HWIN CreateShowFile(void) {
  WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	
	unsigned int a=0,column=0,row=0;
	
	FINFO info;
	finit("M0");
	info.fileID=0;
	char size[8];
	char time[15];
	for (int counter=0;counter<50;counter++){
			ffind("*",&info);
		if(info.fileID>a){
		a=info.fileID;

		LISTVIEW_SetItemText(ID_LISTVIEW_0_hItem,0,row,(char *)info.name);
			if (info.size>1024)
		sprintf(size,"%d KB",info.size/1024);
			else if (info.size>1024*1024)
				sprintf(size,"%d MB",info.size/1024/1024);
			else
				sprintf(size,"%d b",info.size);
			
		LISTVIEW_SetItemText(ID_LISTVIEW_0_hItem,1,row,size);
			sprintf(time,"%d MB",info.time.year);
		LISTVIEW_SetItemText(ID_LISTVIEW_0_hItem,3,row,time);
			
		row++;
		}
	}
	
	
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/