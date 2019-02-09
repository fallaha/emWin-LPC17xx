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
File        : FS.h
Purpose     : Define global functions and types to be used by an
              application using the file system.

              This file needs to be included by any module using the
              file system.
---------------------------END-OF-HEADER------------------------------
*/

#ifndef FS_H               // Avoid recursive and multiple inclusion
#define FS_H

/*********************************************************************
*
*             #include Section
*
**********************************************************************
*/

#include "FS_ConfDefaults.h"        /* FS Configuration */
#include "FS_Types.h"
#include "FS_Storage.h"
#include "FS_Dev.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*             #define constants
*
**********************************************************************
*/

/*********************************************************************
*
*       File system version
*/
#define FS_VERSION      40401UL

/*********************************************************************
*
*       Error codes returned by FS_FError() and other API functions
*/
#define FS_ERRCODE_OK                           0         // No error
#define FS_ERRCODE_EOF                          (-1)      // End of file reached
#define FS_ERRCODE_PATH_TOO_LONG                (-2)      // Path to file or directory is too long
#define FS_ERRCODE_INVALID_PARA                 (-3)      // Invalid parameter passed
#define FS_ERRCODE_WRITE_ONLY_FILE              (-5)      // File can only be written
#define FS_ERRCODE_READ_ONLY_FILE               (-6)      // File can not be written
#define FS_ERRCODE_READ_FAILURE                 (-7)      // Error while reading from storage medium
#define FS_ERRCODE_WRITE_FAILURE                (-8)      // Error while writing to storage medium
#define FS_ERRCODE_FILE_IS_OPEN                 (-9)      // Trying to delete a file which is open
#define FS_ERRCODE_PATH_NOT_FOUND               (-10)     // Path to file or directory not found
#define FS_ERRCODE_FILE_DIR_EXISTS              (-11)     // File or directory already exists
#define FS_ERRCODE_NOT_A_FILE                   (-12)     // Trying to open a directory instead of a file
#define FS_ERRCODE_TOO_MANY_FILES_OPEN          (-13)     // Exceeded number of files opened at once (trial version)
#define FS_ERRCODE_INVALID_FILE_HANDLE          (-14)     // The file handle has been invalidated
#define FS_ERRCODE_VOLUME_NOT_FOUND             (-15)     // The volume name specified in a path is does not exist
#define FS_ERRCODE_READ_ONLY_VOLUME             (-16)     // Trying to write to a volume mounted in read-only mode
#define FS_ERRCODE_VOLUME_NOT_MOUNTED           (-17)     // Trying access a volume which is not mounted
#define FS_ERRCODE_NOT_A_DIR                    (-18)     // Trying to open a file instead of a directory
#define FS_ERRCODE_FILE_DIR_NOT_FOUND           (-19)     // File or directory not found
#define FS_ERRCODE_NOT_SUPPORTED                (-20)     // Functionality not supported
#define FS_ERRCODE_CLUSTER_NOT_FREE             (-21)     // Trying to allocate a cluster which is not free
#define FS_ERRCODE_INVALID_CLUSTER_CHAIN        (-22)     // Detected a shorter than expected cluster chain
#define FS_ERRCODE_STORAGE_NOT_PRESENT          (-23)     // Trying to access a removable storage which is not inserted
#define FS_ERRCODE_BUFFER_NOT_AVAILABLE         (-24)     // No more sector buffers available
#define FS_ERRCODE_STORAGE_TOO_SMALL            (-25)     // Not enough sectors on the storage medium
#define FS_ERRCODE_STORAGE_NOT_READY            (-26)     // Storage device can not be accessed
#define FS_ERRCODE_BUFFER_TOO_SMALL             (-27)     // Sector buffer smaller than sector size of storage medium
#define FS_ERRCODE_INVALID_FS_FORMAT            (-28)     // Storage medium is not formatted or the format is not valid
#define FS_ERRCODE_INVALID_FS_TYPE              (-29)     // Type of file system is invalid or not configured
#define FS_ERRCODE_FILENAME_TOO_LONG            (-30)     // The name of the file is too long
#define FS_ERRCODE_VERIFY_FAILURE               (-31)     // Data verification failure
#define FS_ERRCODE_VOLUME_FULL                  (-32)     // No more space on storage medium
#define FS_ERRCODE_DIR_NOT_EMPTY                (-33)     // Trying to delete a directory which is not empty
#define FS_ERRCODE_IOCTL_FAILURE                (-34)     // Error while executing a driver control command
#define FS_ERRCODE_INVALID_MBR                  (-35)     // Invalid information in the Master Boot Record
#define FS_ERRCODE_OUT_OF_MEMORY                (-36)     // Could not allocate memory
#define FS_ERRCODE_UNKNOWN_DEVICE               (-37)     // Storage device is not configured
#define FS_ERRCODE_ASSERT_FAILURE               (-38)     // FS_DEBUG_ASSERT() macro failed
#define FS_ERRCODE_TOO_MANY_TRANSACTIONS_OPEN   (-39)     // Maximum number of opened journal transactions exceeded
#define FS_ERRCODE_NO_OPEN_TRANSACTION          (-40)     // Trying to close a journal transaction which is not opened
#define FS_ERRCODE_INIT_FAILURE                 (-41)     // Error while initializing the storage medium
#define FS_ERRCODE_FILE_TOO_LARGE               (-42)     // Trying to write to a file which is larger than 4 Gbytes
#define FS_ERRCODE_HW_LAYER_NOT_SET             (-43)     // The HW layer is not configured

/*********************************************************************
*
*       File positioning defines
*/
#define FS_SEEK_CUR                     1
#define FS_SEEK_END                     2
#define FS_SEEK_SET                     0

#define FS_FILE_CURRENT                 FS_SEEK_CUR
#define FS_FILE_END                     FS_SEEK_END
#define FS_FILE_BEGIN                   FS_SEEK_SET

/*********************************************************************
*
*       I/O commands for driver (internal)
*/
#define FS_CMD_REQUIRES_FORMAT          1003
#define FS_CMD_GET_DEVINFO              1004
#define FS_CMD_FORMAT_LOW_LEVEL         1005            // Used internally by FS_FormatLow() to command the driver to perform low-level format
#define FS_CMD_FREE_SECTORS             1006            // Used internally: Allows the FS layer to inform driver about free sectors
#define FS_CMD_SET_DELAY                1007            // Used in the simulation to simulate a slow device with RAM driver
#define FS_CMD_UNMOUNT                  1008            // Used internally by FS_STORAGE_Unmount() to inform the driver. Driver invalidates caches and all other information about medium.
#define FS_CMD_UNMOUNT_FORCED           1009            // Used internally by FS_STORAGE_UnmountForced() to inform the driver about an unforced remove of the device.
                                                        // Driver invalidates caches and all other information about medium.
#define FS_CMD_SYNC                     1010            // Tells the driver to clean caches. Typically, all dirty sectors are written.
#define FS_CMD_UNMOUNT_VOLUME           FS_CMD_UNMOUNT  // Obsolete: FS_CMD_UNMOUNT shall be used instead of FS_CMD_UNMOUNT_VOLUME.
#define FS_CMD_DEINIT                   1011            // Frees the resources allocated by the file system.
#define FS_CMD_CLEAN_ONE                1012            // Tells the driver to perform one clean operation. Usually this operation erases some data block on the storage medium which stores invalid information.
                                                        // Aux                    Not used
                                                        // pBuffer  [*int, OUT]   ==0 A second command will do nothing
                                                        //                        ==1 A second command will perform a clean operation
#define FS_CMD_CLEAN                    1013            // Executes the clean operation until all the invalid data on the storage medium has been erased
#define FS_CMD_GET_SECTOR_USAGE         1014            // Queries the driver about the usage of a logical sector.
                                                        // Aux      [int,  IN]    Index of the sector to be queried
                                                        // pBuffer  [*int, OUT]   ==0 Sector in use
                                                        //                        ==1 Sector not used
                                                        //                        ==2 Unknown
#define FS_CMD_ENABLE_READ_AHEAD        1015            // Requests the driver to start reading sectors in advance
#define FS_CMD_DISABLE_READ_AHEAD       1016            // Requests the driver to stop reading sectors in advance
#define FS_CMD_GET_CLEAN_CNT            1017            // Returns the number of clean operations which should be performed before all the invalid data on the storage medium has been erased
                                                        // pBuffer  [*int, OUT]   Number of clean operations
#define FS_CMD_SET_READ_ERROR_CALLBACK  1018            // Registers a callback function which should be invoked by a driver when a read error occurs.
                                                        // Aux      not used
                                                        // pBuffer  [*FS_ON_READ_ERROR_CALLBACK, IN]  Pointer to callback function
#define FS_CMD_SYNC_SECTORS             1019            // Informs the driver about the sectors which must be synchronized. Typically, the sectors are written to storage if cached.
                                                        // Aux      [int,  IN]    Index of the fist sector to be synchronized
                                                        // pBuffer  [*int, IN]    Number of sectors to synchronize

/*********************************************************************
*
*       CACHE Commands (internal)
*/
#define FS_CMD_CACHE_SET_MODE           6000L
#define FS_CMD_CACHE_CLEAN              6001L   // Write out all dirty sectors
#define FS_CMD_CACHE_SET_QUOTA          6002L
#define FS_CMD_CACHE_FREE_SECTORS       6003L
#define FS_CMD_CACHE_INVALIDATE         6004L   // Invalidate all sectors in cache
#define FS_CMD_CACHE_SET_ASSOC_LEVEL    6005L   // Sets the associativity level for the multi-way cache
#define FS_CMD_CACHE_GET_NUM_SECTORS    6006L   // Returns the number of sectors the cache is able to store
#define FS_CMD_CACHE_GET_TYPE           6007L   // Returns the type of the cache configured

/*********************************************************************
*
*       Directory entry attribute definitions
*/
#define FS_ATTR_READ_ONLY               0x01
#define FS_ATTR_HIDDEN                  0x02
#define FS_ATTR_SYSTEM                  0x04
#define FS_ATTR_ARCHIVE                 0x20
#define FS_ATTR_DIRECTORY               0x10

/*********************************************************************
*
*       File time ids
*/
#define FS_FILETIME_CREATE              0
#define FS_FILETIME_ACCESS              1
#define FS_FILETIME_MODIFY              2

/*********************************************************************
*
*       Volume mount flags
*/
#define FS_MOUNT_R                      (1 << 0)
#define FS_MOUNT_W                      (1 << 1)
#define FS_MOUNT_RW                     (FS_MOUNT_R | FS_MOUNT_W)

/*********************************************************************
*
*       File system selection
*/
#define FS_FAT                          0
#define FS_EFS                          1

/*********************************************************************
*
*       File system type
*/
#define FS_TYPE_FAT12                   0x000C
#define FS_TYPE_FAT16                   0x0010
#define FS_TYPE_FAT32                   0x0020
#define FS_TYPE_EFS                     0x0120

/*********************************************************************
*
*       FS_CheckDisk() error codes
*/
#define FS_CHECKDISK_ERRCODE_0FILE                    0x10
#define FS_CHECKDISK_ERRCODE_SHORTEN_CLUSTER          0x11
#define FS_CHECKDISK_ERRCODE_CROSSLINKED_CLUSTER      0x12
#define FS_CHECKDISK_ERRCODE_FEW_CLUSTER              0x13
#define FS_CHECKDISK_ERRCODE_CLUSTER_UNUSED           0x14
#define FS_CHECKDISK_ERRCODE_CLUSTER_NOT_EOC          0x15
#define FS_CHECKDISK_ERRCODE_INVALID_CLUSTER          0x16
#define FS_CHECKDISK_ERRCODE_INVALID_DIRECTORY_ENTRY  0x17
#define FS_CHECKDISK_ERRCODE_SECTOR_NOT_IN_USE        0x18

/*********************************************************************
*
*       FS_CheckDisk() return values
*/
#define FS_CHECKDISK_RETVAL_OK                0     // OK, file system not in corrupted state
#define FS_CHECKDISK_RETVAL_RETRY             1     // An error has be found and repaired, retry is required.
#define FS_CHECKDISK_RETVAL_ABORT             2     // User aborted operation via callback or API call
#define FS_CHECKDISK_RETVAL_MAX_RECURSE       3     // Max recursion level reached, operation aborted
#define FS_CHECKDISK_RETVAL_ERROR             (-1)  // Error while accessing the file system

