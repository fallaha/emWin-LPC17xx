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
File        : FS_Storage.h
Purpose     : Define global functions and types to be used by an
              application using the storage API.

              This file needs to be included by any module using the
              storage API.
---------------------------END-OF-HEADER------------------------------
*/

#ifndef __FS_STORAGE_H__               // Avoid recursive and multiple inclusion
#define __FS_STORAGE_H__

/*********************************************************************
*
*             #include Section
*
**********************************************************************
*/

#include "FS_ConfDefaults.h"        /* FS Configuration */
#include "FS_Types.h"
#include "FS_Dev.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Defines, non-configurable
*
**********************************************************************
*/

/*********************************************************************
*
*       Media states
*/
#define FS_MEDIA_NOT_PRESENT        0
#define FS_MEDIA_IS_PRESENT         1
#define FS_MEDIA_STATE_UNKNOWN      2

#define FS_OPERATION_READ           0
#define FS_OPERATION_WRITE          1

/*********************************************************************
*
*       Typedefs
*
**********************************************************************
*/

/*********************************************************************
*
*       FS_DEV_INFO
*/
struct FS_DEV_INFO {
  U16 NumHeads;          // Relevant only for mechanical drives
  U16 SectorsPerTrack;   // Relevant only for mechanical drives
  U32 NumSectors;        // Total number of sectors on the medium
  U16 BytesPerSector;    // Number of bytes per sector
};

/*********************************************************************
*
*       FS_STORAGE_COUNTERS
*/
typedef struct {
  U32 ReadOperationCnt;       // Number of "Read sector operation" calls
  U32 ReadSectorCnt;          // Number of sectors read (before cache). Note that this can be (and typically is) higher than the number of "Read sector operation" counts, since one operation can request multiple sectors (in a burst).
  U32 ReadSectorCachedCnt;    // Number of sectors read from cache
  U32 WriteOperationCnt;      // Number of "Write sector operation" calls
  U32 WriteSectorCnt;         // Number of sectors written (before cache). Note that this can be (and typically is) higher than the number of "Write sector operation" counts, since one operation can write multiple sectors (in a burst).
  U32 WriteSectorCntCleaned;  // Number of sectors written by the cache to storage in order to make room for other data.
  U32 ReadSectorCntMan;       // Number of management sectors read (before cache).
  U32 ReadSectorCntDir;       // Number of directory sectors (which store directory entries) read (before cache).
  U32 WriteSectorCntMan;      // Number of management sectors written (before cache).
  U32 WriteSectorCntDir;      // Number of directory sectors (which store directory entries) written (before cache).
} FS_STORAGE_COUNTERS;

typedef void (FS_ONDEVICEACTIVITYHOOK)(FS_DEVICE * pDevice, unsigned Operation, U32 StartSector, U32 NumSectors, int Sectortype);

/*********************************************************************
*
*       Global function prototypes
*
**********************************************************************
*/
int      FS_STORAGE_Clean          (const char * sVolume);
int      FS_STORAGE_CleanOne       (const char * sVolume, int * pMore);
#if FS_SUPPORT_DEINIT
void     FS_STORAGE_DeInit         (void);
#endif
int      FS_STORAGE_FreeSectors    (const char * sVolume, U32 FirstSector, U32 NumSectors);
int      FS_STORAGE_GetCleanCnt    (const char * sVolume, U32 * pCnt);
void     FS_STORAGE_GetCounters    (FS_STORAGE_COUNTERS * pStat);
int      FS_STORAGE_GetDeviceInfo  (const char * sVolume, FS_DEV_INFO * pDevInfo);
int      FS_STORAGE_GetSectorUsage (const char * sVolume, U32 SectorIndex);
unsigned FS_STORAGE_Init           (void);
int      FS_STORAGE_ReadSector     (const char * sVolume,       void * pData, U32 SectorIndex);
int      FS_STORAGE_ReadSectors    (const char * sVolume,       void * pData, U32 FirstSector, U32 NumSectors);
int      FS_STORAGE_RefreshSectors (const char * sVolume, U32 FirstSector, U32 NumSectors, void * pBuffer, U32 NumBytes);
void     FS_STORAGE_ResetCounters  (void);
int      FS_STORAGE_WriteSector    (const char * sVolume, const void * pData, U32 SectorIndex);
int      FS_STORAGE_WriteSectors   (const char * sVolume, const void * pData, U32 FirstSector, U32 NumSectors);
void     FS_STORAGE_Sync           (const char * sVolume);
int      FS_STORAGE_SyncSectors    (const char * sVolume, U32 FirstSector, U32 NumSectors); 
void     FS_STORAGE_Unmount        (const char * sVolume);
void     FS_STORAGE_UnmountForced  (const char * sVolume);
void     FS_SetOnDeviceActivityHook(const char * sVolume, FS_ONDEVICEACTIVITYHOOK * pfOnDeviceActivityHook);

/*********************************************************************
*
*       Compatibility defines
*/
#define FS_InitStorage()                                    FS_STORAGE_Init()
#define FS_ReadSector(sVolume, pData, SectorIndex)          FS_STORAGE_ReadSector(sVolume,  pData, SectorIndex)
#define FS_WriteSector(sVolume, pData, SectorIndex)         FS_STORAGE_WriteSector(sVolume, pData, SectorIndex)
#define FS_UnmountLL(sVolume)                               FS_STORAGE_Unmount(sVolume)
#define FS_CleanVolume(sVolume)                             FS_STORAGE_Sync(sVolume)
#define FS_GetDeviceInfo(sVolume, pDevInfo)                 FS_STORAGE_GetDeviceInfo(sVolume, pDevInfo)

#if defined(__cplusplus)
  }              /* Make sure we have C-declarations in C++ programs */
#endif

#endif // __FS_STORAGE_H__

/*************************** End of file ****************************/
