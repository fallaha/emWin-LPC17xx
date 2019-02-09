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
File        : FAT.h
Purpose     : FAT File System Layer header
---------------------------END-OF-HEADER------------------------------
*/
#ifndef __FAT_H__               // Avoid recursive and multiple inclusion
#define __FAT_H__

#include "FS_Int.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       FAT_DiskInfo
*/
int  FS_FAT_GetDiskInfo   (FS_VOLUME * pVolume, FS_DISK_INFO * pDiskData, int Flags);
U32  FS_FAT_GetDiskSpace  (FS_VOLUME * pVolume);
int  FS_FAT_GetVolumeLabel(FS_VOLUME * pVolume, char * pVolumeLabel, unsigned VolumeLabelSize);
int  FS_FAT_SetVolumeLabel(FS_VOLUME * pVolume, const char * pVolumeLabel);

/*********************************************************************
*
*       FAT_Read
*/
U32  FS_FAT_Read(FS_FILE *pFile, void *pData, U32 NumBytes);

/*********************************************************************
*
*       FAT_Write
*/
U32  FS_FAT_Write  (FS_FILE   * pFile, const void *pData, U32 NumBytes);
void FS_FAT_Close  (FS_FILE   * pFile);
void FS_FAT_Clean  (FS_VOLUME * pVolume);

/*********************************************************************
*
*       FAT_Open
*/
int  FS_FAT_Open   (const char * pFileName, FS_FILE * pFile, U8 DoDel, U8 DoOpen, U8 DoCreate);

/*********************************************************************
*
*       FAT_Misc
*/
int  FS_FAT_CheckBPB            (FS_VOLUME * pVolume);
int  FS_FAT_CreateJournalFile   (FS_VOLUME * pVolume, U32 NumClusters, U32 * pFirstSector, U32 * pNumSectors);
int  FS_FAT_OpenJournalFile     (FS_VOLUME * pVolume);
U32  FS_FAT_GetIndexOfLastSector(FS_VOLUME * pVolume);
int  FS_FAT_FreeSectors         (FS_VOLUME * pVolume);

/*********************************************************************
*
*       FAT_Format
*/
int  FS_FAT_Format  (FS_VOLUME * pVolume, const FS_FORMAT_INFO    * pFormatInfo);
int  FS_FAT_FormatEx(FS_VOLUME * pVolume, const FS_FORMAT_INFO_EX * pFormatInfoEx);

/*********************************************************************
*
*       FAT_Move
*/
int  FS_FAT_Move(const char * sOldName, const char * sNewName, FS_VOLUME * pVolume);

/*********************************************************************
*
*       FS_FAT_DirEntry
*
*/
int  FS_FAT_SetDirEntryInfo(FS_VOLUME * pVolume, const char * sName, const void * p, int Mask);
int  FS_FAT_GetDirEntryInfo(FS_VOLUME * pVolume, const char * sName,       void * p, int Mask);

/*********************************************************************
*
*       FAT_Rename
*/
int   FS_FAT_Rename(const char * sOldName, const char * sNewName, FS_VOLUME * pVolume);

/*********************************************************************
*
*       FAT_Dir
*/
int  FS_FAT_OpenDir  (const char *pDirName, FS__DIR *pDir);
int  FS_FAT_CloseDir (FS__DIR *pDir);
int  FS_FAT_ReadDir  (FS__DIR *pDir, FS_DIRENTRY_INFO * pDirEntryInfo);
int  FS_FAT_RemoveDir(FS_VOLUME * pVolume, const char *pDirName);
int  FS_FAT_CreateDir(FS_VOLUME * pVolume, const char *pDirName);

/*********************************************************************
*
*       FAT_SetEndOfFile
*/
int  FS_FAT_SetEndOfFile(FS_FILE * pFile);
int  FS_FAT_SetFileSize (FS_FILE * pFile, U32 NumBytes);

/*********************************************************************
*
*       FS_FAT_CheckDisk
*/
int  FS_FAT__CheckDisk(FS_VOLUME * pVolume, const FS_DISK_INFO * pDiskInfo, void * pBuffer, U32 BufferSize, int MaxRecursionLevel, FS_CHECKDISK_ON_ERROR_CALLBACK * pfOnError);

#if defined(__cplusplus)
}                /* Make sure we have C-declarations in C++ programs */
#endif

#endif // __FAT_H__

/*************************** End of file ****************************/