/*********************************************************************
*
*       FS_CheckDisk() callback return codes
*/
#define FS_CHECKDISK_ACTION_DO_NOT_REPAIR     0     // Do not repair the error
#define FS_CHECKDISK_ACTION_SAVE_CLUSTERS     1     // Save lost cluster chain to file
#define FS_CHECKDISK_ACTION_ABORT             2     // Abort the disk checking operation
#define FS_CHECKDISK_ACTION_DELETE_CLUSTERS   3     // Delete the chain of lost clusters

/*********************************************************************
*
*       Flags for FS_GetVolumeInfoEx()
*/
#define FS_DISKINFO_FLAG_FREE_SPACE           (1 << 0)  // Returns the available free space on the storage medium

/*********************************************************************
*
*       FS_MTYPE_...
*
*  Ids to distinguish different message types
*/
#define FS_MTYPE_INIT             (1uL << 0)
#define FS_MTYPE_API              (1uL << 1)
#define FS_MTYPE_FS               (1uL << 2)
#define FS_MTYPE_STORAGE          (1uL << 3)
#define FS_MTYPE_JOURNAL          (1uL << 4)
#define FS_MTYPE_CACHE            (1uL << 5)
#define FS_MTYPE_DRIVER           (1uL << 6)
#define FS_MTYPE_OS               (1uL << 7)
#define FS_MTYPE_MEM              (1uL << 8)

/*********************************************************************
*
*       Sector usage
*/
#define FS_SECTOR_IN_USE          0
#define FS_SECTOR_NOT_USED        1
#define FS_SECTOR_USAGE_UNKNOWN   2

/*********************************************************************
*
*       Global data types
*
**********************************************************************
*/

/*********************************************************************
*
*       Callback functions
*/
typedef int    FS_CHECKDISK_ON_ERROR_CALLBACK  (int ErrCode, ...);
typedef void   FS_BUSY_LED_CALLBACK            (U8 OnOff);
typedef int    FS_MEMORY_IS_ACCESSIBLE_CALLBACK(void * p, U32 NumBytes);
typedef void   FS_PF_FREE                      (void * p);
typedef void * FS_PF_ALLOC                     (U32 NumBytes);

/*********************************************************************
*
*       FS_ON_READ_ERROR_CALLBACK
*
*  Function description
*    The function is called when a driver encounters an error while
*    reading sector data. Typ. called by the NAND driver to get corrected
*    sector data when a bit error occurs.
*
*  Parameters
*    pDeviceType    Type of storage device which encountered the read error
*    DeviceUnit     Unit number of the storage device where the read error occurred
*    SectorIndex    Index of the sector where the read error occurred
*    pBuffer        [IN]  ---
*                   [OUT] Corrected sector data
*    NumSectors     Number of sectors on which the read error occurred
*
*  Return value
*    ==0    OK, sector data returned
*    !=0    An error occurred
*/
typedef struct FS_ON_READ_ERROR_CALLBACK {
  int (*pfCallback)(const FS_DEVICE_TYPE * pDeviceType, U32 DeviceUnit, U32 SectorIndex, void * pBuffer, U32 NumSectors);
} FS_ON_READ_ERROR_CALLBACK;

/*********************************************************************
*
*       Structures
*/
struct FS_DIRENT {
  char DirName[FS_MAX_PATH];
  U8   Attributes;
  U32  Size;
  U32  TimeStamp;
};

typedef struct {
  U32 Cluster;                    // Cluster of current sector
  U32 FirstCluster;
  U32 DirEntryIndex;              // Directory entry index (first directory entry has index 0)
  U32 ClusterIndex;
} FS_DIR_POS;

typedef struct {
  FS_DIR_POS  DirPos;             // Current position in directory
  U16         DirEntryIndex;
  U32         FirstCluster;
  FS_VOLUME * pVolume;
  U8          InUse;
} FS__DIR;

typedef struct {
  U8     Attributes;
  U32    CreationTime;
  U32    LastAccessTime;
  U32    LastWriteTime;
  U32    FileSize;
  char * sFileName;
  int    SizeofFileName;
} FS_DIRENTRY_INFO;

typedef struct {
  U8     Attributes;
  U32    CreationTime;
  U32    LastAccessTime;
  U32    LastWriteTime;
  U32    FileSize;
  char * sFileName;
  //
  // Private elements. Not to be used by the application.
  //
  int SizeofFileName;
  FS__DIR Dir;
} FS_FIND_DATA;

typedef struct {
  U32 NumTotalClusters;     // Total number of clusters on the storage medium
  U32 NumFreeClusters;      // Number of unused clusters
  U16 SectorsPerCluster;    // Number of sectors in a cluster
  U16 BytesPerSector;       // Sector size in bytes
  U16 NumRootDirEntries;    // Number of directory entries in the root directory. Valid only for FS_TYPE_FAT12 and FS_TYPE_FAT16.
  U16 FSType;               // Type of file system. One of FS_TYPE_... defines.
  U8  IsSDFormatted;        // Set to 1 if the volume has been formatted acc. to SD specification
  U8  IsDirty;              // Set to 1 if the volume was not unmounted correctly or the file system modified the storage.
} FS_DISK_INFO;

typedef struct {
  U16           SectorsPerCluster;    // Number of sectors in a cluster
  U16           NumRootDirEntries;    // Proposed, actual value depends on FATType
  FS_DEV_INFO * pDevInfo;
} FS_FORMAT_INFO;

typedef struct {
  U16 Year;
  U16 Month;
  U16 Day;
  U16 Hour;
  U16 Minute;
  U16 Second;
} FS_FILETIME;

typedef enum {
  FS_WRITEMODE_SAFE,
  FS_WRITEMODE_MEDIUM,
  FS_WRITEMODE_FAST,
  FS_WRITEMODE_UNKNOWN      // For internal use only.
} FS_WRITEMODE;

#if FS_SUPPORT_DEINIT
  struct FS_ON_EXIT_CB;

  typedef struct FS_ON_EXIT_CB {
    struct FS_ON_EXIT_CB * pNext;
    void (*pfOnExit)(void);
  } FS_ON_EXIT_CB;
#endif

typedef struct {
  U16 Cylinder;
  U8  Head;
  U8  Sector;
} FS_CHS_ADDR;

typedef struct {
  U32         NumSectors;
  U32         StartSector;
  FS_CHS_ADDR StartAddr;
  FS_CHS_ADDR EndAddr;
  U8          Type;
  U8          IsActive;
} FS_PARTITION_INFO;

/*********************************************************************
*
*       Global function prototypes
*
**********************************************************************
*/

/*********************************************************************
*
*       "Standard" file I/O functions
*/
FS_FILE * FS_FOpen  (const char * pFileName, const char * pMode);
int       FS_FOpenEx(const char * pFileName, const char * pMode, FS_FILE ** ppFile);
int       FS_FClose (FS_FILE    * pFile);
U32       FS_FRead  (      void * pData, U32 Size, U32 N, FS_FILE * pFile);
U32       FS_FWrite (const void * pData, U32 Size, U32 N, FS_FILE * pFile);

/*********************************************************************
*
*       Non-standard file I/O functions
*/
U32 FS_Read (FS_FILE * pFile,       void * pData, U32 NumBytes);
U32 FS_Write(FS_FILE * pFile, const void * pData, U32 NumBytes);

/*********************************************************************
*
*       File pointer handling
*/
int FS_FSeek       (FS_FILE * pFile, I32 Offset, int Origin);
int FS_SetEndOfFile(FS_FILE * pFile);
int FS_SetFileSize (FS_FILE * pFile, U32 NumBytes);
I32 FS_FTell       (FS_FILE * pFile);

#define FS_GetFilePos(pFile)                                FS_FTell(pFile)
#define FS_SetFilePos(pFile, DistanceToMove, MoveMethod)    FS_FSeek(pFile, DistanceToMove, MoveMethod)

/*********************************************************************
*
*       I/O error handling
*/
int          FS_FEof        (FS_FILE * pFile);
I16          FS_FError      (FS_FILE * pFile);
void         FS_ClearErr    (FS_FILE * pFile);
const char * FS_ErrorNo2Text(int       ErrCode);

/*********************************************************************
*
*       File functions
*/
int FS_CopyFile   (const char * sSource,       const char * sDest);
int FS_CopyFileEx (const char * sSource,       const char * sDest, void * pBuffer, U32 NumBytes);
U32 FS_GetFileSize(const FS_FILE    * pFile);
int FS_Move       (const char * sExistingName, const char * sNewName);
int FS_Remove     (const char * pFileName);
int FS_Rename     (const char * sOldName,      const char * sNewName);
int FS_Truncate   (FS_FILE    * pFile,         U32          NewSize);
int FS_Verify     (FS_FILE    * pFile,         const void * pData, U32 NumBytes);
int FS_SyncFile   (FS_FILE    * pFile);
int FS_WipeFile   (const char * pFileName);

/*********************************************************************
*
*       IOCTL
*/
int         FS_IoCtl(const char *pVolumeName, I32 Cmd, I32 Aux, void * pBuffer);

/*********************************************************************
*
*       Volume related functions
*/
int         FS_GetVolumeName       (int Index, char * pBuffer, int MaxSize);
U32         FS_GetVolumeSize       (const char * sVolumeName);
U32         FS_GetVolumeSizeKB     (const char * sVolumeName);
U32         FS_GetVolumeFreeSpace  (const char * sVolumeName);
U32         FS_GetVolumeFreeSpaceKB(const char * sVolumeName);
int         FS_GetNumVolumes       (void);
FS_VOLUME * FS_AddDevice           (const FS_DEVICE_TYPE * pDevType);
void        FS_Unmount             (const char * sVolumeName);
int         FS_Mount               (const char * sVolumeName);
int         FS_MountEx             (const char * sVolumeName, U8 MountType);
int         FS_GetVolumeInfo       (const char * sVolumeName, FS_DISK_INFO * pInfo);
int         FS_GetVolumeInfoEx     (const char * sVolumeName, FS_DISK_INFO * pInfo, int Flags);
int         FS_IsVolumeMounted     (const char * sVolumeName);
int         FS_GetVolumeLabel      (const char * sVolumeName, char * pVolumeLabel, unsigned VolumeLabelSize);
int         FS_SetVolumeLabel      (const char * sVolumeName, const char * pVolumeLabel);
void        FS_UnmountForced       (const char * sVolumeName);
void        FS_SetAutoMount        (const char * sVolumeName, U8 MountType);
int         FS_GetVolumeStatus     (const char * sVolumeName);
FS_VOLUME * FS_FindVolume          (const char * sVolumeName);
void        FS_RemoveDevice        (const char * sVolumeName);
int         FS_Sync                (const char * sVolumeName);
int         FS_FreeSectors         (const char * sVolumeName);

/*********************************************************************
*
*       File write mode
*/
void        FS_SetFileWriteMode    (FS_WRITEMODE WriteMode);
void        FS_SetFileWriteModeEx  (FS_WRITEMODE WriteMode, const char * sVolumeName);

/*********************************************************************
*
*       Journaling / transaction safety
*/
typedef struct {
  U32 WriteSectorCnt;     // Number of sectors written by the file system to journal
  U32 NumTransactions;    // Number of journal transactions performed
  U32 FreeSectorCnt;      // Number of sectors freed
} FS_JOURNAL_STAT_COUNTERS;

int FS_JOURNAL_Begin            (const char * sVolumeName);
int FS_JOURNAL_End              (const char * sVolumeName);
int FS_JOURNAL_Create           (const char * sVolumeName, U32 NumBytes);
int FS_JOURNAL_CreateEx         (const char * sVolumeName, U32 NumBytes, U8 SupportFreeSector);
int FS_JOURNAL_Enable           (const char * sVolumeName);
int FS_JOURNAL_Disable          (const char * sVolumeName);
int FS_JOURNAL_GetStatCounters  (const char * sVolumeName, FS_JOURNAL_STAT_COUNTERS * pStat);
int FS_JOURNAL_ResetStatCounters(const char * sVolumeName);
int FS_JOURNAL_GetOpenCnt       (const char * sVolumeName);

/*********************************************************************
*
*       File/directory attribute functions
*/
int  FS_SetFileAttributes   (const char * pName, U8 Attributes);
U8   FS_GetFileAttributes   (const char * pName);
U8   FS_ModifyFileAttributes(const char * pName, U8 SetMask, U8 ClrMask);

