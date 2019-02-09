/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2014  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.26 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : DIALOG_FS_ChooseFile.c
Purpose     : Shows how to make use of the CHOOSEFILE dialog.
              It shows the dialog with and without skinning.
              The button bar is shown at the bottom and at the top
              of the window.
Requirements: WindowManager - (x)
              MemoryDevices - (x)
              AntiAliasing  - ( )
              VNC-Server    - ( )
              PNG-Library   - ( )
              TrueTypeFonts - ( )

              Requires eithar a MS Windows environment or emFile!
----------------------------------------------------------------------
*/

#ifndef SKIP_TEST

	#include <RTL.h>
	#include <stdio.h>
	#include <string.h>
	#include <stdint.h>
	#include "..\FS\FS.h"
#include "DIALOG.h"
#include "MESSAGEBOX.h"
#include "..\MouseKeyboard\KeyboardHost.h"






#ifndef _MAX_PATH
  #define _MAX_PATH 256
#else
  #if (_MAX_PATH > 256)
    #undef  _MAX_PATH
    #define _MAX_PATH 256
  #endif
#endif

#ifndef _MAX_EXT
  #define _MAX_EXT  256
#endif

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static void _EnableSkinning(int OnOff);
static void _Mous(WM_MESSAGE * pMsg);
static int _GetData(CHOOSEFILE_INFO * pInfo);
static struct {
  U32 Mask;
  char c;
} _aAttrib[] = {

  { FS_ATTR_READ_ONLY, 'R' },
  { FS_ATTR_HIDDEN   , 'H' },
  { FS_ATTR_SYSTEM   , 'S' },
  { FS_ATTR_DIRECTORY, 'D' },
  { FS_ATTR_ARCHIVE  , 'A' },

};

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0    (GUI_ID_USER + 0x00)

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "SD", ID_FRAMEWIN_0, 0, -1, 325, 210, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};


/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {

  int          NCode;
  int          Id;
	
	const char        acMask[]            = "*.*";
  CHOOSEFILE_INFO   Info                = { 0 };
  WM_HWIN           hWin;
  int               xSize;
  int               ySize;
  int               r;
  char              acDir[_MAX_PATH]    = "M:";
  char const      * apDrives[1]         = { acDir };
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
		
  finit ("M0");
  WM_SetCreateFlags(WM_CF_MEMDEV);
  WM_SetDesktopColor(GUI_WHITE);
  //
  // Set defaults for FRAMEWIN widget
  //
  FRAMEWIN_SetDefaultFont(GUI_FONT_20_ASCII);
  FRAMEWIN_SetDefaultTextAlign(GUI_TA_HCENTER);
  //
  // Get display size and initialize pointers to search mask and callback routine
  //
  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();
  Info.pfGetData = _GetData;
  Info.pMask     = acMask;
	_EnableSkinning(1);
	CHOOSEFILE_SetTopMode(1);
	
	 _start:
    
    hWin = CHOOSEFILE_Create(WM_HBKWIN, 0, 0, xSize, ySize, apDrives, GUI_COUNTOF(apDrives), 0, "File", 0, &Info);


    r = GUI_ExecCreatedDialog(hWin);
    if (r == 0) {
      hWin = MESSAGEBOX_Create(Info.pRoot, "File:", 0);
      GUI_Delay(2000);
      WM_DeleteWindow(hWin);
    } else {
            hWin = MESSAGEBOX_Create("Iam", "File:", 0);
      GUI_Delay(2000);
      WM_DeleteWindow(hWin);
    }
		goto _start;
  
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    
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
*       _GetExt
*/
#ifndef WIN32
static void _GetExt(char * pFile, char * pExt) {
  int Len;
  int i;
  int j;

  //
  // Search beginning of extension
  //
  Len = strlen(pFile);
	Len = strlen(pFile);
  for (i = Len; i > 0; i--) {
    if (*(pFile + i) == '.') {
      *(pFile + i) = '\0';     // Cut extension from filename
      break;
    }
  }
  //
  // Copy extension
  //
  j = 0;
  while (*(pFile + ++i) != '\0') {
    *(pExt + j) = *(pFile + i);
    j++;
  }
  *(pExt + j) = '\0';          // Set end of string
}
#endif

/*********************************************************************
*
*       _GetData
*/
static int _GetData(CHOOSEFILE_INFO * pInfo) {

	static FINFO info;
  static char         acAttrib[10]         = { 0 };
  static char         acFile  [_MAX_PATH];
  static char         acExt   [_MAX_EXT];
  char                c;
  int                 i;
  int                 r;
	static char Addr[256];
	
  switch (pInfo->Cmd) {
  case CHOOSEFILE_FINDFIRST:
	sprintf(Addr,"%s*.*",pInfo->pRoot);
		info.fileID=0;
		r =ffind(Addr,&info);
  //  r = FS_FindFirstFile(&FindData, pInfo->pRoot, acFile, sizeof(acFile));
    break;
  case CHOOSEFILE_FINDNEXT:
		r =ffind(Addr,&info);
   // r = FS_FindNextFile(&FindData) ^ 1;
    break;
  }
  if (r == 0) {
    pInfo->Flags = (info.attrib & _aAttrib[3].Mask) ? CHOOSEFILE_FLAG_DIRECTORY : 0;
    for (i = 0; i < GUI_COUNTOF(_aAttrib); i++) {
      if (info.attrib & _aAttrib[i].Mask) {
        c = _aAttrib[i].c;
      } else {
        c = '-';
      }
      acAttrib[i] = c;
    }
		
		strcpy(acFile,(char *)info.name);
    _GetExt(acFile, acExt);
    pInfo->pAttrib = acAttrib;
    pInfo->pName   = acFile;
    pInfo->pExt    = acExt;
    pInfo->SizeL   = info.size;
    pInfo->SizeH   = 0;
  }
  return r;
#endif
}

/*********************************************************************
*
*       _EnableSkinning
*/
static void _EnableSkinning(int OnOff) {
    FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
    BUTTON_SetDefaultSkin  (BUTTON_SKIN_FLEX);
    DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
    HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
    SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
  
}


/*********************************************************************
*
*       Createtools
*/

WM_HWIN Createexplorer(void);
WM_HWIN Createexplorer(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);

  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