/*********************************************************************
*
*       File/directory time stamp functions
*/
void FS_FileTimeToTimeStamp(const FS_FILETIME * pFileTime, U32         * pTimeStamp);
int  FS_GetFileTime        (const char        * pName,     U32         * pTimeStamp);
int  FS_GetFileTimeEx      (const char        * pName,     U32         * pTimeStamp, int Index);
int  FS_SetFileTime        (const char        * pName,     U32           TimeStamp);
int  FS_SetFileTimeEx      (const char        * pName,     U32           TimeStamp,  int Index);
void FS_TimeStampToFileTime(U32                 TimeStamp, FS_FILETIME * pFileTime);

/*********************************************************************
*
*       File system misc. functions
*/
int  FS_GetNumFilesOpen(void);
U32  FS_GetMaxSectorSize(void);

/*********************************************************************
*
*       File system directory functions
*/
int  FS_CreateDir      (const char * sDirPath);
int  FS_MkDir          (const char * pDirName);
int  FS_RmDir          (const char * pDirName);
int  FS_FindFirstFile  (FS_FIND_DATA * pfd, const char * sPath, char * sFilename, int sizeofFilename);
int  FS_FindNextFile   (FS_FIND_DATA * pfd);
void FS_FindClose      (FS_FIND_DATA * pfd);
U32  FS_FAT_GrowRootDir(const char * sVolumeName, U32 NumAddEntries);

/*********************************************************************
*
*       Obsolete directory functions
*/
void        FS_DirEnt2Attr   (FS_DIRENT  * pDirEnt, U8   * pAttr);
void        FS_DirEnt2Name   (FS_DIRENT  * pDirEnt, char * pBuffer);
U32         FS_DirEnt2Size   (FS_DIRENT  * pDirEnt);
U32         FS_DirEnt2Time   (FS_DIRENT  * pDirEnt);
U32         FS_GetNumFiles   (FS_DIR     * pDir);

FS_DIR    * FS_OpenDir    (const char * pDirName);
int         FS_CloseDir   (FS_DIR     * pDir);
FS_DIRENT * FS_ReadDir    (FS_DIR     * pDir);
void        FS_RewindDir  (FS_DIR     * pDir);

/*********************************************************************
*
*       File system standard application
*/
void FSTask(void);

/*********************************************************************
*
*       File system control functions
*/
void     FS_Init            (void);
#if FS_SUPPORT_DEINIT
  void   FS_DeInit          (void);
  void   FS_AddOnExitHandler(FS_ON_EXIT_CB * pCB, void (*pfOnExit)(void));
#endif

/*********************************************************************
*
*       Formatting
*/
int          FS_FormatLLIfRequired(const char * sVolumeName);
int          FS_FormatLow         (const char * sVolumeName);
int          FS_Format            (const char * sVolumeName, const FS_FORMAT_INFO * pFormatInfo);
int          FS_FormatSD          (const char * sVolumeName);
int          FS_IsLLFormatted     (const char * sVolumeName);
int          FS_IsHLFormatted     (const char * sVolumeName);

/*********************************************************************
*
*       Partitioning
*/
#define FS_NUM_PARTITIONS  4
int          FS_CreateMBR       (const char * sVolumeName, FS_PARTITION_INFO * pPartInfo, int NumPartitions);
int          FS_GetPartitionInfo(const char * sVolumeName, FS_PARTITION_INFO * pPartInfo, U8 PartIndex);

/*********************************************************************
*
*       CheckDisk functionality
*/
const char * FS_CheckDisk_ErrCode2Text(int ErrCode);
int          FS_CheckDisk             (const char * sVolumeName, void * pBuffer, U32 BufferSize, int MaxRecursionLevel, FS_CHECKDISK_ON_ERROR_CALLBACK * pfOnError);
void         FS_FAT_AbortCheckDisk    (void);

/*********************************************************************
*
*       Configuration functions.
*/

/*********************************************************************
*
*       General runtime configuration functions
*/
void FS_SetMaxSectorSize      (unsigned MaxSectorSize);
void FS_ConfigOnWriteDirUpdate(char OnOff);
void FS_AssignMemory          (U32 * pMem, U32 NumBytes);
void FS_SetMemHandler         (FS_PF_ALLOC * pfAlloc, FS_PF_FREE * pfFree);

/*********************************************************************
*
*       FAT runtime configuration functions.
*/
void   FS_FAT_SupportLFN                (void);
void   FS_FAT_DisableLFN                (void);
#if FS_FAT_USE_FSINFO_SECTOR
  void FS_FAT_ConfigFSInfoSectorUse     (int OnOff);
#endif
#if FS_MAINTAIN_FAT_COPY
  void FS_FAT_ConfigFATCopyMaintenance  (int OnOff);
#endif
#if FS_FAT_PERMIT_RO_FILE_MOVE
  void FS_FAT_ConfigROFileMovePermission(int OnOff);
#endif
#if FS_FAT_UPDATE_DIRTY_FLAG
  void FS_FAT_ConfigDirtyFlagUpdate     (int OnOff);
#endif

/*********************************************************************
*
*       File buffer related functions.
*/
#define FS_FILE_BUFFER_WRITE              (1 << 0)
#define FS_SIZEOF_FILE_BUFFER_STRUCT      24        // sizeof(FS_FILE_BUFFER)
#define FS_SIZEOF_FILE_BUFFER(NumBytes)   (FS_SIZEOF_FILE_BUFFER_STRUCT + (NumBytes))
#if FS_USE_FILE_BUFFER
  void FS_ConfigFileBufferDefault(int BufferSize, int Flags);     // Configures default file buffer size and flags for all files which will be opened. Should only be called once, in FS_X_AddDevices()
  void FS_SetFileBufferFlags(FS_FILE * pFile, int Flags);         // Is only allowed to be called immediately after a FS_FOpen(), in order to change the buffer flags of a specific file
#endif
#if FS_SUPPORT_FILE_BUFFER
  int FS_SetFileBuffer(FS_FILE * pFile, const void * pData, I32 NumBytes, int Flags);
#endif

/*********************************************************************
*
*       BusyLED support
*/
void FS_SetBusyLEDCallback(const char * sVolumeName, FS_BUSY_LED_CALLBACK * pfBusyLEDCallback);

/*********************************************************************
*
*       Memory accessible support
*/
void FS_SetMemAccessCallback(const char * sVolumeName, FS_MEMORY_IS_ACCESSIBLE_CALLBACK * pfIsAccessibleCallback);

/*********************************************************************
*
*       Misc. functions
*/
int FS_GetFileId(const char * pFileName, U8 * pId);

/*********************************************************************
*
*       Device Drivers
*
*/
struct FS_DEVICE_TYPE {      // Note: This definition is really intern and should be in "FS_Int.h". In order to avoid problems with old compilers, keep it here.
  const char * (*pfGetName)    (U8 Unit);
  int          (*pfAddDevice)  (void);                                                       // Called from AddDevice. Usually the first call to the driver
  int          (*pfRead)       (U8 Unit, U32 SectorIndex,       void * pBuffer, U32 NumSectors);
  int          (*pfWrite)      (U8 Unit, U32 SectorIndex, const void * pBuffer, U32 NumSectors, U8 RepeatSame);
  int          (*pfIoCtl)      (U8 Unit, I32 Cmd, I32 Aux, void * pBuffer);
  int          (*pfInitMedium) (U8 Unit);                                                       // Called when formatting or mounting a device
  int          (*pfGetStatus)  (U8 Unit);
  int          (*pfGetNumUnits)(void);
};

extern const FS_DEVICE_TYPE FS_RAMDISK_Driver;         // Driver which uses RAM as storage
extern const FS_DEVICE_TYPE FS_WINDRIVE_Driver;        // Driver for Windows drives
extern const FS_DEVICE_TYPE FS_MMC_CardMode_Driver;    // Driver for SD/MMC using card controller
extern const FS_DEVICE_TYPE FS_MMC_SPI_Driver;         // Driver for SD/MMC using SPI
extern const FS_DEVICE_TYPE FS_IDE_Driver;             // Driver for IDE and Compact Flash
extern const FS_DEVICE_TYPE FS_NOR_Driver;             // Driver for NOR flashes (fast write)
extern const FS_DEVICE_TYPE FS_NAND_Driver;            // Driver for SLC NAND flashes
extern const FS_DEVICE_TYPE FS_NOR_BM_Driver;          // Driver for NOR flashes (reduced RAM usage)
extern const FS_DEVICE_TYPE FS_NAND_UNI_Driver;        // Driver for SLC NAND flashes with ECC engine
extern const FS_DEVICE_TYPE FS_DISKPART_Driver;        // Logical driver for disk partitions
extern const FS_DEVICE_TYPE FS_CRYPT_Driver;           // Logical driver for encryption
extern const FS_DEVICE_TYPE FS_READAHEAD_Driver;       // Logical driver which reads sectors in advance
extern const FS_DEVICE_TYPE FS_RAID1_Driver;           // Logical driver which performs disk mirroring
extern const FS_DEVICE_TYPE FS_SECSIZE_Driver;         // Logical driver which converts between different sector sizes
extern const FS_DEVICE_TYPE FS_WRBUF_Driver;           // Logical driver which buffers sector write operations

/*********************************************************************
*
*       NOR driver
*
**********************************************************************
*/

/*********************************************************************
*
*       Block types assigned to a physical sector (FS_NOR_BM_Driver)
*/
#define FS_NOR_BLOCK_TYPE_UNKNOWN           0
#define FS_NOR_BLOCK_TYPE_DATA              1
#define FS_NOR_BLOCK_TYPE_WORK              2
#define FS_NOR_BLOCK_TYPE_EMPTY_ERASED      3
#define FS_NOR_BLOCK_TYPE_EMPTY_NOT_ERASED  4

/*********************************************************************
*
*       FS_NOR_SECTOR_INFO
*
*  Information about a physical sector (FS_NOR_Driver)
*/
typedef struct {
  U32 Off;
  U32 Size;
  U32 EraseCnt;
  U16 NumUsedSectors;
  U16 NumFreeSectors;
  U16 NumEraseableSectors;
} FS_NOR_SECTOR_INFO;

/*********************************************************************
*
*       FS_NOR_DISK_INFO
*
*  Information about the NOR flash organization (FS_NOR_Driver)
*/
typedef struct {
  U32 NumPhysSectors;
  U32 NumLogSectors;
  U32 NumUsedSectors;   // Number of used logical sectors
} FS_NOR_DISK_INFO;

/*********************************************************************
*
*       FS_NOR_STAT_COUNTERS
*
*  Statistical counters (FS_NOR_Driver)
*/
typedef struct {
  U32 EraseCnt;
  U32 ReadSectorCnt;
  U32 WriteSectorCnt;
  U32 CopySectorCnt;
} FS_NOR_STAT_COUNTERS;

/*********************************************************************
*
*       FS_NOR_BM_SECTOR_INFO
*
*  Information about a physical sector (FS_NOR_BM_Driver)
*/
typedef struct {
  U32 Off;          // Offset of the first byte relative to begin of NOR flash
  U32 Size;         // Size of physical sector in bytes
  U32 EraseCnt;     // Number of times the physical sector has been erased
  U16 lbi;          // Index of the logical block stored in the physical sector
  U8  Type;         // Type of data stored in the physical sector (see FS_NOR_BLOCK_TYPE_...)
} FS_NOR_BM_SECTOR_INFO;

/*********************************************************************
*
*       FS_NOR_BM_DISK_INFO
*
*  Information about the NOR flash organization (FS_NOR_BM_Driver)
*/
typedef struct {
  U16 NumPhySectors;
  U16 NumLogBlocks;
  U16 NumUsedPhySectors;
  U16 LSectorsPerPSector;
  U16 BytesPerSector;
  U32 EraseCntMax;
  U32 EraseCntMin;
  U32 EraseCntAvg;
  U8  HasFatalError;
  U8  ErrorType;
  U16 ErrorPSI;
} FS_NOR_BM_DISK_INFO;

/*********************************************************************
*
*       FS_NOR_BM_STAT_COUNTERS
*
*  Statistical counters (FS_NOR_BM_Driver)
*/
typedef struct {
  U32 NumFreeBlocks;
  U32 EraseCnt;
  U32 ReadSectorCnt;
  U32 WriteSectorCnt;
  U32 ConvertViaCopyCnt;
  U32 ConvertInPlaceCnt;
  U32 NumValidSectors;
  U32 CopySectorCnt;
} FS_NOR_BM_STAT_COUNTERS;

/*********************************************************************
*
*       FS_NOR_PHY_TYPE
*
*  Physical layer API
*/
typedef struct FS_NOR_PHY_TYPE {
  int  (*pfWriteOff)     (U8 Unit, U32 Off, const void * pSrc, U32 Len);
  int  (*pfReadOff)      (U8 Unit, void * pDest, U32 Off, U32 Len);
  int  (*pfEraseSector)  (U8 Unit, unsigned int SectorIndex);
  void (*pfGetSectorInfo)(U8 Unit, unsigned int SectorIndex, U32 * pOff, U32 * pLen);
  int  (*pfGetNumSectors)(U8 Unit);
  void (*pfConfigure)    (U8 Unit, U32 BaseAddr, U32 StartAddr, U32 NumBytes);
  void (*pfOnSelectPhy)  (U8 Unit);
  void (*pfDeInit)       (U8 Unit);
} FS_NOR_PHY_TYPE;

typedef void (FS_NOR_READ_CFI_FUNC)(U8 Unit, U32 BaseAddr, U32 Off, U8 * pData, unsigned NumItems);

/*********************************************************************
*
*       FS_NOR_HW_TYPE_SPI
*
*  Hardware layer for NOR flash devices connected via SPI
*/
typedef struct FS_NOR_HW_TYPE_SPI {
  int     (*pfInit)     (U8 Unit);
  void    (*pfEnableCS) (U8 Unit);
  void    (*pfDisableCS)(U8 Unit);
  void    (*pfRead)     (U8 Unit,       U8 * pData, int NumBytes);
  void    (*pfWrite)    (U8 Unit, const U8 * pData, int NumBytes);
  void    (*pfRead_x2)  (U8 Unit,       U8 * pData, int NumBytes);
  void    (*pfWrite_x2) (U8 Unit, const U8 * pData, int NumBytes);
  void    (*pfRead_x4)  (U8 Unit,       U8 * pData, int NumBytes);
  void    (*pfWrite_x4) (U8 Unit, const U8 * pData, int NumBytes);
} FS_NOR_HW_TYPE_SPI;

/*********************************************************************
*
*       FS_NOR_HW_TYPE_SPIFI
*
*  Hardware layer for NOR flash devices connected via memory-mapped SPI
*/
typedef struct FS_NOR_HW_TYPE_SPIFI {
  int     (*pfInit)      (U8 Unit);
  void    (*pfSetCmdMode)(U8 Unit);
  void    (*pfSetMemMode)(U8 Unit, U8 ReadCmd, unsigned NumBytesAddr, unsigned NumBytesDummy, U16 BusWidth);
  void    (*pfExecCmd)   (U8 Unit, U8 Cmd, U8 BusWidth);
  void    (*pfReadData)  (U8 Unit, U8 Cmd, const U8 * pPara, unsigned NumBytesPara, unsigned NumBytesAddr,       U8 * pData, unsigned NumBytesData, U16 BusWidth);
  void    (*pfWriteData) (U8 Unit, U8 Cmd, const U8 * pPara, unsigned NumBytesPara, unsigned NumBytesAddr, const U8 * pData, unsigned NumBytesData, U16 BusWidth);
} FS_NOR_HW_TYPE_SPIFI;

/*********************************************************************
*
*       FS_NOR_SPI_TYPE
*
*  Operations on serial NOR devices
*/
typedef struct FS_NOR_SPI_TYPE FS_NOR_SPI_TYPE;

/*********************************************************************
*
*       FS_NOR_SPI_DEVICE_LIST
*
*  Defines a list of supported devices.
*/
typedef struct FS_NOR_SPI_DEVICE_LIST {
  U8                       NumDevices;
  const FS_NOR_SPI_TYPE ** ppDevice;
} FS_NOR_SPI_DEVICE_LIST;

/*********************************************************************
*
*       Available physical layers for FS_NOR_Driver and FS_NOR_BM_Driver
*/
extern const FS_NOR_PHY_TYPE FS_NOR_PHY_CFI_1x16;            // 1 x 16-bit CFI compliant NOR flash
extern const FS_NOR_PHY_TYPE FS_NOR_PHY_CFI_2x16;            // 2 x 16-bit CFI compliant NOR flash
extern const FS_NOR_PHY_TYPE FS_NOR_PHY_ST_M25;              // ST M25P compliant Serial NOR flash
extern const FS_NOR_PHY_TYPE FS_NOR_PHY_SFDP;                // Serial NOR flash which supports Serial Flash Discoverable Parameters JDEDEC standard
extern const FS_NOR_PHY_TYPE FS_NOR_PHY_SPIFI;               // Serial NOR flash connected via a memory-mapped SPI interface

/*********************************************************************
*
*       Default HW layers for the NOR flash drivers.
*/
extern const FS_NOR_HW_TYPE_SPI FS_NOR_HW_ST_M25_Default;
extern const FS_NOR_HW_TYPE_SPI FS_NOR_HW_SFDP_Default;

/*********************************************************************
*
*       Lists of supported serial NOR devices
*/
extern const FS_NOR_SPI_DEVICE_LIST FS_NOR_SPI_DeviceList_All;
extern const FS_NOR_SPI_DEVICE_LIST FS_NOR_SPI_DeviceList_Default;
extern const FS_NOR_SPI_DEVICE_LIST FS_NOR_SPI_DeviceList_Micron;
extern const FS_NOR_SPI_DEVICE_LIST FS_NOR_SPI_DeviceList_Spansion;
extern const FS_NOR_SPI_DEVICE_LIST FS_NOR_SPI_DeviceList_Microchip;

/*********************************************************************
*
*       FS_NOR_Driver API functions
*/
void         FS_NOR_Configure              (U8 Unit, U32 BaseAddr, U32 StartAddr, U32 NumBytes);
void         FS_NOR_ConfigureReserve       (U8 Unit, U8 Percentage2Reserve);
int          FS_NOR_EraseDevice            (U8 Unit);
int          FS_NOR_FormatLow              (U8 Unit);
void         FS_NOR_GetDiskInfo            (U8 Unit, FS_NOR_DISK_INFO * pDiskInfo);
void         FS_NOR_GetSectorInfo          (U8 Unit, U32 PhysSectorIndex, FS_NOR_SECTOR_INFO * pSectorInfo);
void         FS_NOR_GetStatCounters        (U8 Unit, FS_NOR_STAT_COUNTERS * pStat);
int          FS_NOR_IsLLFormatted          (U8 Unit);
const void * FS_NOR_LogSector2PhySectorAddr(U8 Unit, U32 LogSectorNo);
int          FS_NOR_ReadOff                (U8 Unit, U32 Off, void * pData, U32 NumBytes);
void         FS_NOR_ResetStatCounters      (U8 Unit);
void         FS_NOR_SetPhyType             (U8 Unit, const FS_NOR_PHY_TYPE * pPhyType);
void         FS_NOR_SetSectorSize          (U8 Unit, U16 SectorSize);

/*********************************************************************
*
*       FS_NOR_BM_Driver API functions
*/
void         FS_NOR_BM_Configure           (U8 Unit, U32 BaseAddr, U32 StartAddr, U32 NumBytes);
int          FS_NOR_BM_EraseDevice         (U8 Unit);
int          FS_NOR_BM_FormatLow           (U8 Unit);
int          FS_NOR_BM_GetDiskInfo         (U8 Unit, FS_NOR_BM_DISK_INFO * pDiskInfo);
int          FS_NOR_BM_GetSectorInfo       (U8 Unit, U32 PhysSectorIndex, FS_NOR_BM_SECTOR_INFO * pSectorInfo);
void         FS_NOR_BM_GetStatCounters     (U8 Unit, FS_NOR_BM_STAT_COUNTERS * pStat);
int          FS_NOR_BM_IsLLFormatted       (U8 Unit);
int          FS_NOR_BM_ReadOff             (U8 Unit, void * pData, U32 Off, U32 NumBytes);
void         FS_NOR_BM_ResetStatCounters   (U8 Unit);
void         FS_NOR_BM_SetMaxEraseCntDiff  (U8 Unit, U32 EraseCntDiff);
void         FS_NOR_BM_SetNumWorkBlocks    (U8 Unit, unsigned NumWorkBlocks);
void         FS_NOR_BM_SetPhyType          (U8 Unit, const FS_NOR_PHY_TYPE * pPhyType);
void         FS_NOR_BM_SetSectorSize       (U8 Unit, unsigned SectorSize);

/*********************************************************************
*
*       CFI physical layer API functions
*/
void         FS_NOR_CFI_SetAddrGap         (U8 Unit, U32 StartAddr, U32 NumBytes);
void         FS_NOR_CFI_SetReadCFIFunc     (U8 Unit, FS_NOR_READ_CFI_FUNC * pReadCFI);

/*********************************************************************
*
*       ST M25 physical layer API functions
*/
void         FS_NOR_SPI_Configure          (U8 Unit, U32 SectorSize, U16 NumSectors);
void         FS_NOR_SPI_SetPageSize        (U8 Unit, U16 BytesPerPage);
void         FS_NOR_SPI_SetHWType          (U8 Unit, const FS_NOR_HW_TYPE_SPI * pHWType);

/*********************************************************************
*
*       SFDP physical layer API functions
*/
void         FS_NOR_SFDP_Allow2bitMode     (U8 Unit, U8 OnOff);
void         FS_NOR_SFDP_Allow4bitMode     (U8 Unit, U8 OnOff);
void         FS_NOR_SFDP_SetHWType         (U8 Unit, const FS_NOR_HW_TYPE_SPI * pHWType);
void         FS_NOR_SFDP_SetDeviceList     (U8 Unit, const FS_NOR_SPI_DEVICE_LIST * pDeviceList);

/*********************************************************************
*
*       SPIFI physical layer API functions
*/
void         FS_NOR_SPIFI_Allow2bitMode    (U8 Unit, U8 OnOff);
void         FS_NOR_SPIFI_Allow4bitMode    (U8 Unit, U8 OnOff);
void         FS_NOR_SPIFI_SetHWType        (U8 Unit, const FS_NOR_HW_TYPE_SPIFI   * pHWType);
void         FS_NOR_SPIFI_SetDeviceList    (U8 Unit, const FS_NOR_SPI_DEVICE_LIST * pDeviceList);

/*********************************************************************
*
*       RAMDISK driver
*
**********************************************************************
*/
void FS_RAMDISK_Configure(U8 Unit, void * pData, U16 BytesPerSector, U32 NumSectors);

/*********************************************************************
*
*       MMC/SD driver
*
**********************************************************************
*/

/*********************************************************************
*
*       Type of storage cards
*/
#define FS_MMC_CARD_TYPE_UNKNOWN    0
#define FS_MMC_CARD_TYPE_MMC        1
#define FS_MMC_CARD_TYPE_SD         2

/*********************************************************************
*
*       MMC_CARD_ID
*/
typedef struct MMC_CARD_ID {
  U8 aData[16];
} MMC_CARD_ID;

/*********************************************************************
*
*       FS_MMC_STAT_COUNTERS
*/
typedef struct FS_MMC_STAT_COUNTERS {
  U32 WriteSectorCnt;
  U32 WriteErrorCnt;
  U32 ReadSectorCnt;
  U32 ReadErrorCnt;
  U32 CmdExecCnt;
} FS_MMC_STAT_COUNTERS;

/*********************************************************************
*
*       FS_MMC_CARD_INFO
*/
typedef struct FS_MMC_CARD_INFO {
  U8  CardType;           // Type of the storage card (FS_MMC_CARD_TYPE_...)
  U8  BusWidth;           // Number of data lines used for the data transfer: 1,4 or 8.
  U8  IsWriteProtected;   // Set to 1 if the card is write protected
  U8  IsHighSpeedMode;    // Set to 1 if the card operates in the high-speed mode
  U16 BytesPerSector;     // Number of bytes in a sector
  U32 NumSectors;         // Total number of sectors on the card
} FS_MMC_CARD_INFO;

/*********************************************************************
*
*       FS_MMC_HW_TYPE_SPI
*
*  Hardware layer API for FS_MMC_Driver
*/
typedef struct FS_MMC_HW_TYPE_SPI {
  void  (*pfEnableCS)        (U8 Unit);
  void  (*pfDisableCS)       (U8 Unit);
  int   (*pfIsPresent)       (U8 Unit);
  int   (*pfIsWriteProtected)(U8 Unit);
  U16   (*pfSetMaxSpeed)     (U8 Unit, U16 MaxFreq);
  int   (*pfSetVoltage)      (U8 Unit, U16 Vmin, U16 Vmax);
  void  (*pfRead)            (U8 Unit,       U8 * pData, int NumBytes);
  void  (*pfWrite)           (U8 Unit, const U8 * pData, int NumBytes);
  int   (*pfReadEx)          (U8 Unit,       U8 * pData, int NumBytes);
  int   (*pfWriteEx)         (U8 Unit, const U8 * pData, int NumBytes);
  void  (*pfLock)            (U8 Unit);
  void  (*pfUnlock)          (U8 Unit);
} FS_MMC_HW_TYPE_SPI;

/*********************************************************************
*
*       FS_MMC_HW_TYPE_CM
*
*  Hardware layer API for FS_MMC_CardMode_Driver
*/
typedef struct FS_MMC_HW_TYPE_CM {
  void  (*pfInitHW)            (U8 Unit);
  void  (*pfDelay)             (int ms);
  int   (*pfIsPresent)         (U8 Unit);
  int   (*pfIsWriteProtected)  (U8 Unit);
  U16   (*pfSetMaxSpeed)       (U8 Unit, U16 Freq);
  void  (*pfSetResponseTimeOut)(U8 Unit, U32 Value);
  void  (*pfSetReadDataTimeOut)(U8 Unit, U32 Value);
  void  (*pfSendCmd)           (U8 Unit, unsigned Cmd, unsigned CmdFlags, unsigned ResponseType, U32 Arg);
  int   (*pfGetResponse)       (U8 Unit, void * pBuffer, U32 Size);
  int   (*pfReadData)          (U8 Unit,       void * pBuffer, unsigned NumBytes, unsigned NumBlocks);
  int   (*pfWriteData)         (U8 Unit, const void * pBuffer, unsigned NumBytes, unsigned NumBlocks);
  void  (*pfSetDataPointer)    (U8 Unit, const void * p);
  void  (*pfSetHWBlockLen)     (U8 Unit, U16 BlockSize);
  void  (*pfSetHWNumBlocks)    (U8 Unit, U16 NumBlocks);
  U16   (*pfGetMaxReadBurst)   (U8 Unit);
  U16   (*pfGetMaxWriteBurst)  (U8 Unit);
} FS_MMC_HW_TYPE_CM;

/*********************************************************************
*
*       Compatibility HW layers
*/
extern const FS_MMC_HW_TYPE_SPI     FS_MMC_HW_SPI_Default;
extern const FS_MMC_HW_TYPE_CM      FS_MMC_HW_CM_Default;

/*********************************************************************
*
*       FS_MMC_Driver API functions
*/
void FS_MMC_ActivateCRC          (void);
void FS_MMC_DeactivateCRC        (void);
int  FS_MMC_GetCardId            (U8 Unit, MMC_CARD_ID * pCardId);
void FS_MMC_GetStatCounters      (U8 Unit, FS_MMC_STAT_COUNTERS * pStat);
void FS_MMC_ResetStatCounters    (U8 Unit);
void FS_MMC_SetHWType            (U8 Unit, const FS_MMC_HW_TYPE_SPI * pHWType);

/*********************************************************************
*
*       FS_MMC_CardMode_Driver API functions
*/
void FS_MMC_CM_Allow4bitMode     (U8 Unit, U8 OnOff);
void FS_MMC_CM_Allow8bitMode     (U8 Unit, U8 OnOff);
void FS_MMC_CM_AllowHighSpeedMode(U8 Unit, U8 OnOff);
void FS_MMC_CM_AllowReliableWrite(U8 Unit, U8 OnOff);
int  FS_MMC_CM_Erase             (U8 Unit, U32 StartSector, U32 NumSectors);
int  FS_MMC_CM_GetCardId         (U8 Unit, MMC_CARD_ID * pCardId);
int  FS_MMC_CM_GetCardInfo       (U8 Unit, FS_MMC_CARD_INFO * pCardInfo);
void FS_MMC_CM_GetStatCounters   (U8 Unit, FS_MMC_STAT_COUNTERS * pStat);
int  FS_MMC_CM_ReadExtCSD        (U8 Unit, U32 * pBuffer);
void FS_MMC_CM_ResetStatCounters (U8 Unit);
void FS_MMC_CM_SetHWType         (U8 Unit, const FS_MMC_HW_TYPE_CM * pHWType);
int  FS_MMC_CM_UnlockCardForced  (U8 Unit);
int  FS_MMC_CM_WriteExtCSD       (U8 Unit, unsigned Off, const U8 * pData, unsigned NumBytes);

/*********************************************************************
*
*       IDE/CF driver
*
**********************************************************************
*/

/*********************************************************************
*
*       FS_IDE_HW_TYPE
*
*  Hardware layer API
*/
typedef struct FS_IDE_HW_TYPE {
  void (*pfReset)       (U8 Unit);
  int  (*pfIsPresent)   (U8 Unit);
  void (*pfDelay400ns)  (U8 Unit);
  U16  (*pfReadReg)     (U8 Unit, unsigned AddrOff);
  void (*pfWriteReg)    (U8 Unit, unsigned AddrOff, U16 Data);
  void (*pfReadData)    (U8 Unit,       U8 * pData, unsigned NumBytes);
  void (*pfWriteData)   (U8 Unit, const U8 * pData, unsigned NumBytes);
} FS_IDE_HW_TYPE;

/*********************************************************************
*
*       Compatibility HW layer
*/
extern const FS_IDE_HW_TYPE FS_IDE_HW_Default;

/*********************************************************************
*
*       API functions
*/
void FS_IDE_Configure(U8 Unit, U8 IsSlave);
void FS_IDE_SetHWType(U8 Unit, const FS_IDE_HW_TYPE * pHWType);

/*********************************************************************
*
*       NAND driver
*
**********************************************************************
*/

/*********************************************************************
*
*       NAND block types
*/
#define FS_NAND_BLOCK_TYPE_UNKNOWN              0
#define FS_NAND_BLOCK_TYPE_BAD                  1
#define FS_NAND_BLOCK_TYPE_EMPTY                2
#define FS_NAND_BLOCK_TYPE_WORK                 3
#define FS_NAND_BLOCK_TYPE_DATA                 4

/*********************************************************************
*
*       Return values for FS_NAND_TestBlock()/FS_NAND_UNI_TestBlock()
*/
#define FS_NAND_TEST_RETVAL_OK                  0
#define FS_NAND_TEST_RETVAL_CORRECTABLE_ERROR   1
#define FS_NAND_TEST_RETVAL_FATAL_ERROR         2
#define FS_NAND_TEST_RETVAL_BAD_BLOCK           3
#define FS_NAND_TEST_RETVAL_ERASE_FAILURE       4
#define FS_NAND_TEST_RETVAL_WRITE_FAILURE       5
#define FS_NAND_TEST_RETVAL_READ_FAILURE        6
#define FS_NAND_TEST_RETVAL_INTERNAL_ERROR      (-1)

/*********************************************************************
*
*       ECC correction status
*/
#define FS_NAND_CORR_NOT_APPLIED                0    // No bit errors detected
#define FS_NAND_CORR_APPLIED                    1    // Bit errors were detected and corrected
#define FS_NAND_CORR_FAILURE                    2    // Bit errors were detected but not corrected

/*********************************************************************
*
*       FS_NAND_SECTOR_INFO
*
*   Information about a logical sector stored on the NAND flash.
*/
typedef struct FS_NAND_SECTOR_INFO {
  U16 brsi;              // Block relative sector index.
  U32 ECC;               // ECC stored for this sector.
  U8  IsValid;           // Sector contains valid data.
} FS_NAND_SECTOR_INFO;

/*********************************************************************
*
*       FS_NAND_BLOCK_INFO
*
*   Information about a NAND flash block.
*/
typedef struct FS_NAND_BLOCK_INFO {
  U32          EraseCnt;                  // Number of times the block has been erased
  U32          lbi;                       // Logical block index assigned to queried physical block.
  U16          NumSectorsBlank;           // Sectors are not used yet.
  U16          NumSectorsValid;           // Sectors contain correct data.
  U16          NumSectorsInvalid;         // Sectors have been invalidated.
  U16          NumSectorsECCError;        // Sectors have incorrect ECC.
  U16          NumSectorsECCCorrectable;  // Sectors have correctable ECC error.
  U16          NumSectorsErrorInECC;      // Sectors have error in ECC.
  const char * sType;                     // Zero-terminated string holding the block type.
  U8           Type;                      // Type of data stored in the block (see the FS_NAND_BLOCK_TYPE_... defines).
} FS_NAND_BLOCK_INFO;

/*********************************************************************
*
*       FS_NAND_DISK_INFO
*
*   Information about the NAND flash.
*/
typedef struct FS_NAND_DISK_INFO {
  U32 NumPhyBlocks;
  U32 NumLogBlocks;
  U32 NumUsedPhyBlocks;
  U32 NumBadPhyBlocks;
  U32 NumPagesPerBlock;
  U32 NumSectorsPerBlock;
  U32 BytesPerPage;
  U32 BytesPerSpareArea;
  U32 BytesPerSector;
  U32 EraseCntMin;
  U32 EraseCntMax;
  U32 EraseCntAvg;
  U8  IsWriteProtected;
  U8  HasFatalError;
  U8  ErrorType;
  U32 ErrorSectorIndex;
  U16 BlocksPerGroup;
  U32 NumWorkBlocks;
} FS_NAND_DISK_INFO;

/*********************************************************************
*
*       FS_NAND_ECC_INFO
*
*   Information about the ECC used to protect data stored on the NAND flash.
*/
typedef struct FS_NAND_ECC_INFO {
  U8 NumBitsCorrectable;                // Number of bits the ECC should be able to correct
  U8 ldBytesPerBlock;                   // Number of bytes the ECC should protect
} FS_NAND_ECC_INFO;

/*********************************************************************
*
*       FS_NAND_ECC_RESULT
*
*   Information about the ECC number of bits corrected in an ECC block.
*/
typedef struct FS_NAND_ECC_RESULT {
  U8 CorrectionStatus;                  // Indicates if the correction succeeded or failed (FS_NAND_CORR_STATUS_...).
  U8 MaxNumBitsCorrected;               // Maximum number of bit errors detected and corrected in any ECC block of a page.
} FS_NAND_ECC_RESULT;

/*********************************************************************
*
*       FS_NAND_DEVICE_INFO
*
*   Information about the NAND flash device.
*/
typedef struct FS_NAND_DEVICE_INFO {
  U8               BPP_Shift;           // Bytes per page shift: 9 for 512 bytes/page or 11 for 2048 bytes/page
  U8               PPB_Shift;           // Pages per block shift: 32 -> 5, 64 -> 6
  U16              NumBlocks;           // Number of blocks in device
  U16              BytesPerSpareArea;   // Number of bytes in the spare area. Usually this is (1 << BPP_Shift)/32. Micron MT29F8G08ABABA has a spare area of 224 bytes for 4096 bytes/page.
  FS_NAND_ECC_INFO ECC_Info;            // Required ECC capability
} FS_NAND_DEVICE_INFO;

/*********************************************************************
*
*       FS_NAND_STAT_COUNTERS
*
*   Statistic counters of NAND flash driver.
*/
typedef struct FS_NAND_STAT_COUNTERS {
  U32 NumFreeBlocks;
  U32 NumBadBlocks;
  U32 EraseCnt;
  U32 ReadDataCnt;
  U32 ReadSpareCnt;
  U32 ReadSectorCnt;
  U32 NumReadRetries;
  U32 WriteDataCnt;
  U32 WriteSpareCnt;
  U32 WriteSectorCnt;
  U32 ConvertViaCopyCnt;
  U32 ConvertInPlaceCnt;
  U32 NumValidSectors;
  U32 CopySectorCnt;
  U32 BlockRelocationCnt;
} FS_NAND_STAT_COUNTERS;

/*********************************************************************
*
*       FS_NAND_FATAL_ERROR_INFO
*
*   Information passed to callback function when a fatal error happens.
*/
typedef struct FS_NAND_FATAL_ERROR_INFO {
  U8 Unit;
} FS_NAND_FATAL_ERROR_INFO;

/*********************************************************************
*
*       FS_NAND_TEST_INFO
*
*   Additional information passed to test routine.
*   The test routine returns information about what went wrong during the test.
*/
typedef struct FS_NAND_TEST_INFO {
  //
  // Input (required only for FS_NAND_UNI_TestBlock)
  //
  U8  NumBitsCorrectable;     // Number of bits the ECC can correct in the data and spare area (typ. 4)
  U8  OffSpareECCProt;        // Offset in the spare area of the first byte protected by ECC (typ. 4).
  U8  NumBytesSpareECCProt;   // Number of bytes in the spare area protected by ECC (typ. 4 bytes)
  U16 BytesPerSpare;          // Total number of bytes in the spare area. When set to 0 the default value of 1/32 of page size is used.
  //
  // Output
  //
  U32 BitErrorCnt;            // Number of bit errors detected and corrected.
  U32 PageIndex;              // Index of the physical page where the error happened.
} FS_NAND_TEST_INFO;

typedef int FS_NAND_ON_FATAL_ERROR_CALLBACK(FS_NAND_FATAL_ERROR_INFO * pFatalErrorInfo);

/*********************************************************************
*
*       FS_NAND_ECC_HOOK
*
*   External ECC computation module.
*/
typedef struct FS_NAND_ECC_HOOK {
  void      (*pfCompute)        (const U32 * pData, U8 * pSpare);
  int       (*pfApply)          (      U32 * pData, U8 * pSpare);
  unsigned  NumBitsCorrectable; // Number of bits the ECC algorithm can correct per 512 bytes.
} FS_NAND_ECC_HOOK;

/*********************************************************************
*
*       FS_NAND_PHY_TYPE
*
*   API structure of a NAND physical layer.
*/
typedef struct FS_NAND_PHY_TYPE {
  int (*pfEraseBlock)       (U8 Unit, U32 Block);
  int (*pfInitGetDeviceInfo)(U8 Unit, FS_NAND_DEVICE_INFO * pDevInfo);
  int (*pfIsWP)             (U8 Unit);
  int (*pfRead)             (U8 Unit, U32 PageIndex,       void * pData, unsigned Off, unsigned NumBytes);
  int (*pfReadEx)           (U8 Unit, U32 PageIndex,       void * pData, unsigned Off, unsigned NumBytes,       void * pSpare, unsigned OffSpare, unsigned NumBytesSpare);
  int (*pfWrite)            (U8 Unit, U32 PageIndex, const void * pData, unsigned Off, unsigned NumBytes);
  int (*pfWriteEx)          (U8 Unit, U32 PageIndex, const void * pData, unsigned Off, unsigned NumBytes, const void * pSpare, unsigned OffSpare, unsigned NumBytesSpare);
  int (*pfEnableECC)        (U8 Unit);
  int (*pfDisableECC)       (U8 Unit);
  int (*pfConfigureECC)     (U8 Unit, U8 NumBitsCorrectable, U16 BytesPerECCBlock);
  int (*pfCopyPage)         (U8 Unit, U32 PageIndexSrc, U32 PageIndexDest);
  int (*pfGetECCResult)     (U8 Unit, FS_NAND_ECC_RESULT * pResult);
} FS_NAND_PHY_TYPE;

/*********************************************************************
*
*       FS_NAND_HW_TYPE
*
*   API structure of the HW layer for NAND flash devices connected via parallel I/O.
*/
typedef struct FS_NAND_HW_TYPE {
  void    (*pfInit_x8)        (U8 Unit);
  void    (*pfInit_x16)       (U8 Unit);
  void    (*pfDisableCE)      (U8 Unit);
  void    (*pfEnableCE)       (U8 Unit);
  void    (*pfSetAddrMode)    (U8 Unit);
  void    (*pfSetCmdMode)     (U8 Unit);
  void    (*pfSetDataMode)    (U8 Unit);
  int     (*pfWaitWhileBusy)  (U8 Unit, unsigned us);
  void    (*pfRead_x8)        (U8 Unit,       void * pBuffer, unsigned NumBytes);
  void    (*pfWrite_x8)       (U8 Unit, const void * pBuffer, unsigned NumBytes);
  void    (*pfRead_x16)       (U8 Unit,       void * pBuffer, unsigned NumBytes);
  void    (*pfWrite_x16)      (U8 Unit, const void * pBuffer, unsigned NumBytes);
} FS_NAND_HW_TYPE;

/*********************************************************************
*
*       FS_NAND_HW_TYPE_SPI
*
*   API structure of the HW layer for NAND flash devices connected via SPI.
*/
typedef struct FS_NAND_HW_TYPE_SPI {
  int     (*pfInit)           (U8 Unit);
  void    (*pfDisableCS)      (U8 Unit);
  void    (*pfEnableCS)       (U8 Unit);
  void    (*pfDelay)          (U8 Unit, int ms);
  int     (*pfRead)           (U8 Unit,       void * pData, unsigned NumBytes);
  int     (*pfWrite)          (U8 Unit, const void * pData, unsigned NumBytes);
} FS_NAND_HW_TYPE_SPI;

/*********************************************************************
*
*       FS_NAND_HW_TYPE_DF
*
*   API structure of the HW layer for DataFlash devices.
*/
typedef struct FS_NAND_HW_TYPE_DF {
  int     (*pfInit)           (U8 Unit);
  void    (*pfEnableCS)       (U8 Unit);
  void    (*pfDisableCS)      (U8 Unit);
  void    (*pfRead)           (U8 Unit,       U8 * pData, int NumBytes);
  void    (*pfWrite)          (U8 Unit, const U8 * pData, int NumBytes);
} FS_NAND_HW_TYPE_DF;

/*********************************************************************
*
*       Available physical layers
*/
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_x;          // 512 or 2048 byte pages,  8-bit or 16-bit interface
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_x8;         // 512 or 2048 byte pages,  8-bit interface
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_512x8;      // 512 byte pages, 8-bit interface
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_2048x8;     // 2048 byte pages, 8-bit interface
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_2048x16;    // 2048 byte pages, 16-bit interface
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_4096x8;     // 4096 byte pages, 8-bit interface
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_DataFlash;  // Physical layer for ATMEL serial DataFlash
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_ONFI;       // Physical layer for NAND flashes which support ONFI
extern const FS_NAND_PHY_TYPE FS_NAND_PHY_SPI;        // Physical layer for NAND flashes with SPI interface

/*********************************************************************
*
*       Default HW layers for the NAND flash drivers.
*/
extern const FS_NAND_HW_TYPE      FS_NAND_HW_Default;
extern const FS_NAND_HW_TYPE_SPI  FS_NAND_HW_SPI_Default;
extern const FS_NAND_HW_TYPE_DF   FS_NAND_HW_DF_Default;

/*********************************************************************
*
*       ECC computation modules
*/
extern const FS_NAND_ECC_HOOK FS_NAND_ECC_HW_NULL;    // Uses the HW ECC of the NAND flash device
extern const FS_NAND_ECC_HOOK FS_NAND_ECC_HW_4BIT;    // Uses the HW ECC of the NAND flash controller
extern const FS_NAND_ECC_HOOK FS_NAND_ECC_HW_8BIT;    // Uses the HW ECC of the NAND flash controller
extern const FS_NAND_ECC_HOOK FS_NAND_ECC_SW_1BIT;    // 1-bit SW ECC over 512 bytes of page and 4 bytes of spare area

/*********************************************************************
*
*       SLC1 NAND driver functions
*/
void FS_NAND_EraseFlash             (U8 Unit);
int  FS_NAND_FormatLow              (U8 Unit);
void FS_NAND_GetBlockInfo           (U8 Unit, U32 PhyBlockIndex, FS_NAND_BLOCK_INFO * pBlockInfo);
void FS_NAND_GetDiskInfo            (U8 Unit, FS_NAND_DISK_INFO * pDiskInfo);
void FS_NAND_GetSectorInfo          (U8 Unit, U32 PhySectorIndex, FS_NAND_SECTOR_INFO * pBlockInfo);
void FS_NAND_GetStatCounters        (U8 Unit, FS_NAND_STAT_COUNTERS * pStat);
int  FS_NAND_IsLLFormatted          (U8 Unit);
int  FS_NAND_ReadPhySector          (U8 Unit, U32 PhySectorIndex, void * pData, unsigned * pNumBytesData, void * pSpare, unsigned * pNumBytesSpare);
void FS_NAND_ResetStatCounters      (U8 Unit);
void FS_NAND_SetBlockRange          (U8 Unit, U16 FirstBlock, U16 MaxNumBlocks);
void FS_NAND_SetPhyType             (U8 Unit, const FS_NAND_PHY_TYPE * pPhyType);
void FS_NAND_SetMaxEraseCntDiff     (U8 Unit, U32 EraseCntDiff);
void FS_NAND_SetNumWorkBlocks       (U8 Unit, U32 NumWorkBlocks);
void FS_NAND_SetOnFatalErrorCallback(FS_NAND_ON_FATAL_ERROR_CALLBACK * pfOnFatalError);
int  FS_NAND_TestBlock              (U8 Unit, unsigned BlockIndex, U32 Pattern, FS_NAND_TEST_INFO * pInfo);

/*********************************************************************
*
*       Universal NAND driver functions
*/
void FS_NAND_UNI_AllowBlankUnusedSectors(U8 Unit, U8 OnOff);
int  FS_NAND_UNI_EraseFlash             (U8 Unit);
int  FS_NAND_UNI_Clean                  (U8 Unit, unsigned NumBlocksFree, unsigned NumSectorsFree);
int  FS_NAND_UNI_GetBlockInfo           (U8 Unit, U32 PhysBlockIndex, FS_NAND_BLOCK_INFO * pBlockInfo);
int  FS_NAND_UNI_GetDiskInfo            (U8 Unit, FS_NAND_DISK_INFO * pDiskInfo);
void FS_NAND_UNI_GetStatCounters        (U8 Unit, FS_NAND_STAT_COUNTERS * pStat);
int  FS_NAND_UNI_ReadPhySector          (U8 Unit, U32 PhySectorIndex, void * pData, unsigned * pNumBytesData, void * pSpare, unsigned * pNumBytesSpare);
void FS_NAND_UNI_ResetStatCounters      (U8 Unit);
int  FS_NAND_UNI_SetCleanThreshold      (U8 Unit, unsigned NumBlocksFree, unsigned NumSectorsFree);
void FS_NAND_UNI_SetECCHook             (U8 Unit, const FS_NAND_ECC_HOOK * pECCHook);
void FS_NAND_UNI_SetBlockRange          (U8 Unit, U16 FirstBlock, U16 MaxNumBlocks);
void FS_NAND_UNI_SetMaxBitErrorCnt      (U8 Unit, unsigned BitErrorCnt);
void FS_NAND_UNI_SetMaxEraseCntDiff     (U8 Unit, U32 EraseCntDiff);
void FS_NAND_UNI_SetNumWorkBlocks       (U8 Unit, U32 NumWorkBlocks);
void FS_NAND_UNI_SetNumBlocksPerGroup   (U8 Unit, unsigned BlocksPerGroup);
void FS_NAND_UNI_SetOnFatalErrorCallback(FS_NAND_ON_FATAL_ERROR_CALLBACK * pfOnFatalError);
void FS_NAND_UNI_SetPhyType             (U8 Unit, const FS_NAND_PHY_TYPE * pPhyType);
int  FS_NAND_UNI_TestBlock              (U8 Unit, unsigned BlockIndex, U32 Pattern, FS_NAND_TEST_INFO * pInfo);

/*********************************************************************
*
*       NAND physical layer specific functions
*/
void FS_NAND_PHY_ReadDeviceId           (U8 Unit, U8 * pId, U32 NumBytes);
int  FS_NAND_PHY_ReadONFIPara           (U8 Unit, void * pPara);
void FS_NAND_PHY_SetHWType              (U8 Unit, const FS_NAND_HW_TYPE * pHWType);

/*********************************************************************
*
*       2048x16 physical layer
*/
void FS_NAND_2048x16_SetHWType          (U8 Unit, const FS_NAND_HW_TYPE * pHWType);

/*********************************************************************
*
*       2048x8 physical layer
*/
void FS_NAND_2048x8_EnableReadCache     (U8 Unit);
void FS_NAND_2048x8_DisableReadCache    (U8 Unit);
void FS_NAND_2048x8_SetHWType           (U8 Unit, const FS_NAND_HW_TYPE * pHWType);

/*********************************************************************
*
*       4096x8 physical layer
*/
void FS_NAND_4096x8_SetHWType           (U8 Unit, const FS_NAND_HW_TYPE * pHWType);

/*********************************************************************
*
*       512x8 physical layer
*/
void FS_NAND_512x8_SetHWType            (U8 Unit, const FS_NAND_HW_TYPE * pHWType);

/*********************************************************************
*
*       x8 physical layer
*/
void FS_NAND_x8_SetHWType               (U8 Unit, const FS_NAND_HW_TYPE * pHWType);

/*********************************************************************
*
*       x physical layer
*/
void FS_NAND_x_SetHWType                (U8 Unit, const FS_NAND_HW_TYPE * pHWType);

/*********************************************************************
*
*       ONFI physical layer
*/
void FS_NAND_ONFI_SetHWType             (U8 Unit, const FS_NAND_HW_TYPE * pHWType);

/*********************************************************************
*
*       SPI physical layer
*/
void FS_NAND_SPI_EnableReadCache        (U8 Unit);
void FS_NAND_SPI_DisableReadCache       (U8 Unit);
void FS_NAND_SPI_SetHWType              (U8 Unit, const FS_NAND_HW_TYPE_SPI * pHWType);

/*********************************************************************
*
*       DataFlash physical layer
*/
void FS_NAND_DF_EraseChip               (U8 Unit);
void FS_NAND_DF_SetMinPageSize          (U8 Unit, U32 NumBytes);
void FS_NAND_DF_SetHWType               (U8 Unit, const FS_NAND_HW_TYPE_DF * pHWType);

/*********************************************************************
*
*       WinDrive driver
*
**********************************************************************
*/
void FS_WINDRIVE_Configure(U8 Unit, const char * sWindowsDriveName);

/*********************************************************************
*
*       Logical drivers
*/
int  FS_AddPhysDevice     (const FS_DEVICE_TYPE * pDevType);

/*********************************************************************
*
*       Logical volume
*/
int  FS_LOGVOL_Create     (const char * sVolName);
int  FS_LOGVOL_AddDevice  (const char * sLogVolName, const FS_DEVICE_TYPE * pDevice, U8 Unit, U32 StartOff, U32 NumSectors);

/*********************************************************************
*
*       MBR disk partition
*/
void FS_DISKPART_Configure(U8 Unit, const FS_DEVICE_TYPE * pDevice, U8 DeviceUnit, U8 PartIndex);

/*********************************************************************
*
*       Encryption
*/
typedef struct {
  void (*pfPrepare)(void * pContext, const U8 * pKey);
  void (*pfEncrypt)(void * pContext, U8 * pDest, const U8 * pSrc, U32 NumBytes, U32 BlockIndex);
  void (*pfDecrypt)(void * pContext, U8 * pDest, const U8 * pSrc, U32 NumBytes, U32 BlockIndex);
  U16    BitsPerBlock;     // Number of bits encrypted at once by the algorithm: AES -> 128, DES -> 64.
} FS_CRYPT_ALGO_TYPE;

/*********************************************************************
*
*       FS_CRYPT_OBJ
*
*   FS internal structure. Describes an encryption object.
*/
typedef struct {
  void                     * pContext;
  const FS_CRYPT_ALGO_TYPE * pAlgoType;
  U16                        ldBytesPerBlock;
} FS_CRYPT_OBJ;

/*********************************************************************
*
*       FS_DES_CONTEXT
*
*   FS internal structure. Describes the context of the DES encryption algorithm.
*/
typedef struct {
  U32 _aRoundKey[32];
} FS_DES_CONTEXT;

/*********************************************************************
*
*       FS_AES_CONTEXT
*
*   FS internal structure. Describes the context of the AES encryption algorithm.
*/
typedef struct {
  U32 aRoundKeyEnc[60];
  U32 aRoundKeyDec[60];
} FS_AES_CONTEXT;

extern const FS_CRYPT_ALGO_TYPE   FS_CRYPT_ALGO_DES;      // 56-bit DES encryption
extern const FS_CRYPT_ALGO_TYPE   FS_CRYPT_ALGO_AES128;   // 128-bit AES encryption
extern const FS_CRYPT_ALGO_TYPE   FS_CRYPT_ALGO_AES256;   // 256-bit AES encryption

void FS_CRYPT_Configure(U8 Unit, const FS_DEVICE_TYPE * pDeviceType, U8 DeviceUnit, const FS_CRYPT_ALGO_TYPE * pAlgoType, void * pContext, const U8 * pKey);
void FS_CRYPT_Prepare  (FS_CRYPT_OBJ * pCryptObj, const FS_CRYPT_ALGO_TYPE * pAlgoType, void * pContext, U32 BytesPerBlock, const U8 * pKey);
void FS_CRYPT_Encrypt  (const FS_CRYPT_OBJ * pCryptObj, U8 * pDest, const U8 * pSrc, U32 NumBytes, U32 * pBlockIndex);
void FS_CRYPT_Decrypt  (const FS_CRYPT_OBJ * pCryptObj, U8 * pDest, const U8 * pSrc, U32 NumBytes, U32 * pBlockIndex);

#if FS_SUPPORT_ENCRYPTION
  int FS_SetEncryptionObject(FS_FILE * pFile, FS_CRYPT_OBJ * pCryptObj);
#endif

/*********************************************************************
*
*       Encryption performance test routines
*/
void FS_CRYPT_DES_Encrypt1MB   (void);
void FS_CRYPT_DES_Decrypt1MB   (void);
void FS_CRYPT_AES256_Encrypt1MB(void);
void FS_CRYPT_AES256_Decrypt1MB(void);
void FS_CRYPT_AES128_Encrypt1MB(void);
void FS_CRYPT_AES128_Decrypt1MB(void);

/*********************************************************************
*
*       Sector read ahead driver
*/
typedef struct {
  U32 ReadSectorCnt;
  U32 ReadSectorCachedCnt;
} FS_READAHEAD_STAT_COUNTERS;

void FS_READAHEAD_Configure        (U8 Unit, const FS_DEVICE_TYPE * pDevice, U8 DeviceUnit, U32 * pData, U32 NumBytes);
void FS_READAHEAD_GetStatCounters  (U8 Unit, FS_READAHEAD_STAT_COUNTERS * pStat);
void FS_READAHEAD_ResetStatCounters(U8 Unit);

/*********************************************************************
*
*       Disk mirroring driver
*/
void FS_RAID1_Configure      (U8 Unit, const FS_DEVICE_TYPE * pDeviceType0, U8 DeviceUnit0, const FS_DEVICE_TYPE * pDeviceType1, U8 DeviceUnit1);
void FS_RAID1_SetSectorRanges(U8 Unit, U32 NumSectors, U32 StartSector0, U32 StartSector1);
void FS_RAID1_SetSyncBuffer  (U8 Unit, void * pBuffer, U32 NumBytes);
void FS_RAID1_SetSyncSource  (U8 Unit, unsigned StorageIndex);

/*********************************************************************
*
*       Sector size conversion driver
*/
void FS_SECSIZE_Configure(U8 Unit, const FS_DEVICE_TYPE * pDeviceType, U8 DeviceUnit, U16 BytesPerSector);

/*********************************************************************
*
*       Sector write buffer
*/

/*********************************************************************
*
*       FS_WRBUF_SECTOR_INFO
*
*  FS internal structure. One instance per sector.
*/
typedef struct FS_WRBUF_SECTOR_INFO {
  U32      SectorIndex;
  unsigned IsValid;
} FS_WRBUF_SECTOR_INFO;

/*********************************************************************
*
*       FS_WRBUF_SIZEOF
*
*  Computes the number of bytes required to store the specified number of sectors.
*/
#define FS_SIZEOF_WRBUF(NumSectors, BytesPerSector)     ((sizeof(FS_WRBUF_SECTOR_INFO) + (BytesPerSector)) * NumSectors)

void FS_WRBUF_Configure(U8 Unit, const FS_DEVICE_TYPE * pDeviceType, U8 DeviceUnit, void * pBuffer, U32 NumBytes);

/*********************************************************************
*
*       Cache handling
*/
#if FS_SUPPORT_CACHE

/*********************************************************************
*
*       General Cache mode defines, do not use in application
*/
#define FS_CACHE_MODE_R         0x01
#define FS_CACHE_MODE_W         0x02
#define FS_CACHE_MODE_D         0x04

#define FS_CACHE_MODE_FULL      (FS_CACHE_MODE_R | FS_CACHE_MODE_W | FS_CACHE_MODE_D)   // Obsolete Cache mode define, do not use

/*********************************************************************
*
*       Cache mode defines that can be used in application
*/
#define FS_CACHE_MODE_WT        (FS_CACHE_MODE_R | FS_CACHE_MODE_W)
#define FS_CACHE_MODE_WB        (FS_CACHE_MODE_R | FS_CACHE_MODE_W | FS_CACHE_MODE_D)

/*********************************************************************
*
*       Cache specific defines
*/
#define FS_CACHE_NONE      NULL
#define FS_CACHE_MAN       FS_CacheMan_Init
#define FS_CACHE_RW        FS_CacheRW_Init
#define FS_CACHE_RW_QUOTA  FS_CacheRWQuota_Init
#define FS_CACHE_MULTI_WAY FS_CacheMultiWay_Init
#define FS_CACHE_ALL       FS_CacheAll_Init

/*********************************************************************
*
*       Defines to compute the cache size
*/
#define FS_SIZEOF_CACHE_ALL_DATA                12    // sizeof(CACHE_ALL_DATA)
#define FS_SIZEOF_CACHE_ALL_BLOCK_INFO          4     // sizeof(CACHE_ALL_BLOCK_INFO)
#define FS_SIZEOF_CACHE_MAN_DATA                12    // sizeof(CACHE_MAN_DATA)
#define FS_SIZEOF_CACHE_MAN_BLOCK_INFO          4     // sizeof(CACHE_MAN_BLOCK_INFO)
#define FS_SIZEOF_CACHE_RW_DATA                 16    // sizeof(CACHE_RW_DATA)
#define FS_SIZEOF_CACHE_RW_BLOCK_INFO           8     // sizeof(CACHE_RW_BLOCK_INFO)
#define FS_SIZEOF_CACHE_RW_QUOTA_DATA           52    // sizeof(CACHE_RW_QUOTA_DATA)
#define FS_SIZEOF_CACHE_RW_QUOTA_BLOCK_INFO     8     // sizeof(CACHE_RW_QUOTA_BLOCK_INFO)
#define FS_SIZEOF_CACHE_MULTI_WAY_DATA          20    // sizeof(CACHE_MULTI_WAY_DATA)
#define FS_SIZEOF_CACHE_MULTI_WAY_BLOCK_INFO    8     // sizeof(CACHE_MULTI_WAY_BLOCK_INFO)

#define FS_SIZEOF_CACHE_ALL(NumSectors, SectorSize)         (FS_SIZEOF_CACHE_ALL_DATA + (FS_SIZEOF_CACHE_ALL_BLOCK_INFO + (SectorSize)) * (NumSectors))
#define FS_SIZEOF_CACHE_MAN(NumSectors, SectorSize)         (FS_SIZEOF_CACHE_MAN_DATA + (FS_SIZEOF_CACHE_MAN_BLOCK_INFO + (SectorSize)) * (NumSectors))
#define FS_SIZEOF_CACHE_RW(NumSectors, SectorSize)          (FS_SIZEOF_CACHE_RW_DATA + (FS_SIZEOF_CACHE_RW_BLOCK_INFO + (SectorSize)) * (NumSectors))
#define FS_SIZEOF_CACHE_RW_QUOTA(NumSectors, SectorSize)    (FS_SIZEOF_CACHE_RW_QUOTA_DATA + (FS_SIZEOF_CACHE_RW_QUOTA_BLOCK_INFO + (SectorSize)) * (NumSectors))
#define FS_SIZEOF_CACHE_MULTI_WAY(NumSectors, SectorSize)   (FS_SIZEOF_CACHE_MULTI_WAY_DATA + (FS_SIZEOF_CACHE_MULTI_WAY_BLOCK_INFO + (SectorSize)) * (NumSectors))
#define FS_SIZEOF_CACHE_ANY(NumSectors, SectorSize)         FS_SIZEOF_CACHE_RW_QUOTA(NumSectors, SectorSize)

/*********************************************************************
*
*       Cache specific types
*/
typedef U32    FS_INIT_CACHE (FS_DEVICE * pDevice, void * pData, I32 NumBytes);
typedef U32 (* FS_CACHE_TYPE)(FS_DEVICE * pDevice, void * pData, I32 NumBytes);

/*********************************************************************
*
*       Cache specific prototypes
*/
U32    FS_AssignCache        (const char * pVolumeName, void * pData, I32 NumBytes, FS_INIT_CACHE * pfInit);
void   FS_CACHE_Clean        (const char * pVolumeName);
int    FS_CACHE_Command      (const char * pVolumeName, int   Cmd,      void * pData);
int    FS_CACHE_SetMode      (const char * pVolumeName, int   TypeMask, int    ModeMask);
int    FS_CACHE_SetQuota     (const char * pVolumeName, int   TypeMask, U32    NumSectors);
int    FS_CACHE_SetAssocLevel(const char * pVolumeName, int   AssocLevel);
int    FS_CACHE_GetNumSectors(const char * pVolumeName, U32 * pNumSectors);
int    FS_CACHE_Invalidate   (const char * sVolumeName);

/*********************************************************************
*
*       Cache initialization prototypes
*/
U32 FS_CacheAll_Init     (FS_DEVICE * pDev, void * pData, I32 NumBytes);
U32 FS_CacheMan_Init     (FS_DEVICE * pDev, void * pData, I32 NumBytes);
U32 FS_CacheRW_Init      (FS_DEVICE * pDev, void * pData, I32 NumBytes);
U32 FS_CacheRWQuota_Init (FS_DEVICE * pDev, void * pData, I32 NumBytes);
U32 FS_CacheMultiWay_Init(FS_DEVICE * pDev, void * pData, I32 NumBytes);

#endif // FS_SUPPORT_CACHE

/*********************************************************************
*
*       File system selection prototype
*/
#if FS_SUPPORT_MULTIPLE_FS
  int FS_SetFSType(const char * sVolumeName, int FSType);
  int FS_GetFSType(const char * sVolumeName);
#endif // FS_SUPPORT_MULTIPLE_FS

/*********************************************************************
*
*       Memory allocation functions
*/
void * FS_Alloc         (I32     NumBytes);
void * FS_AllocZeroed   (I32     NumBytes);
void   FS_Free          (void *  p);
void   FS_AllocZeroedPtr(void ** pp, I32 NumBytes);
void * FS_TryAlloc      (I32     NumBytesReq);
void * FS_GetFreeMem    (I32   * pNumBytes);

/*********************************************************************
*
*       File system locking
*/
#if FS_OS_LOCKING
  void FS_Lock        (void);
  void FS_Unlock      (void);
  void FS_LockVolume  (const char * sVolumeName);
  void FS_UnlockVolume(const char * sVolumeName);
#endif // FS_OS_LOCKING

/*********************************************************************
*
*       Instrumentation
*/
typedef struct FS_PROFILE_API {
  void (*pfRecordEndCall)   (unsigned EventId);
  void (*pfRecordEndCallU32)(unsigned EventId, U32 Para0);
  void (*pfRecordVoid)      (unsigned EventId);
  void (*pfRecordU32)       (unsigned EventId, U32 Para0);
  void (*pfRecordU32x2)     (unsigned EventId, U32 Para0, U32 Para1);
  void (*pfRecordU32x3)     (unsigned EventId, U32 Para0, U32 Para1, U32 Para2);
  void (*pfRecordU32x4)     (unsigned EventId, U32 Para0, U32 Para1, U32 Para2, U32 Para3);
  void (*pfRecordU32x5)     (unsigned EventId, U32 Para0, U32 Para1, U32 Para2, U32 Para3, U32 Para4);
  void (*pfRecordU32x6)     (unsigned EventId, U32 Para0, U32 Para1, U32 Para2, U32 Para3, U32 Para4, U32 Para5);
  void (*pfRecordU32x7)     (unsigned EventId, U32 Para0, U32 Para1, U32 Para2, U32 Para3, U32 Para4, U32 Para5, U32 Para6);
  void (*pfRecordString)    (unsigned EventId, const char * pPara0);
  void (*pfRecordStringx2)  (unsigned EventId, const char * pPara0, const char * pPara1);
} FS_PROFILE_API;

U32  FS_PROFILE_GetAPIDesc(const char ** psDesc);
void FS_PROFILE_SetAPI    (const FS_PROFILE_API * pAPI, U32 IdOffset);

#if FS_SUPPORT_PROFILE
  void FS_SYSVIEW_Init(void);
#endif

/*********************************************************************
*
*       Application supplied functions FS_X_...
*/

/*********************************************************************
*
*       Debug support
*/
void FS_X_Log     (const char *s);
void FS_X_Warn    (const char *s);
void FS_X_ErrorOut(const char *s);

/*********************************************************************
*
*       Misc. functions
*/
void FS_X_Panic      (int ErrorCode);
void FS_X_AddDevices (void);
U32  FS_X_GetTimeDate(void);

/*********************************************************************
*
*       Compatibility defines
*/

/*********************************************************************
*
*       Error codes
*/
#define FS_ERR_OK                       FS_ERRCODE_OK
#define FS_ERR_EOF                      FS_ERRCODE_EOF
#define FS_ERR_CMDNOTSUPPORTED          (-4)    // Deprecated
#define FS_ERR_DISKFULL                 FS_ERRCODE_VOLUME_FULL
#define FS_ERR_INVALIDPAR               FS_ERRCODE_INVALID_PARA
#define FS_ERR_WRITEONLY                FS_ERRCODE_WRITE_ONLY_FILE
#define FS_ERR_READONLY                 FS_ERRCODE_READ_ONLY_FILE
#define FS_ERR_READERROR                FS_ERRCODE_READ_FAILURE
#define FS_ERR_WRITEERROR               FS_ERRCODE_WRITE_FAILURE
#define FS_ERROR_ALLOC                  FS_ERRCODE_OUT_OF_MEMORY
#define FS_ERROR_INVALID_PARA           FS_ERRCODE_INVALID_PARA
#define FS_ERROR_UNKNOWN_DEVICE         FS_ERRCODE_UNKNOWN_DEVICE

/*********************************************************************
*
*       Cache API
*/
#define FS_CACHEALL_API             FS_CacheAll_Init
#define FS_CACHEMAN_API             FS_CacheMan_Init

/*********************************************************************
*
*       Configuration API
*/
#define FS_WriteUpdateDir(OnOff)                    FS_ConfigUpdateDirOnWrite(OnOff)
#define FS_ConfigFileEncryption(pFile, pCryptObj)   FS_SetEncryptionObject(pFile, pCryptObj)
#define FS_ConfigFileBufferFlags(pFile, Flags)      FS_SetFileBufferFlags(pFile, Flags)
#define WINDRIVE_Configure(Unit, sWindowsDriveName) FS_WINDRIVE_Configure(Unit, sWindowsDriveName)
#define FS_ConfigUpdateDirOnWrite(OnOff)            FS_ConfigOnWriteDirUpdate(OnOff)

/*********************************************************************
*
*       File API
*/
#define FS_FFlush(pFile)            FS_SyncFile(pFile)

/*********************************************************************
*
*       NAND flash driver
*/
#define FS_NAND_ON_FATAL_ERROR_CB                       FS_NAND_ON_FATAL_ERROR_CALLBACK
#define FS_NAND_SetOnFatalErrorCB(pfOnFatalError)       FS_NAND_SetOnFatalErrorCallback(pfOnFatalError)
#define FS_NAND_ReadDeviceId(Unit, pId, NumBytes)       FS_NAND_PHY_ReadDeviceId(Unit, pId, NumBytes)
#define FS_NAND_ECC_NULL                                FS_NAND_ECC_HW_NULL
#define FS_NAND_ECC_1BIT                                FS_NAND_ECC_SW_1BIT
#define FS_NAND_UNI_SetOnFatalErrorCB(pfOnFatalError)   FS_NAND_UNI_SetOnFatalErrorCallback(pfOnFatalError)
#define NAND_BLOCK_TYPE_UNKNOWN                         FS_NAND_BLOCK_TYPE_UNKNOWN
#define NAND_BLOCK_TYPE_BAD                             FS_NAND_BLOCK_TYPE_BAD
#define NAND_BLOCK_TYPE_EMPTY                           FS_NAND_BLOCK_TYPE_EMPTY
#define NAND_BLOCK_TYPE_WORK                            FS_NAND_BLOCK_TYPE_WORK
#define NAND_BLOCK_TYPE_DATA                            FS_NAND_BLOCK_TYPE_DATA
#define FS_DF_ChipErase(Unit)                           FS_NAND_DF_EraseChip(Unit)

/*********************************************************************
*
*       Journaling add-on
*/
#define FS_CreateJournal(sVolumeName, NumBytes)         FS_JOURNAL_Create(sVolumeName, NumBytes)
#define FS_BeginTransaction(sVolumeName)                FS_JOURNAL_Begin(sVolumeName)
#define FS_EndTransaction(sVolumeName)                  FS_JOURNAL_End(sVolumeName)

/*********************************************************************
*
*       Disk checking
*/
#define FS_ERRCODE_0FILE                      FS_CHECKDISK_ERRCODE_0FILE
#define FS_ERRCODE_SHORTEN_CLUSTER            FS_CHECKDISK_ERRCODE_SHORTEN_CLUSTER
#define FS_ERRCODE_CROSSLINKED_CLUSTER        FS_CHECKDISK_ERRCODE_CROSSLINKED_CLUSTER
#define FS_ERRCODE_FEW_CLUSTER                FS_CHECKDISK_ERRCODE_FEW_CLUSTER
#define FS_ERRCODE_CLUSTER_UNUSED             FS_CHECKDISK_ERRCODE_CLUSTER_UNUSED
#define FS_ERRCODE_CLUSTER_NOT_EOC            FS_CHECKDISK_ERRCODE_CLUSTER_NOT_EOC
#define FS_ERRCODE_INVALID_CLUSTER            FS_CHECKDISK_ERRCODE_INVALID_CLUSTER
#define FS_ERRCODE_INVALID_DIRECTORY_ENTRY    FS_CHECKDISK_ERRCODE_INVALID_DIRECTORY_ENTRY

#define FS_QUERY_F_TYPE                  FS_ON_CHECK_DISK_ERROR_CALLBACK
#define FS_ON_CHECK_DISK_ERROR_CALLBACK  FS_CHECKDISK_ON_ERROR_CALLBACK
#define FS_EFS_CheckDisk(sVolumeName, pBuffer, BufferSize, MaxRecursionLevel, pfOnError)  FS_CheckDisk(sVolumeName, pBuffer, BufferSize, MaxRecursionLevel, pfOnError)
#define FS_FAT_CheckDisk(sVolumeName, pBuffer, BufferSize, MaxRecursionLevel, pfOnError)  FS_CheckDisk(sVolumeName, pBuffer, BufferSize, MaxRecursionLevel, pfOnError)
#define FS_EFS_CheckDisk_ErrCode2Text(ErrCode)                                            FS_CheckDisk_ErrCode2Text(ErrCode)
#define FS_FAT_CheckDisk_ErrCode2Text(ErrCode)                                            FS_CheckDisk_ErrCode2Text(ErrCode)

/*********************************************************************
*
*       Space information functions
*/
#define FS_GetFreeSpace(sVolume)  FS_GetVolumeFreeSpace(sVolume)
#define FS_GetTotalSpace(sVolume) FS_GetVolumeSize(sVolume)

/*********************************************************************
*
*       FAT
*/
#define FS_FAT_ConfigMaintainFATCopy(OnOff)   FS_FAT_ConfigFATCopyMaintenance(OnOff)
#define FS_FAT_ConfigUseFSInfoSector(OnOff)   FS_FAT_ConfigFSInfoSectorUse(OnOff)

#if defined(__cplusplus)
  }              /* Make sure we have C-declarations in C++ programs */
#endif

#endif // __FS_H__

/*************************** End of file ****************************/
