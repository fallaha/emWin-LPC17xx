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
File        : FS_Int.h
Purpose     : Internals used across different layers of the file system
---------------------------END-OF-HEADER------------------------------
*/

#ifndef FS_INT_H                    // Avoid recursive and multiple inclusion
#define FS_INT_H

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "FS_Types.h"
#include "FS.h"
#include "FS_Debug.h"
#include "FS_ConfDefaults.h"        // FS Configuration
#include "FS_OS.h"

#if FS_SUPPORT_FAT
  #include "FAT.h"
#endif
#if FS_SUPPORT_EFS
  #include "EFS.h"
#endif

#if defined(__cplusplus)
extern "C" {                        // Make sure we have C-declarations in C++ programs
#endif

#ifdef FS_CORE_C
  #define EXTERN
#else
  #define EXTERN extern
#endif

/*********************************************************************
*
*       Global defines
*
**********************************************************************
*/
#ifndef COUNTOF
  #define COUNTOF(a)        (sizeof((a))/sizeof(((a)[0])))
#endif
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define ZEROFILL(p, Size)   (memset(p, 0, Size))

#define CLUSTER_ID_INVALID            0xFFFFFFFF
#define FS_MAX_FILE_SIZE              0xFFFFFFFFuL

/*********************************************************************
*
*       Defines related to Master Boot Record
*
**********************************************************************
*/
#define MBR_OFF_PARTITION0            0x01BE     // Start of partition table
#define MBR_OFF_SIGNATURE             0x01FE
#define MBR_SIGNATURE                 0xAA55

#define PART_ENTRY_STATUS_ACTIVE      0x80
#define PART_ENTRY_STATUS_INACTIVE    0x00

#define PART_ENTRY_SIZE               16
#define PART_ENTRY_OFF_TYPE           0x04
#define PART_ENTRY_OFF_START_SECTOR   0x08
#define PART_ENTRY_OFF_NUM_SECTORS    0x0C

/*********************************************************************
*
*       Data types
*
**********************************************************************
*/
typedef struct {
  int TypeMask;    // Combination of FS_SECTOR_TYPE_DATA, FS_SECTOR_TYPE_MAN, FS_SECTOR_TYPE_DIR
  int ModeMask;    // Combination of FS_CACHE_MODE_R, FS_CACHE_MODE_W, FS_CACHE_MODE_D
} CACHE_MODE;

typedef struct {
  int TypeMask;    // Combination of FS_SECTOR_TYPE_DATA, FS_SECTOR_TYPE_MAN, FS_SECTOR_TYPE_DIR
  U32 NumSectors;  // Number of sectors to use for the type(s) specified
} CACHE_QUOTA;

typedef struct {
  U32 FirstSector;
  U32 NumSectors;
} CACHE_FREE;

struct FS_FORMAT_INFO_EX {
  U16           SectorsPerCluster;
  U16           NumRootDirEntries;    // Proposed, actual value depends on FATType
  U16           NumReservedSectors;
  U8            UpdatePartition;
  FS_DEV_INFO * pDevInfo;
};

/*********************************************************************
*
*       FS_INFO
*
*  The following structures are related:
*  The first 3 entries of the structures in the union need to be identical.
*/
struct FS_FAT_INFO {
  U32          NumSectors;        // RSVD + FAT + ROOT + FATA
  U16          BytesPerSector;    // 512,1024,2048,4096
  U16          ldBytesPerSector;  // 9, 10, 11, 12
  U32          FirstDataSector;
  U32          BytesPerCluster;
  U32          FATSize;           // Number of FAT sectors
  U32          RootDirPos;        // Position of root directory. FAT32: Cluster, FAT12/16: Sector
  U16          RootEntCnt;        // Number of root directory entries
  U16          RsvdSecCnt;        // 1 for FAT12 and FAT16
  U8           SectorsPerCluster; // Number of sector in an allocation unit
  U8           NumFATs;           // Typical value is 2
  U8           FATType;           // FS_FAT_TYPE_FAT12/16/32
  U32          NumClusters;
  U32          NumFreeClusters;   // Once known, we keep track of the number of free clusters
  U32          NextFreeCluster;   // Once known, we keep track of the next free cluster
#if FS_FAT_USE_FSINFO_SECTOR
  U16          FSInfoSector;
  U16          FSInfoSectorIsInvalidated;
#endif
#if FS_FAT_UPDATE_DIRTY_FLAG
  U8           IsDirty;           // Set to 1 as soon as the file system writes to storage. Set to 0 when the file system is unmounted or synchronized.
#endif
  //
  //  Used with file write operation mode is set to fast
  //
  U32          FreeClusterStart;
  U32          FreeClusterCnt;
  U32          FreeClusterUse;
  FS_FILE *    pFileUsedWithFreeCluster;
};

struct FS_EFS_INFO {
  U32          NumSectors;         // RSVD + EFS alloc table + DATA
  U16          BytesPerSector;     // 512,1024,2048,4096
  U16          ldBytesPerSector;   // 9, 10, 11, 12
  U32          FirstDataSector;    // First data cluster -> First Sector after InfoSector, EFSAllocTable.
  U32          BytesPerCluster;    // Bytes for each cluster: 512 - 32768
  U32          EFSTableSize;       // EFS Alloc table size in no. of sectors
  U32          RootDirPos;         // Cluster that is used for the root directory
  U8           SectorsPerCluster;  // Number of sectors for a cluster
  U32          NumClusters;        // Number of available clusters
  U32          NumFreeClusters;    // Once known, we keep track of the number of free clusters
  U32          NextFreeCluster;    // Once known, we keep track of the next free cluster
};

typedef union {
  FS_FAT_INFO FATInfo;
  FS_EFS_INFO EFSInfo;
  struct {
    U32       NumSectors;           // RSVD + EFS alloc table + DATA
    U16       BytesPerSector;       // 512,1024,2048,4096
    U16       ldBytesPerSector;     // 9, 10, 11, 12
  } Info;
} FS_INFO;

typedef struct {
  U8 Unit;
  U8 IsInited;
#if FS_SUPPORT_JOURNAL
  U8 JournalIsActive;
  U8 JournalUnit;
  U8 JournalIsInited;
  U8 JournalLogAlways;              // 1 means all data should be written to journal.
#endif
#if FS_SUPPORT_CACHE
  const FS_CACHE_API * pCacheAPI;
  void               * pCacheData;
#endif
#if FS_SUPPORT_BUSY_LED
  FS_BUSY_LED_CALLBACK * pfSetBusyLED;
#endif
#if FS_SUPPORT_CHECK_MEMORY
  FS_MEMORY_IS_ACCESSIBLE_CALLBACK * pfMemoryIsAccessible;
#endif
  FS_ONDEVICEACTIVITYHOOK * pfOnDeviceActivityHook;
} FS_DEVICE_DATA;

struct FS_DEVICE {
  const FS_DEVICE_TYPE * pType;
  FS_DEVICE_DATA         Data;
};

struct FS_PARTITION {
  FS_DEVICE Device;
  U32       StartSector;
  U32       NumSectors;
};

typedef struct {
  int   InUse;
  U32 * pBuffer;
} SECTOR_BUFFER;

typedef void        FS_CB_CLOSE               (FS_FILE    * pFile);
typedef int         FS_CB_CHECKFS_API         (FS_VOLUME  * pVolume);
typedef U32         FS_CB_READ                (FS_FILE    * pFile,           void  * pData, U32 NumBytes);
typedef U32         FS_CB_WRITE               (FS_FILE    * pFile,     const void  * pData, U32 NumBytes);
typedef int         FS_CB_OPEN                (const char * pFileName, FS_FILE * pFile, U8 DoDel, U8 DoOpen, U8 DoCreate);
typedef int         FS_CB_FORMAT              (FS_VOLUME  * pVolume,   const FS_FORMAT_INFO * pFormatInfo);
typedef int         FS_CB_OPENDIR             (const char * pDirName,  FS__DIR *pDir);
typedef int         FS_CB_CLOSEDIR            (FS__DIR    * pDir);
typedef int         FS_CB_READDIR             (FS__DIR    * pDir,      FS_DIRENTRY_INFO * pDirEntryInfo);
typedef int         FS_CB_REMOVEDIR           (FS_VOLUME  * pVolume,   const char * pDirName);
typedef int         FS_CB_CREATEDIR           (FS_VOLUME  * pVolume,   const char * pDirName);
typedef int         FS_CB_RENAME              (const char * sOldName,  const char * sNewName, FS_VOLUME * pVolume);
typedef int         FS_CB_MOVE                (const char * sOldName,  const char * sNewName, FS_VOLUME * pVolume);
typedef int         FS_CB_SETDIRENTRYINFO     (FS_VOLUME  * pVolume,   const char * sName, const void * p, int Mask);
typedef int         FS_CB_GETDIRENTRYINFO     (FS_VOLUME  * pVolume,   const char * sName,       void * p, int Mask);
typedef int         FS_CB_SETENDOFFILE        (FS_FILE    * pFile);
typedef void        FS_CB_UNMOUNT             (FS_VOLUME  * pVolume);
typedef int         FS_CB_GETDISKINFO         (FS_VOLUME  * pVolume,   FS_DISK_INFO * pDiskData, int Flags);
typedef int         FS_CB_GETVOLUMELABEL      (FS_VOLUME  * pVolume,   char * pVolumeLabel, unsigned VolumeLabelSize);
typedef int         FS_CB_SETVOLUMELABEL      (FS_VOLUME  * pVolume,   const char * pVolumeLabel);
typedef int         FS_CB_CREATEJOURNALFILE   (FS_VOLUME  * pVolume,   U32 NumBytes, U32 * pFirstSector, U32 * pNumSectors);
typedef int         FS_CB_OPENJOURNALFILE     (FS_VOLUME  * pVolume);
typedef U32         FS_CB_GETINDEXOFLASTSECTOR(FS_VOLUME  * pVolume);
typedef int         FS_CB_CHECKDISK           (FS_VOLUME  * pVolume,   const FS_DISK_INFO * pDiskInfo, void * pBuffer, U32 BufferSize, int MaxRecursionLevel, FS_CHECKDISK_ON_ERROR_CALLBACK * pfOnError);
typedef void        FS_CB_UPDATE_FILE         (FS_FILE    * pFile);
typedef int         FS_CB_SET_FILE_SIZE       (FS_FILE    * pFile, U32 NumBytes);
typedef int         FS_CB_FREE_SECTORS        (FS_VOLUME  * pVolume);

typedef struct {
  FS_CB_CHECKFS_API          * pfCheckBootSector;
  FS_CB_OPEN                 * pfOpen;
  FS_CB_CLOSE                * pfClose;
  FS_CB_READ                 * pfRead;
  FS_CB_WRITE                * pfWrite;
  FS_CB_FORMAT               * pfFormat;
  FS_CB_OPENDIR              * pfOpenDir;
  FS_CB_CLOSEDIR             * pfCloseDir;
  FS_CB_READDIR              * pfReadDir;
  FS_CB_REMOVEDIR            * pfRemoveDir;
  FS_CB_CREATEDIR            * pfCreateDir;
  FS_CB_RENAME               * pfRename;
  FS_CB_MOVE                 * pfMove;
  FS_CB_SETDIRENTRYINFO      * pfSetDirEntryInfo;
  FS_CB_GETDIRENTRYINFO      * pfGetDirEntryInfo;
  FS_CB_SETENDOFFILE         * pfSetEndOfFile;
  FS_CB_UNMOUNT              * pfUnmount;
  FS_CB_GETDISKINFO          * pfGetDiskInfo;
  FS_CB_GETVOLUMELABEL       * pfGetVolumeLabel;
  FS_CB_SETVOLUMELABEL       * pfSetVolumeLabel;
  FS_CB_CREATEJOURNALFILE    * pfCreateJournalFile;
  FS_CB_OPENJOURNALFILE      * pfOpenJournalFile;
  FS_CB_GETINDEXOFLASTSECTOR * pfGetIndexOfLastSector;
  FS_CB_CHECKDISK            * pfCheckDisk;
  FS_CB_UPDATE_FILE          * pfUpdateFile;
  FS_CB_SET_FILE_SIZE        * pfSetFileSize;
  FS_CB_FREE_SECTORS         * pfFreeSectors;
} FS_FS_API;

struct FS_VOLUME {
  FS_PARTITION      Partition;
  FS_INFO           FSInfo;
  U8                PartitionIndex;
  U8                MountType;          // 0 if the volume is not mounted.
  U8                AllowAutoMount;
  U8                InUse;
  FS_WRITEMODE      WriteMode;
#if FS_SUPPORT_MULTIPLE_FS
  const FS_FS_API * pFS_API;
#endif
  FS_VOLUME       * pNext;
};

/*********************************************************************
*
*       Directory types
*/
struct FS_DIR {
  FS__DIR   Dir;
  FS_DIRENT DirEntry;
  U8        InUse;
};

/*********************************************************************
*
*       File types
*/
typedef struct {
  U32 CurClusterFile;   // Current cluster within the file. First cluster is 0, next cluster is 1 ...
  U32 CurClusterAbs;    // Current cluster on the medium. This needs to be computed from file cluster, consulting the FAT
  U32 DirEntrySector;   // Sector of directory
  U16 DirEntryIndex;    // Index of directory entry
  U16 NumAdjClusters;   // Number of cluster that are sequentially in chain behind current one
} FS_INT_DATA_FAT;

typedef struct {
  U32 DirCluster;       // Start cluster of directory file
  U32 DirEntryPos;      // Offset of directory entry in directory file
  U32 CurClusterFile;   // Current cluster within the file. First cluster is 0, next cluster is 1 ...
  U32 CurClusterAbs;    // Current cluster on the medium. This needs to be computed from file cluster, consulting the FAT
  U16 NumAdjClusters;   // Number of cluster that are sequentially in chain behind current one
} FS_INT_DATA_EFS;

typedef union {
  FS_INT_DATA_FAT Fat;
  FS_INT_DATA_EFS Efs;
} FS_INT_DATA;

typedef struct FS_FILE_BUFFER {
  U8       * pData;               // Pointer to buffer where the data is stored
  U32        FilePos;             // Position in the file of the first byte in pData
  U32        SectorNo;            // Index of the logical sector
  unsigned   BufferSize;          // Buffer size in bytes
  unsigned   NumBytesInBuffer;    // Number of bytes stored in the buffer
  U8         IsDirty;             // Set to 1 if the application changed the data in the buffer
  U8         Flags;               // Flags which indicate the operating mode (read/write)
} FS_FILE_BUFFER;

typedef struct FS_FILE_OBJ FS_FILE_OBJ;
struct FS_FILE_OBJ {              // Describes the file object structure below the handle
  U32              FirstCluster;  // First cluster used for file
  U32              Size;          // Size of the file in bytes
  FS_VOLUME      * pVolume;
  U8               UseCnt;
  FS_INT_DATA      Data;
#if FS_MULTI_HANDLE_SAFE
  char             acFullFileName[FS_MAX_LEN_FULL_FILE_NAME];
#endif
#if FS_USE_FILE_BUFFER
  FS_FILE_BUFFER * pBuffer;
#endif
#if FS_SUPPORT_ENCRYPTION
  FS_CRYPT_OBJ   * pCryptObj;
  U32              SizeEncrypted; // Actual size of the file encrypted on the storage medium. Can be different from Size if the file buffer is active.
#endif
  FS_FILE_OBJ    * pNext;
};

struct FS_FILE {                          // Describes the file handle structure
  FS_FILE_OBJ    * pFileObj;              // Pointer to the associated file object
  U32              FilePos;               // Current position in file
#if (FS_SUPPORT_FILE_BUFFER)
  FS_FILE_BUFFER * pBuffer;               // File buffer
#endif
  I16              Error;                 // Last processing error
  U8               InUse;                 // Set to 1 if the file handle is opened
  U8               AccessFlags;           // Access mode required by the application
  U8               IsDirUpdateRequired;   // Set to 1 if the directory entry must be updated on file close
  FS_FILE        * pNext;
};

/*********************************************************************
*
*       Smart buffer (SB) type, internal
*/
struct FS_SB {
  U32 SectorNo;
#if FS_MAINTAIN_FAT_COPY
  U32 WriteCopyOff;             // Duplicate on write if value is != 0
#endif
  FS_PARTITION * pPart;
  U8           * pBuffer;
  char           IsDirty;
  char           HasError;
  U8             Type;
  U8             Read;            // 1 if data is valid, usually because the sector has been read
  U8             WriteToJournal;  // 1 if the write should go through journal
};

/*********************************************************************
*
*       CACHE_ALL_BLOCK_INFO
*
*  FS internal structure. One instance per block. Every cache block can cache a single sector.
*  It starts with CACHE_ALL_BLOCK_INFO, followed by the cached data.
*/
typedef struct {
  U32 SectorNo;
} CACHE_ALL_BLOCK_INFO;

/*********************************************************************
*
*       CACHE_ALL_DATA
*
*  FS internal structure. Once instance per cache. Size needs to be a multiple of 4.
*/
typedef struct {
  U32 NumSectors;
  U32 SectorSize;
  U32 NumBytesCache;
} CACHE_ALL_DATA;

/*********************************************************************
*
*       CACHE_MAN_BLOCK_INFO
*
*  FS internal structure. One instance per block. Every cache block can cache a single sector.
*  It starts with CACHE_MAN_BLOCK_INFO, followed by the cached data.
*/
typedef struct {
  U32 SectorNo;
} CACHE_MAN_BLOCK_INFO;

/*********************************************************************
*
*       CACHE_MAN_DATA
*
*  FS internal structure. Once instance per cache. Size needs to be a multiple of 4.
*/
typedef struct {
  U32 NumSectors;
  U32 SectorSize;
  U32 NumBytesCache;
} CACHE_MAN_DATA;

/*********************************************************************
*
*       CACHE_RW_BLOCK_INFO
*
*  FS internal structure. One instance per block. Every cache block can cache a single sector.
*  It starts with CACHE_RW_BLOCK_INFO, followed by the cached data.
*/
typedef struct {
  U32 SectorNo;
  U32 IsDirty;
} CACHE_RW_BLOCK_INFO;

/*********************************************************************
*
*       CACHE_RW_DATA
*
*  FS internal structure. Once instance per cache. Size needs to be a multiple of 4.
*/
typedef struct {
  U32 NumSectors;
  U32 SectorSize;
  U8  aCacheMode[FS_SECTOR_TYPE_COUNT];
#if FS_SECTOR_TYPE_COUNT % 4
  U8  aPadding[4 - (FS_SECTOR_TYPE_COUNT % 4)];     // Make sure we pad this to a multiple of 4 bytes
#endif
  U32 NumBytesCache;
} CACHE_RW_DATA;

/*********************************************************************
*
*       CACHE_RW_QUOTA_BLOCK_INFO
*
*  FS internal structure. One instance per block. Every cache block can cache a single sector.
*  It starts with CACHE_RW_QUOTA_BLOCK_INFO, followed by the cached data.
*/
typedef struct {
  U32 SectorNo;
  U32 IsDirty;
} CACHE_RW_QUOTA_BLOCK_INFO;

/*********************************************************************
*
*       CACHE_RW_QUOTA_DATA
*
*  FS internal structure. Once instance per cache. Size needs to be a multiple of 4.
*/
typedef struct {
  U32 NumSectors;
  U32 SectorSize;
  U8  aCacheMode[FS_SECTOR_TYPE_COUNT];
#if FS_SECTOR_TYPE_COUNT % 4
  U8  aPadding[4 - (FS_SECTOR_TYPE_COUNT % 4)];     // Make sure we pad this to a multiple of 4 bytes
#endif
  U32 aNumSectorsReq[FS_SECTOR_TYPE_COUNT];
  U32 aNumSectorsAct[FS_SECTOR_TYPE_COUNT];
  U32 aHashStart[FS_SECTOR_TYPE_COUNT];
  U32 NumBytesCache;
} CACHE_RW_QUOTA_DATA;

/*********************************************************************
*
*       CACHE_MULTI_WAY_BLOCK_INFO
*
*  FS internal structure. One instance per block. Every cache block can cache a single sector.
*  It starts with CACHE_MULTI_WAY_BLOCK_INFO, followed by the cached data.
*/
typedef struct {
  U32 SectorNo;       // Index of the sector stored in this block
  U16 AccessCnt;      // Access count used for the replacement policy
  U16 IsDirty;        // 1 if the sector has been modified and needs to be written to medium
} CACHE_MULTI_WAY_BLOCK_INFO;

/*********************************************************************
*
*       CACHE_MULTI_WAY_DATA
*
*  FS internal structure. Once instance per cache. Size needs to be a multiple of 4.
*  This structure is stored a the beginning of cache memory.
*  It is followed by an array of cache blocks.
*/
typedef struct {
  U32 NumSets;            // Total number of sets that can be stored in the cache
  U32 ldAssocLevel;       // Number of sectors in a set. Power of 2 value as bit shift.
  U32 SectorSize;         // Size of a sector in bytes
  U8  aCacheMode[FS_SECTOR_TYPE_COUNT];           // Cache strategy for each sector type (management, directory and data)
#if FS_SECTOR_TYPE_COUNT % 4
  U8  aPadding[4 - (FS_SECTOR_TYPE_COUNT % 4)];   // Make sure the aCacheMode[] is padded to a multiple of 4 bytes
#endif
  U32 NumBytesCache;      // Total size of the cache in bytes.
} CACHE_MULTI_WAY_DATA;

/*********************************************************************
*
*       FS_PROFILE
*/
typedef struct FS_PROFILE {
  U32                    IdOffset;
  const FS_PROFILE_API * pAPI;
} FS_PROFILE;

/*********************************************************************
*
*       Partition related functions
*/
signed char   FS__LocatePartition   (FS_VOLUME * pVolume);
int           FS__CreateMBR         (FS_VOLUME * pVolume,       FS_PARTITION_INFO * pPartInfo, int NumPartitions);
int           FS__WriteMBR          (FS_VOLUME * pVolume, const FS_PARTITION_INFO * pPartInfo, int NumPartitions);
int           FS__GetPartitionInfo  (FS_VOLUME * pVolume,       FS_PARTITION_INFO * pPartInfo, U8 PartIndex);
void          FS__LoadPartitionInfo (U8 PartIndex,       FS_PARTITION_INFO * pPartInfo, const U8 * pBuffer);
void          FS__StorePartitionInfo(U8 PartIndex, const FS_PARTITION_INFO * pPartInfo,       U8 * pBuffer);
void          FS__CalcPartitionInfo (FS_PARTITION_INFO * pPartInfo, U32 NumSectorsDevice);

/*********************************************************************
*
*       little endian translation functions, internal
*/
U16           FS_LoadU16LE (const U8 *pBuffer);
U32           FS_LoadU32LE (const U8 *pBuffer);
void          FS_StoreU16LE(      U8 *pBuffer, unsigned Data);
void          FS_StoreU24LE(      U8 *pBuffer, U32 Data);
void          FS_StoreU32LE(      U8 *pBuffer, U32 Data);

/*********************************************************************
*
*       big endian translation functions, internal
*/
U32           FS_LoadU32BE (const U8 *pBuffer);
U16           FS_LoadU16BE (const U8 *pBuffer);
void          FS_StoreU32BE(      U8 *pBuffer, U32 Data);
void          FS_StoreU24BE(      U8 *pBuffer, U32 Data);
void          FS_StoreU16BE(      U8 *pBuffer, unsigned Data);

/*********************************************************************
*
*       CACHE API type, internal
*/
struct FS_CACHE_API {
  char   (*pfReadFromCache)   (FS_DEVICE * pDevice,      U32 SectorNo,       void * pData, U8 Type);
  char   (*pfUpdateCache)     (FS_DEVICE * pDevice,      U32 SectorNo, const void * pData, U8 Type);    // Returns 0 if write cached, which means no further write is required
  void   (*pfInvalidateCache) (void      * pCacheData);
  int    (*pfCommand)         (FS_DEVICE * pDevice   ,   int Cmd, void *p);
  char   (*pfWriteIntoCache)  (FS_DEVICE * pDevice   ,   U32 SectorNo, const void * pData, U8 Type);    // Returns 0 if write cached, which means no further write is required
};

/*********************************************************************
*
*       Smart buffer (SB) API-functions, internal
*
*/
void FS__SB_Clean            (FS_SB * pSB);
char FS__SB_Create           (FS_SB * pSB, FS_PARTITION * pPart);
void FS__SB_Delete           (FS_SB * pSB);
void FS__SB_Flush            (FS_SB * pSB);
void FS__SB_MarkDirty        (FS_SB * pSB);
void FS__SB_MarkValid        (FS_SB * pSB, U32 SectorNo, U8 Type, U8 WriteToJournal);
void FS__SB_MarkNotDirty     (FS_SB * pSB);
char FS__SB_Read             (FS_SB * pSB);
void FS__SB_SetSector        (FS_SB * pSB, U32 SectorNo, U8 Type, U8 WriteToJournal);
char FS__SB_Write            (FS_SB * pSB);
#if FS_MAINTAIN_FAT_COPY
  void FS__SB_SetWriteCopyOff(FS_SB * pSB, U32 Off);
#endif

/*********************************************************************
*
*       Cache related functions, internal
*
*/
int   FS__CACHE_CommandVolume  (FS_VOLUME * pVolume, int Cmd, void * pData);
int   FS__CACHE_CommandVolumeNL(FS_VOLUME * pVolume, int Cmd, void * pData);
int   FS__CACHE_CommandDevice  (FS_DEVICE * pDevice, int Cmd, void * pData);
void  FS__CACHE_CleanNL        (FS_DEVICE * pDevice);
void  FS__CACHE_Clean          (FS_VOLUME * pVolume);

/*********************************************************************
*
*       Sector allocation API-functions, internal
*
*/
U8 *  FS__AllocSectorBuffer (void);
void  FS__FreeSectorBuffer  (const void * p);

/*********************************************************************
*
*       String operation API-functions, internal
*
*/
void         FS_memcpy      (void * pDest, const void * pSrc, int NumBytes);
const char * FS__strchr     (const char *s, int c);
void         FS__AddSpaceHex(U32 v, U8 Len, char** ps);

/*********************************************************************
*
*       Volume API-functions, internal
*
*/
FS_VOLUME * FS__FindVolume        (const char * pFullName, const char ** pFileName);
int         FS__Mount             (FS_VOLUME * pVolume, U8 MountType);
int         FS__AutoMount         (FS_VOLUME * pVolume);
void        FS__Unmount           (FS_VOLUME * pVolume);
void        FS__UnmountLL         (FS_VOLUME * pVolume);
void        FS__UnmountLL_NL      (FS_VOLUME * pVolume);
void        FS__UnmountForcedLL   (FS_VOLUME * pVolume);
void        FS__UnmountForcedLL_NL(FS_VOLUME * pVolume);
void        FS__Sync              (FS_VOLUME * pVolume);
void        FS__SyncNL            (FS_VOLUME * pVolume);

/*********************************************************************
*
*       API-functions, internal (without Global locking)
*
*/
int          FS__FClose              (FS_FILE * pFile);
int          FS__SyncFile            (FS_FILE * pFile);
U32          FS__GetFileSize         (const FS_FILE * pFile);
U32          FS__Read                (FS_FILE * pFile,       void * pData, U32 NumBytes);
U32          FS__Write               (FS_FILE * pFile, const void * pData, U32 NumBytes);
int          FS__Verify              (FS_FILE * pFile, const void * pData, U32 NumBytes);
int          FS__Remove              (const char * pFileName);
int          FS__FOpen               (const char * pFileName, const char * pMode, FS_FILE ** ppFile);
int          FS__FOpenEx             (const char * pFileName, U8 AccessFlags, U8 DoCreate, U8 DoDel, U8 DoOpen, FS_FILE ** ppFile);
int          FS__OpenEx              (FS_VOLUME  * pVolume,   const char * sFilePath, U8 AccessFlags, U8 DoCreate, U8 DoDel, U8 DoOpen, FS_FILE ** ppFile);

U32          FS__CalcSizeInBytes     (U32 NumClusters,   U32 SectorsPerCluster, U32 BytesPerSector);
U32          FS__CalcSizeInKB        (U32 NumClusters, U32 SectorsPerCluster, U32 BytesPerSector);

FS_VOLUME  * FS__AddDevice           (const FS_DEVICE_TYPE * pDevType);
int          FS__AddPhysDevice       (const FS_DEVICE_TYPE * pDevType);

int          FS__IoCtl               (FS_VOLUME * pVolume, I32 Cmd, I32 Aux, void *pBuffer);
int          FS__Format              (FS_VOLUME * pVolume, const FS_FORMAT_INFO * pFormatInfo);
int          FS__SD_Format           (FS_VOLUME * pVolume);
int          FS__IsSDFormatted       (FS_VOLUME * pVolume);
int          FS__GetNumVolumes       (void);
int          FS__CopyFile            (const char * sSource, const char   * sDest);
int          FS__CopyFileEx          (const char * sSource, const char   * sDest, void * pBuffer, U32 NumBytes);
int          FS__GetVolumeInfo       (const char * sVolume, FS_DISK_INFO * pInfo);
int          FS__GetVolumeInfoEx     (const char * sVolume, FS_DISK_INFO * pInfo, int Flags);
int          FS__CreateDir           (const char * sDir);
int          FS__MkDir               (const char * pDirName);
int          FS__RmDir               (const char * pDirName);
int          FS__FSeek               (FS_FILE *pFile, I32 Offset, int Origin);
I32          FS__FTell               (FS_FILE *pFile);
int          FS__GetFileTimeEx       (const char * pName, U32 * pTimeStamp, int Index);
int          FS__SetFileTimeEx       (const char * pName, U32   TimeStamp,  int Index);
int          FS__SetFileAttributes   (const char * pName, U8    Attributes);
U8           FS__GetFileAttributes   (const char * pName);
U8           FS__ModifyFileAttributes(const char * pName, U8 SetMask, U8 ClrMask);
int          FS__SetEndOfFile        (FS_FILE    * pFile);
int          FS__SetFileSize         (FS_FILE    * pFile, U32 NumBytes);
void         FS__RemoveDevices       (void);
void         FS__RemoveDevice        (FS_VOLUME * pVolume);

FS_DIR     * FS__OpenDir             (const char *pDirName);
FS_DIRENT  * FS__ReadDir             (FS_DIR *pDir);
int          FS__CloseDir            (FS_DIR *pDir);
void         FS__RewindDir           (FS_DIR *pDir);
void         FS__DirEnt2Attr         (const FS_DIRENT *pDirEnt, U8* pAttr);
U32          FS__GetNumFiles         (FS_DIR *pDir);
int          FS__FindFirstFile       (FS_FIND_DATA * pfd, const char * sPath, char * sFilename, int sizeofFilename);
int          FS__FindFirstFileEx     (FS_FIND_DATA * pfd, FS_VOLUME * pVolume, const char * sPath, char * sFilename, int sizeofFilename);
int          FS__FindNextFile        (FS_FIND_DATA * pfd);
void         FS__FindClose           (FS_FIND_DATA * pfd);

int          FS__FormatLow           (FS_VOLUME * pVolume);
int          FS__FormatLowNL         (FS_VOLUME * pVolume);
void         FS__UnmountForced       (FS_VOLUME * pVolume);
int          FS__GetVolumeStatus     (FS_VOLUME * pVolume);
int          FS__GetVolumeStatusNL   (FS_VOLUME * pVolume);

int          FS__ReadSector          (FS_VOLUME * pVolume,       void * pData, U32 SectorIndex);
int          FS__ReadSectorNL        (FS_VOLUME * pVolume,       void * pData, U32 SectorIndex);
int          FS__ReadSectors         (FS_VOLUME * pVolume,       void * pData, U32 SectorIndex, U32 NumSectors);
int          FS__ReadSectorsNL       (FS_VOLUME * pVolume,       void * pData, U32 SectorIndex, U32 NumSectors);
int          FS__WriteSector         (FS_VOLUME * pVolume, const void * pData, U32 SectorIndex);
int          FS__WriteSectorNL       (FS_VOLUME * pVolume, const void * pData, U32 SectorIndex);
int          FS__WriteSectors        (FS_VOLUME * pVolume, const void * pData, U32 SectorIndex, U32 NumSectors);
int          FS__WriteSectorsNL      (FS_VOLUME * pVolume, const void * pData, U32 SectorIndex, U32 NumSectors);
int          FS__GetDeviceInfo       (FS_VOLUME * pVolume, FS_DEV_INFO * pDevInfo);
int          FS__GetDeviceInfoNL     (FS_VOLUME * pVolume, FS_DEV_INFO * pDevInfo);
int          FS__IsLLFormatted       (FS_VOLUME * pVolume);
int          FS__IsLLFormattedNL     (FS_VOLUME * pVolume);
void         FS__CalcDeviceInfo      (FS_DEV_INFO * pDevInfo);
FS_WRITEMODE FS__GetFileWriteModeEx  (const FS_VOLUME * pVolume);

/*********************************************************************
*
*       File buffer related functions, internal
*/
int          FS__FB_Read       (      FS_FILE * pFile,       void * pData, U32 NumBytes);
int          FS__FB_Write      (      FS_FILE * pFile, const void * pData, U32 NumBytes);
int          FS__FB_Clean      (      FS_FILE * pFile);
int          FS__FB_Sync       (      FS_FILE * pFile);
U32          FS__FB_GetFileSize(const FS_FILE * pFile);

/*********************************************************************
*
*       Storage layer related functions, internal
*
*/
void        FS__STORAGE_Sync            (FS_VOLUME * pVolume);
void        FS__STORAGE_SyncNL          (FS_VOLUME * pVolume);
int         FS__STORAGE_RefreshSectors  (FS_VOLUME * pVolume, U32 SectorIndex, U32 NumSectors, void * pBuffer, unsigned NumSectorsInBuffer);
int         FS__STORAGE_GetSectorUsage  (FS_VOLUME * pVolume, U32 SectorIndex);
int         FS__STORAGE_GetSectorUsageNL(FS_VOLUME * pVolume, U32 SectorIndex);

/*********************************************************************
*
*       API-functions, internal (without driver locking)
*
*/
int         FS__FCloseNL             (FS_FILE   * pFile);
int         FS__CACHE_CommandDeviceNL(FS_DEVICE * pDevice, int Cmd, void * pData);
int         FS__IoCtlNL              (FS_VOLUME * pVolume, I32 Cmd, I32 Aux, void * Buffer);
void        FS__UnmountForcedNL      (FS_VOLUME * pVolume);
void        FS__UnmountNL            (FS_VOLUME * pVolume);
int         FS__MountNL              (FS_VOLUME * pVolume, U8 MountType);
int         FS__AutoMountNL          (FS_VOLUME * pVolume);

/*********************************************************************
*
*       API-functions, file handle operations
*
*/
FS_FILE     * FS__AllocFileHandle(void);
void          FS__FreeFileHandle(FS_FILE * pFile);

/*********************************************************************
*
*       API-functions, file object operations
*
*/
FS_FILE_OBJ * FS__AllocFileObj (const char  * sFullFileName);
FS_FILE_OBJ * FS__GetFileObj   (const char  * sFullFileName);
void          FS__FreeFileObj  (FS_FILE_OBJ * pFileObj);
void          FS__FreeFileObjNL(FS_FILE_OBJ * pFileObj);

/*********************************************************************
*
*       ECC256
*
*/
int  FS__ECC256_Apply  (U32 * pData, U32 eccRead);
FS_OPTIMIZE
U32  FS__ECC256_Calc   (const U32 * pData);
int  FS__ECC256_IsValid(U32 ecc);
U32  FS__ECC256_Load   (const U8 * p);
void FS__ECC256_Store  (U8 * p, U32 ecc);

/*********************************************************************
*
*       Helper functions
*
*/
U32      FS__DivideU32Up            (      U32  Nom,      U32      Div);
U32      FS__DivModU32              (      U32  v,        U32      Div,      U32 * pRem);
U32      FS_CRC32_Calc              (const U8 * pData,    unsigned NumBytes, U32 crc);
FS_OPTIMIZE
U16      FS_CRC16_Calc              (const U8 * pData,    unsigned NumBytes, U16 crc);
U16      FS_CRC16_CalcBitByBit      (const U8 * pData,    unsigned NumBytes, U16 crc, U16 Polynom);
int      FS_CRC16_Validate          (void);
U32      FS_BITFIELD_CalcSize       (      U32  NumItems, unsigned BitsPerItem);
unsigned FS_BITFIELD_CalcNumBitsUsed(      U32  NumItems);
U32      FS_BITFIELD_ReadEntry      (const U8 * pBase,    U32      Index,       unsigned NumBits);
void     FS_BITFIELD_WriteEntry     (      U8 * pBase,    U32      Index,       unsigned NumBits, U32 v);
void     FS_PRNG_Init               (U16 Value);
U16      FS_PRNG_Generate           (void);

/*********************************************************************
*
*       Public data
*
*/

/*********************************************************************
*
*       FS_GLOBAL
*/
typedef struct {
  unsigned        FileBufferSize;
  U8              FileBufferFlags;
  SECTOR_BUFFER * paSectorBuffer;
  unsigned        NumSectorBuffers;
  U16             MaxSectorSize;
  U8              IsInited;
  FS_FILE       * pFirstFileHandle;
  FS_FILE_OBJ   * pFirstFileObj;
  FS_VOLUME       FirstVolume;
  int             NumVolumes;
  FS_WRITEMODE    WriteMode;
#if FS_SUPPORT_DEINIT
  FS_ON_EXIT_CB * pFirstOnExit;
#endif
#if FS_SUPPORT_PROFILE
  FS_PROFILE      Profile;
#endif
} FS_GLOBAL;

EXTERN FS_GLOBAL FS_Global;

/*********************************************************************
*
*       OS mapping macros (multi tasking locks)
*
*
* Notes
*   These macros map to locking routines or are empty,
*   depending on the configuration
*   There are 3 different lock-levels:
*   FS_OS == 0                     -> No locking
*   FS_OS == 1
*     FS_OS_LOCK_PER_DRIVER == 0     -> Single, global lock in every API function
*     FS_OS_LOCK_PER_DRIVER == 1     -> Multiple locks
*
**********************************************************************
*/
#if (FS_OS == 0)                               /* No locking */
  #define FS_LOCK()
  #define FS_UNLOCK()

  #define FS_LOCK_SYS()
  #define FS_UNLOCK_SYS()

  #define FS_OS_INIT(MaxNumLocks)
  #define FS_OS_DEINIT()


  #define FS_LOCK_DRIVER(pDriver)
  #define FS_UNLOCK_DRIVER(pDriver)

  #define FS_OS_ADD_DRIVER(pDevice)
  #define FS_OS_REMOVE_DRIVER(pDevice)

  #define  FS_OS_GETNUM_DRIVERLOCKS()               0
  #define  FS_OS_GETNUM_SYSLOCKS()                  0

  #define FS_OS_WAIT(Timeout)
  #define FS_OS_SIGNAL()


#elif (FS_OS) && (FS_OS_LOCK_PER_DRIVER == 0)
  //
  // Coarse lock granularity:
  //   One global lock for all FS API functions
  //

  #define FS_LOCK_ID_SYSTEM                  0

  #define FS_LOCK()           FS_OS_Lock(FS_LOCK_ID_SYSTEM)
  #define FS_UNLOCK()         FS_OS_Unlock(FS_LOCK_ID_SYSTEM)

  #define FS_LOCK_SYS()
  #define FS_UNLOCK_SYS()

  #define FS_LOCK_DRIVER(pDriver)
  #define FS_UNLOCK_DRIVER(pDriver)

  #define FS_OS_ADD_DRIVER(pDevice)
  #define FS_OS_REMOVE_DRIVER(pDevice)

  #define  FS_OS_GETNUM_DRIVERLOCKS()               0
  #define  FS_OS_GETNUM_SYSLOCKS()                  1

  #define FS_OS_INIT(MaxNumLocks)                       FS_OS_Init(MaxNumLocks)
  #define FS_OS_DEINIT()                                FS_OS_DeInit()

  #define FS_OS_WAIT(Timeout)                           FS_X_OS_Wait(Timeout)
  #define FS_OS_SIGNAL()                                FS_X_OS_Signal()
#else
  //
  // Fine lock granularity:
  //   Lock for different FS functions
  //
  #define FS_LOCK_ID_SYSTEM                  0
  #define FS_LOCK_ID_DEVICE                  1

  #define FS_LOCK()
  #define FS_UNLOCK()


  #define FS_LOCK_SYS()                      FS_OS_Lock  (FS_LOCK_ID_SYSTEM)
  #define FS_UNLOCK_SYS()                    FS_OS_Unlock(FS_LOCK_ID_SYSTEM)

  #define FS_LOCK_DRIVER(pDevice)            FS_OS_LockDriver(pDevice)
  #define FS_UNLOCK_DRIVER(pDevice)          FS_OS_UnlockDriver(pDevice)
  void FS_OS_LockDriver  (const FS_DEVICE * pDevice);
  void FS_OS_UnlockDriver(const FS_DEVICE * pDevice);


  #define  FS_OS_GETNUM_DRIVERLOCKS()               FS_OS_GetNumDriverLocks()
  #define  FS_OS_GETNUM_SYSLOCKS()                  1
  unsigned FS_OS_GetNumDriverLocks(void);

  #define FS_OS_ADD_DRIVER(pDevice)         FS_OS_AddDriver(pDevice)
  void FS_OS_AddDriver(const FS_DEVICE_TYPE * pDriver);

  #define FS_OS_REMOVE_DRIVER(pDevice)              FS_OS_RemoveDriver(pDevice)
  void FS_OS_RemoveDriver(const FS_DEVICE_TYPE * pDriver);

  #define FS_OS_INIT(MaxNumLocks)                       FS_OS_Init(MaxNumLocks)
  #define FS_OS_DEINIT()                                FS_OS_DeInit()

  #define FS_OS_WAIT(Timeout)                           FS_X_OS_Wait(Timeout)
  #define FS_OS_SIGNAL()                                FS_X_OS_Signal()
#endif

void FS_OS_Lock  (unsigned LockIndex);
void FS_OS_Unlock(unsigned LockIndex);
void FS_OS_Init  (unsigned NumLocks);
void FS_OS_DeInit(void);

/*********************************************************************
*
*       FS_JOURNAL
*
*       Journal to make file system layer transaction and fail safe
*
**********************************************************************
*/

typedef void (FS_JOURNAL_TEST_HOOK)(U8 Unit);

void FS__JOURNAL_DeInit           (const FS_VOLUME * pVolume);
int  FS__JOURNAL_GetNumFreeSectors(FS_VOLUME * pVolume);
int  FS__JOURNAL_IsPresent        (const FS_DEVICE * pDevice);
int  FS__JOURNAL_Mount            (FS_VOLUME * pVolume);
int  FS__JOURNAL_Read             (const FS_DEVICE * pDevice, U32 SectorNo, void       * pBuffer, U32 NumSectors);
int  FS__JOURNAL_Write            (const FS_DEVICE * pDevice, U32 SectorNo, const void * pBuffer, U32 NumSectors, U8 RepeatSame);
int  FS__JOURNAL_Begin            (FS_VOLUME * pVolume);
int  FS__JOURNAL_End              (FS_VOLUME * pVolume);
void FS__JOURNAL_Delete           (FS_VOLUME * pVolume, U32 LastSectorInFS);
void FS__JOURNAL_Invalidate       (FS_VOLUME * pVolume);
int  FS__JOURNAL_Clean            (FS_VOLUME * pVolume);
void FS__JOURNAL_SetTestHook      (FS_JOURNAL_TEST_HOOK * pfTestHook);
int  FS__JOURNAL_FreeSectors      (const FS_DEVICE * pDevice, U32 SectorNo, U32 NumSectors);
int  FS__JOURNAL_GetOpenCnt       (FS_VOLUME * pVolume);

#if FS_SUPPORT_JOURNAL
  #define  FS_JOURNAL_BEGIN(pVolume)                  FS__JOURNAL_Begin(pVolume)
  #define  FS_JOURNAL_END(pVolume)                    FS__JOURNAL_End  (pVolume)
  #define  FS_JOURNAL_MOUNT(pVolume)                  FS__JOURNAL_Mount(pVolume)
  #define  FS_JOURNAL_DELETE(pVolume, LastSector)     FS__JOURNAL_Delete(pVolume, LastSector)
  #define  FS_JOURNAL_INVALIDATE(pVolume)             FS__JOURNAL_Invalidate(pVolume)
  #define  FS_JOURNAL_CLEAN(pVolume)                  FS__JOURNAL_Clean(pVolume)
#else
  #define  FS_JOURNAL_BEGIN(pVolume)                  FS_USE_PARA(pVolume)
  #define  FS_JOURNAL_END(pVolume)                    FS_USE_PARA(pVolume)
  #define  FS_JOURNAL_MOUNT(pVolume)
  #define  FS_JOURNAL_DELETE(pVolume, LastSector)
  #define  FS_JOURNAL_INVALIDATE(pVolume)
  #define  FS_JOURNAL_CLEAN(pVolume)
#endif

#if FS_SUPPORT_DEINIT && FS_SUPPORT_JOURNAL
  #define  FS_JOURNAL_DEINIT(pVolume)                 FS__JOURNAL_DeInit(pVolume)
#else
  #define  FS_JOURNAL_DEINIT(pVolume)
#endif

/*********************************************************************
*
*       MEMORY API
*
*       This macro map to the memory free functions of the file system
*       when deinitialization is enabled.
*
**********************************************************************
*/
#if FS_SUPPORT_DEINIT
  #define  FS_FREE(pMem)                 FS_Free(pMem)
#else
  #define  FS_FREE(pMem)
#endif

/*********************************************************************
*
*       API mapping macros
*
*       These macros map to the functions of the file system (Currently FAT or EFS)
*       or - in case of multiple file systems - to a mapping layer, which calls the
*       appropriate function depending on the file system of the volume
*
**********************************************************************
*/

#define FS_IOCTL(    pDevice, Cmd, Aux, pBuffer)                FS_LB_Ioctl(pDevice, Cmd, Aux, pBuffer)

#if FS_SUPPORT_FAT && (FS_SUPPORT_EFS == 0)
  #define FS_CHECK_INFOSECTOR(pVolume)                                FS_FAT_CheckBPB(pVolume)
  #define FS_CLOSEDIR(pDir)                                           FS_FAT_CloseDir(pDir);
  #define FS_CREATEDIR(pVolume, s)                                    FS_FAT_CreateDir(pVolume, s)
  #define FS_CLOSE_FILE(pFile)                                        FS_FAT_Close(pFile)
  #define FS_OPEN_FILE(s, pFile, DoDel, DoOpen, DoCreate)             FS_FAT_Open(s, pFile, DoDel, DoOpen, DoCreate)
  #define FS_FORMAT(pVolume, pFormatInfo)                             FS_FAT_Format(pVolume, pFormatInfo)
  #define FS_FREAD(pFile, pData, NumBytes)                            FS_FAT_Read(pFile, pData, NumBytes)
  #define FS_FWRITE(pFile, pData, NumBytes)                           FS_FAT_Write(pFile, pData, NumBytes)
  #define FS_GETDIRENTRYINFO(pVolume, sName, p, Mask)                 FS_FAT_GetDirEntryInfo(pVolume, sName, p, Mask)
  #define FS_GET_DISKINFO(pVolume, pInfo, Flags)                      FS_FAT_GetDiskInfo(pVolume, pInfo, Flags)
  #define FS_MOVE(sSrc, sDest, pVolume)                               FS_FAT_Move(sSrc, sDest, pVolume)
  #define FS_OPENDIR(s, pDirHandle)                                   FS_FAT_OpenDir(s, pDirHandle)
  #define FS_READDIR(pDir, pDirEntryInfo)                             FS_FAT_ReadDir(pDir, pDirEntryInfo)
  #define FS_REMOVEDIR(pVolume, s)                                    FS_FAT_RemoveDir(pVolume, s)
  #define FS_RENAME(s, sNewName, pVolume)                             FS_FAT_Rename(s, sNewName, pVolume)
  #define FS_SETDIRENTRYINFO(pVolume, sName, p, Mask)                 FS_FAT_SetDirEntryInfo(pVolume, sName, p, Mask)
  #define FS_SET_END_OF_FILE(pFile)                                   FS_FAT_SetEndOfFile(pFile)
  #define FS_CLEAN_FS(pVolume)                                        FS_FAT_Clean(pVolume)
  #define FS_GET_VOLUME_LABEL(pVolume, pVolLabel, VolLabelSize)       FS_FAT_GetVolumeLabel(pVolume, pVolLabel, VolLabelSize)
  #define FS_SET_VOLUME_LABEL(pVolume, pVolLabel)                     FS_FAT_SetVolumeLabel(pVolume, pVolLabel)
  #define FS_CREATE_JOURNAL_FILE(pVolume, NumBytes, pFirstS, pNumS)   FS_FAT_CreateJournalFile(pVolume, NumBytes, pFirstS, pNumS)
  #define FS_OPEN_JOURNAL_FILE(pVolume)                               FS_FAT_OpenJournalFile(pVolume)
  #define FS_GET_INDEX_OF_LAST_SECTOR(pVolume)                        FS_FAT_GetIndexOfLastSector(pVolume)
  #define FS_CHECKDISK(pVolume, pDiskInfo, pBuffer, BufferSize, MaxRecursionLevel, pfOnError) FS_FAT__CheckDisk(pVolume, pDiskInfo, pBuffer, BufferSize, MaxRecursionLevel, pfOnError)
  #define FS_UPDATE_FILE(pFile)                                       FS_FAT_Close(pFile)
  #define FS_SET_FILE_SIZE(pFile, NumBytes)                           FS_FAT_SetFileSize(pFile, NumBytes)
  #define FS_FREE_SECTORS(pVolume)                                    FS_FAT_FreeSectors(pVolume)
#elif (FS_SUPPORT_FAT == 0) && FS_SUPPORT_EFS
  #define FS_CHECK_INFOSECTOR(pVolume)                                FS_EFS_CheckInfoSector(pVolume)
  #define FS_CLOSEDIR(pDir)                                           FS_EFS_CloseDir(pDir)
  #define FS_CREATEDIR(pVolume, s)                                    FS_EFS_CreateDir(pVolume, s)
  #define FS_CLOSE_FILE(pFile)                                        FS_EFS_Close(pFile)
  #define FS_OPEN_FILE(s, pFile, DoDel, DoOpen, DoCreate)             FS_EFS_Open(s, pFile, DoDel, DoOpen, DoCreate)
  #define FS_FORMAT(pVolume, pFormatInfo)                             FS_EFS_Format(pVolume, pFormatInfo)
  #define FS_FREAD(pFile, pData, NumBytes)                            FS_EFS_Read(pFile, pData, NumBytes)
  #define FS_FWRITE(pFile, pData, NumBytes)                           FS_EFS_Write(pFile, pData, NumBytes)
  #define FS_GETDIRENTRYINFO(pVolume, sName, p, Mask)                 FS_EFS_GetDirEntryInfo(pVolume, sName, p, Mask)
  #define FS_GET_DISKINFO(pVolume, pInfo, Flags)                      FS_EFS_GetDiskInfo(pVolume, pInfo, Flags)
  #define FS_MOVE(sSrc, sDest, pVolume)                               FS_EFS_Move(sSrc, sDest, pVolume)
  #define FS_OPENDIR(s, pDirHandle)                                   FS_EFS_OpenDir(s, pDirHandle)
  #define FS_READDIR(pDir, pDirEntryInfo)                             FS_EFS_ReadDir(pDir, pDirEntryInfo)
  #define FS_REMOVEDIR(pVolume, s)                                    FS_EFS_RemoveDir(pVolume, s)
  #define FS_RENAME(s, sNewName, pVolume)                             FS_EFS_Rename(s, sNewName, pVolume)
  #define FS_SETDIRENTRYINFO(pVolume, sName, p, Mask)                 FS_EFS_SetDirEntryInfo(pVolume, sName, p, Mask)
  #define FS_SET_END_OF_FILE(pFile)                                   FS_EFS_SetEndOfFile(pFile)
  #define FS_CLEAN_FS(pVolume)                                        FS_EFS_Clean(pVolume)
  #define FS_GET_VOLUME_LABEL(pVolume, pVolLabel, VolLabelSize)       FS_EFS_GetVolumeLabel(pVolume, pVolLabel, VolLabelSize)
  #define FS_SET_VOLUME_LABEL(pVolume, pVolLabel)                     FS_EFS_SetVolumeLabel(pVolume, pVolLabel)
  #define FS_CREATE_JOURNAL_FILE(pVolume, NumBytes, pFirstS, pNumS)   FS_EFS_CreateJournalFile(pVolume, NumBytes, pFirstS, pNumS)
  #define FS_OPEN_JOURNAL_FILE(pVolume)                               FS_EFS_OpenJournalFile(pVolume)
  #define FS_GET_INDEX_OF_LAST_SECTOR(pVolume)                        FS_EFS_GetIndexOfLastSector(pVolume)
  #define FS_CHECKDISK(pVolume, pDiskInfo, pBuffer, BufferSize, MaxRecursionLevel, pfOnError) FS_EFS__CheckDisk(pVolume, pDiskInfo, pBuffer, BufferSize, MaxRecursionLevel, pfOnError)
  #define FS_UPDATE_FILE(pFile)                                       FS_EFS_Close(pFile)
  #define FS_SET_FILE_SIZE(pFile, NumBytes)                           FS_EFS_SetFileSize(pFile, NumBytes)
  #define FS_FREE_SECTORS(pVolume)                                    FS_EFS_FreeSectors(pVolume)
#elif FS_SUPPORT_FAT && FS_SUPPORT_EFS
  #define FS_CHECK_INFOSECTOR(pVolume)                                FS_MAP_CheckFS_API(pVolume)
  #define FS_CLOSEDIR(pDir)                                           FS_MAP_CloseDir(pDir)
  #define FS_CREATEDIR(pVolume, s)                                    FS_MAP_CreateDir(pVolume, s)
  #define FS_CLOSE_FILE(pFile)                                        FS_MAP_Close(pFile)
  #define FS_OPEN_FILE(s, pFile, DoDel, DoOpen, DoCreate)             FS_MAP_Open(s, pFile, DoDel, DoOpen, DoCreate)
  #define FS_FORMAT(pVolume, pFormatInfo)                             FS_MAP_Format(pVolume, pFormatInfo)
  #define FS_FREAD(pFile, pData, NumBytes)                            FS_MAP_Read(pFile, pData, NumBytes)
  #define FS_FWRITE(pFile, pData, NumBytes)                           FS_MAP_Write(pFile, pData, NumBytes)
  #define FS_GETDIRENTRYINFO(pVolume, sName, p, Mask)                 FS_MAP_GetDirEntryInfo(pVolume, sName, p, Mask)
  #define FS_GET_DISKINFO(pVolume, pInfo, Flags)                      FS_MAP_GetDiskInfo(pVolume, pInfo, Flags)
  #define FS_MOVE(sSrc, sDest, pVolume)                               FS_MAP_Move(sSrc, sDest, pVolume)
  #define FS_OPENDIR(s, pDirHandle)                                   FS_MAP_OpenDir(s, pDirHandle)
  #define FS_READDIR(pDir, pDirEntryInfo)                             FS_MAP_ReadDir(pDir, pDirEntryInfo)
  #define FS_REMOVEDIR(pVolume, s)                                    FS_MAP_RemoveDir(pVolume, s)
  #define FS_RENAME(s, sNewName, pVolume)                             FS_MAP_Rename(s, sNewName, pVolume)
  #define FS_SETDIRENTRYINFO(pVolume, sName, p, Mask)                 FS_MAP_SetDirEntryInfo(pVolume, sName, p, Mask)
  #define FS_SET_END_OF_FILE(pFile)                                   FS_MAP_SetEndOfFile(pFile)
  #define FS_CLEAN_FS(pVolume)                                        FS_MAP_Clean(pVolume)
  #define FS_GET_VOLUME_LABEL(pVolume, pVolLabel, VolLabelSize)       FS_MAP_GetVolumeLabel(pVolume, pVolLabel, VolLabelSize)
  #define FS_SET_VOLUME_LABEL(pVolume, pVolLabel)                     FS_MAP_SetVolumeLabel(pVolume, pVolLabel)
  #define FS_CREATE_JOURNAL_FILE(pVolume, NumBytes, pFirstS, pNumS)   FS_MAP_CreateJournalFile(pVolume, NumBytes, pFirstS, pNumS)
  #define FS_OPEN_JOURNAL_FILE(pVolume)                               FS_MAP_OpenJournalFile(pVolume)
  #define FS_GET_INDEX_OF_LAST_SECTOR(pVolume)                        FS_MAP_GetIndexOfLastSector(pVolume)
  #define FS_CHECKDISK(pVolume, pDiskInfo, pBuffer, BufferSize, MaxRecursionLevel, pfOnError) FS_MAP_CheckDisk(pVolume, pDiskInfo, pBuffer, BufferSize, MaxRecursionLevel, pfOnError)
  #define FS_UPDATE_FILE(pFile)                                       FS_MAP_UpdateFile(pFile)
  #define FS_SET_FILE_SIZE(pFile, NumBytes)                           FS_MAP_SetFileSize(pFile, NumBytes)
  #define FS_FREE_SECTORS(pVolume)                                    FS_MAP_FreeSectors(pVolume)
#else
  #define FS_CHECK_INFOSECTOR(pVolume)                                0
  #define FS_CLOSEDIR(pDir)                                           0
  #define FS_CREATEDIR(pVolume, sDirName)                             0
  #define FS_CLOSE_FILE(pFile)
  #define FS_OPEN_FILE(sFileName, pFile, DoDel, DoOpen, DoCreate)     0
  #define FS_FORMAT(pVolume,  pFormatInfo)                            0
  #define FS_FREAD(pFile, pData, NumBytes)                            0
  #define FS_FWRITE(pFile, pData, NumBytes)                           0
  #define FS_GETDIRENTRYINFO(pVolume, sDirName, p, Mask)              0
  #define FS_GET_DISKINFO(pVolume, pInfo, Flags)                      0
  #define FS_MOVE(sSrc, sDest, pVolume)                               0
  #define FS_OPENDIR(sDirName, pDirHandle)                            0
  #define FS_READDIR(pDir, pDirEntryInfo)                             0
  #define FS_REMOVEDIR(pVolume, sDirName)                             0
  #define FS_RENAME(sOldName, sNewName, pVolume)                      0
  #define FS_SETDIRENTRYINFO(pVolume, sDirName, p, Mask)              0
  #define FS_SET_END_OF_FILE(pFile)                                   0
  #define FS_CLEAN_FS(pVolume)
  #define FS_GET_VOLUME_LABEL(pVolume, pVolumeLabel, VolumeLabelSize) 0
  #define FS_SET_VOLUME_LABEL(pVolume, pVolumeLabel)                  0
  #define FS_CREATE_JOURNAL_FILE(pVolume, NumBytes, pFirstSector, pNumSectors) 0
  #define FS_OPEN_JOURNAL_FILE(pVolume)                               0
  #define FS_GET_INDEX_OF_LAST_SECTOR(pVolume)                        0
  #define FS_CHECKDISK(pVolume, pDiskInfo, pBuffer, BufferSize, MaxRecursionLevel, pfOnError) 0
  #define FS_UPDATE_FILE(pFile)
  #define FS_SET_FILE_SIZE(pFile, NumBytes)                           0
  #define FS_FREE_SECTORS(pVolume)                                    0
#endif

void        FS_MAP_Close               (FS_FILE    * pFile);
int         FS_MAP_CheckFS_API         (FS_VOLUME  * pVolume);
U32         FS_MAP_Read                (FS_FILE    * pFile,           void  * pData, U32 NumBytes);
U32         FS_MAP_Write               (FS_FILE    * pFile,     const void  * pData, U32 NumBytes);
int         FS_MAP_Open                (const char * pFileName, FS_FILE * pFile, U8 DoDel, U8 DoOpen, U8 DoCreate);
int         FS_MAP_Format              (FS_VOLUME  * pVolume,   const FS_FORMAT_INFO * pFormatInfo);
int         FS_MAP_OpenDir             (const char * pDirName,  FS__DIR *pDir);
int         FS_MAP_CloseDir            (FS__DIR    * pDir);
int         FS_MAP_ReadDir             (FS__DIR    * pDir,      FS_DIRENTRY_INFO * pDirEntryInfo);
int         FS_MAP_RemoveDir           (FS_VOLUME  * pVolume,   const char * pDirName);
int         FS_MAP_CreateDir           (FS_VOLUME  * pVolume,   const char * pDirName);
int         FS_MAP_Rename              (const char * sOldName,  const char * sNewName, FS_VOLUME * pVolume);
int         FS_MAP_Move                (const char * sOldName,  const char * sNewName, FS_VOLUME * pVolume);
int         FS_MAP_SetDirEntryInfo     (FS_VOLUME  * pVolume,   const char * sName, const void * p, int Mask);
int         FS_MAP_GetDirEntryInfo     (FS_VOLUME  * pVolume,   const char * sName,       void * p, int Mask);
int         FS_MAP_SetEndOfFile        (FS_FILE    * pFile);
void        FS_MAP_Clean               (FS_VOLUME  * pVolume);
int         FS_MAP_GetDiskInfo         (FS_VOLUME  * pVolume,   FS_DISK_INFO * pDiskData, int Flags);
int         FS_MAP_GetVolumeLabel      (FS_VOLUME  * pVolume,   char * pVolumeLabel, unsigned VolumeLabelSize);
int         FS_MAP_SetVolumeLabel      (FS_VOLUME  * pVolume,   const char * pVolumeLabel);
int         FS_MAP_CreateJournalFile   (FS_VOLUME  * pVolume,   U32 NumBytes, U32 * pFirstSector, U32 * pNumSectors);
int         FS_MAP_OpenJournalFile     (FS_VOLUME  * pVolume);
U32         FS_MAP_GetIndexOfLastSector(FS_VOLUME  * pVolume);
int         FS_MAP_CheckDisk           (FS_VOLUME  * pVolume,   FS_DISK_INFO * pDiskInfo, void * pBuffer, U32 BufferSize, int MaxRecursionLevel, FS_CHECKDISK_ON_ERROR_CALLBACK * pfOnError);
void        FS_MAP_UpdateFile          (FS_FILE    * pFile);
int         FS_MAP_SetFileSize         (FS_FILE    * pFile,     U32 NumBytes);
int         FS_MAP_FreeSectors         (FS_VOLUME  * pVolume);
int         FS__GetFSType              (const FS_VOLUME  * pVolume);

/*********************************************************************
*
*       CLIB
*
*   Optional replacements for standard "C" library routines.
*
**********************************************************************
*/

int         FS__CLIB_atoi    (const char *s);
int         FS__CLIB_memcmp  (const void *s1, const void *s2, unsigned n);
void *      FS__CLIB_memset  (void *s, int c, U32 n);
int         FS__CLIB_strcmp  (const char *s1, const char *s2);
char *      FS__CLIB_strcpy  (char *s1, const char *s2);
unsigned    FS__CLIB_strlen  (const char *s);
int         FS__CLIB_strncmp (const char *s1, const char *s2, int n);
char *      FS__CLIB_strncpy (char *s1, const char *s2, U32 n);
int         FS__CLIB_toupper (int c);
int         FS__CLIB_tolower (int c);
char *      FS__CLIB_strcat  (char *s1, const char *s2);
char *      FS__CLIB_strncat (char *s1, const char *s2, U32 n);
int         FS__CLIB_isupper (int c);
int         FS__CLIB_islower (int c);

#if FS_NO_CLIB
#ifndef   FS_ATOI
  #define FS_ATOI(s)             FS__CLIB_atoi(s)
#endif
#ifndef   FS_MEMCMP
  #define FS_MEMCMP(s1,s2,n)     FS__CLIB_memcmp(s1,s2,n)
#endif
#ifndef   FS_MEMCPY
  #define FS_MEMCPY(s1,s2,n)     FS_memcpy(s1,s2,(int)n)
#endif
#ifndef   FS_MEMSET
  #define FS_MEMSET(s,c,n)       FS__CLIB_memset(s,c,n)
#endif
#ifndef   FS_STRCAT
  #define FS_STRCAT(s1,s2)       FS__CLIB_strcat(s1, s2)
#endif
#ifndef   FS_STRCMP
  #define FS_STRCMP(s1,s2)       FS__CLIB_strcmp(s1,s2)
#endif
#ifndef   FS_STRCPY
  #define FS_STRCPY(s1,s2)       FS__CLIB_strcpy(s1,s2)
#endif
#ifndef   FS_STRLEN
  #define FS_STRLEN(s)           FS__CLIB_strlen(s)
#endif
#ifndef   FS_STRNCAT
  #define FS_STRNCAT(s1,s2,n)    FS__CLIB_strncat(s1, s2, n)
#endif
#ifndef   FS_STRNCMP
  #define FS_STRNCMP(s1,s2,n)    FS__CLIB_strncmp(s1,s2,n)
#endif
#ifndef   FS_STRNCPY
  #define FS_STRNCPY(s1,s2,n)    FS__CLIB_strncpy(s1,s2,n)
#endif
#ifndef   FS_TOUPPER
  #define FS_TOUPPER(c)          FS__CLIB_toupper(c)
#endif
#ifndef   FS_TOLOWER
  #define FS_TOLOWER(c)          FS__CLIB_tolower(c)
#endif
#ifndef   FS_ISUPPER
  #define FS_ISUPPER(c)          FS__CLIB_isupper(c)
#endif
#ifndef   FS_ISLOWER
  #define FS_ISLOWER(c)          FS__CLIB_islower(c)
#endif
#else
#ifndef   FS_ATOI
  #define FS_ATOI(s)             atoi(s)
#endif
#ifndef   FS_MEMCMP
  #define FS_MEMCMP(s1,s2,n)     memcmp(s1,s2,n)
#endif
#ifndef     FS_MEMCPY
  #ifdef __ICCARM__
    #define FS_MEMCPY(s1,s2,n)   FS_memcpy(s1,s2,n)
  #else
    #define FS_MEMCPY(s1,s2,n)   memcpy(s1,s2,n)
  #endif
#endif
#ifndef   FS_MEMSET
  #define FS_MEMSET(s,c,n)       memset(s,c,n)
#endif
#ifndef   FS_STRCAT
  #define FS_STRCAT(s1,s2)       strcat(s1,s2)
#endif
#ifndef   FS_STRCMP
  #define FS_STRCMP(s1,s2)       strcmp(s1,s2)
#endif
#ifndef   FS_STRCPY
  #define FS_STRCPY(s1,s2)       strcpy(s1,s2)
#endif
#ifndef   FS_STRLEN
  #define FS_STRLEN(s)           strlen(s)
#endif
#ifndef   FS_STRNCAT
  #define FS_STRNCAT(s1,s2,n)    strncat(s1, s2, n)
#endif
#ifndef   FS_STRNCMP
  #define FS_STRNCMP(s1,s2,n)    strncmp(s1,s2,n)
#endif
#ifndef   FS_STRNCPY
  #define FS_STRNCPY(s1,s2,n)    strncpy(s1,s2,n)
#endif
#ifndef   FS_TOUPPER
  #define FS_TOUPPER(c)          toupper(c)
#endif
#ifndef   FS_TOLOWER
  #define FS_TOLOWER(c)          tolower(c)
#endif
#ifndef   FS_ISUPPER
  #define FS_ISUPPER(c)          isupper(c)
#endif
#ifndef   FS_ISLOWER
  #define FS_ISLOWER(c)          islower(c)
#endif
#endif

/*********************************************************************
*
*       FS_LB
*
*       Logical block layer
*
**********************************************************************
*/

int    FS_LB_GetStatus           (const FS_DEVICE * pDevice);
U16    FS_GetSectorSize          (FS_DEVICE       * pDevice);
int    FS_LB_GetDeviceInfo       (FS_DEVICE       * pDevice, FS_DEV_INFO * pDevInfo);
int    FS_LB_InitMedium          (FS_DEVICE       * pDevice);
int    FS_LB_InitMediumIfRequired(FS_DEVICE       * pDevice);
int    FS_LB_Ioctl               (FS_DEVICE       * pDevice, I32 Cmd,      I32 Aux,              void * pBuffer);
int    FS_LB_FreeSectorsPart     (FS_PARTITION    * pPart,   U32 SectorIndex, U32 NumSectors);
int    FS_LB_ReadBurst           (FS_DEVICE       * pDevice, U32 SectorIndex, U32 NumSectors,       void * pBuffer, U8 Type);
int    FS_LB_ReadBurstPart       (FS_PARTITION    * pPart,   U32 SectorIndex, U32 NumSectors,       void * pBuffer, U8 Type);
int    FS_LB_ReadDevice          (FS_DEVICE       * pDevice, U32 SectorIndex,                       void * pBuffer, U8 Type);
int    FS_LB_ReadPart            (FS_PARTITION    * pPart,   U32 SectorIndex,                       void * pBuffer, U8 Type);
int    FS_LB_WriteBurst          (FS_DEVICE       * pDevice, U32 SectorIndex, U32 NumSectors, const void * pBuffer, U8 Type, U8 WriteToJournal);
int    FS_LB_WriteBurstPart      (FS_PARTITION    * pPart,   U32 SectorIndex, U32 NumSectors, const void * pBuffer, U8 Type, U8 WriteToJournal);
int    FS_LB_WriteDevice         (FS_DEVICE       * pDevice, U32 SectorIndex,                 const void * pBuffer, U8 Type, U8 WriteToJournal);
int    FS_LB_WritePart           (FS_PARTITION    * pPart,   U32 SectorIndex,                 const void * pBuffer, U8 Type, U8 WriteToJournal);
int    FS_LB_WriteMultiple       (FS_DEVICE       * pDevice, U32 SectorIndex, U32 NumSectors, const void * pBuffer, U8 Type, U8 WriteToJournal);
int    FS_LB_WriteMultiplePart   (FS_PARTITION    * pPart,   U32 SectorIndex, U32 NumSectors, const void * pBuffer, U8 Type, U8 WriteToJournal);
int    FS_LB_WriteBack           (FS_DEVICE       * pDevice, U32 SectorIndex,                 const void * pBuffer);

/*********************************************************************
*
*       FS_CRYPT
*
*       File encryption
*
**********************************************************************
*/
void FS__CRYPT_DecryptBytes(U8 * pDest, const U8 * pSrc, U32 NumBytes, U8 RandKey, const U8 * pFirstKey);
void FS__CRYPT_EncryptBytes(U8 * pDest, const U8 * pSrc, U32 NumBytes, U8 RandKey, const U8 * pFirstKey);

/*********************************************************************
*
*       Read-ahead cache
*
**********************************************************************
*/
#if FS_SUPPORT_READ_AHEAD
  #define FS_ENABLE_READ_AHEAD(pVolume)    FS__IoCtlNL(pVolume, FS_CMD_ENABLE_READ_AHEAD, 0, NULL)
  #define FS_DISABLE_READ_AHEAD(pVolume)   FS__IoCtlNL(pVolume, FS_CMD_DISABLE_READ_AHEAD, 0, NULL)
#else
  #define FS_ENABLE_READ_AHEAD(pVolume)
  #define FS_DISABLE_READ_AHEAD(pVolume)
#endif

/*********************************************************************
*
*       Sector write buffer
*
**********************************************************************
*/
U32 FS__WRBUF_GetNumSectors(U8 Unit);

/*********************************************************************
*
*       Instrumentation via SystemView
*
**********************************************************************
*/

/*********************************************************************
*
*       Profile event identifiers
*/
enum {
  //
  // Events for the FS API functions
  //
  FS_EVTID_INIT = 0,
  FS_EVTID_DEINIT,
  FS_EVTID_MOUNT,
  FS_EVTID_MOUNTEX,
  FS_EVTID_SETAUTOMOUNT,
  FS_EVTID_SYNC,
  FS_EVTID_UNMOUNT,
  FS_EVTID_UNMOUNTFORCED,
  FS_EVTID_ADDDEVICE,
  FS_EVTID_ADDPHYSDEVICE,
  FS_EVTID_ASSIGNMEMORY,
  FS_EVTID_CONFIGFILEBUFFERDEFAULT,
  FS_EVTID_CONFIGONWRITEDIRUPDATE,
  FS_EVTID_LOGVOL_CREATE,
  FS_EVTID_LOGVOL_ADDDEVICE,
  FS_EVTID_SETFILEBUFFERFLAGS,
  FS_EVTID_SETFILEWRITEMODE,
  FS_EVTID_SETFILEWRITEMODEEX,
  FS_EVTID_SETMEMHANDLER,
  FS_EVTID_SETMAXSECTORSIZE,
  FS_EVTID_FCLOSE,
  FS_EVTID_FOPEN,
  FS_EVTID_FOPENEX,
  FS_EVTID_FREAD,
  FS_EVTID_FWRITE,
  FS_EVTID_READ,
  FS_EVTID_SYNCFILE,
  FS_EVTID_WRITE,
  FS_EVTID_FSEEK,
  FS_EVTID_FTELL,
  FS_EVTID_COPYFILE,
  FS_EVTID_COPYFILEEX,
  FS_EVTID_GETFILEATTRIBUTES,
  FS_EVTID_GETFILETIME,
  FS_EVTID_GETFILETIMEEX,
  FS_EVTID_MODIFYFILEATTRIBUTES,
  FS_EVTID_MOVE,
  FS_EVTID_REMOVE,
  FS_EVTID_RENAME,
  FS_EVTID_SETENDOFFILE,
  FS_EVTID_SETFILEATTRIBUTES,
  FS_EVTID_SETFILETIME,
  FS_EVTID_SETFILETIMEEX,
  FS_EVTID_SETFILESIZE,
  FS_EVTID_TRUNCATE,
  FS_EVTID_VERIFY,
  FS_EVTID_WIPEFILE,
  FS_EVTID_CREATEDIR,
  FS_EVTID_FINDCLOSE,
  FS_EVTID_FINDFIRSTFILE,
  FS_EVTID_FINDNEXTFILE,
  FS_EVTID_MKDIR,
  FS_EVTID_RMDIR,
  FS_EVTID_FORMAT,
  FS_EVTID_FORMATLLIFREQUIRED,
  FS_EVTID_FORMATLOW,
  FS_EVTID_ISHLFORMATTED,
  FS_EVTID_ISLLFORMATTED,
  FS_EVTID_CHECKDISK,
  FS_EVTID_CHECKDISK_ERRCODE2TEXT,
  FS_EVTID_CREATEMBR,
  FS_EVTID_FILETIMETOTIMESTAMP,
  FS_EVTID_FREESECTORS,
  FS_EVTID_GETFILESIZE,
  FS_EVTID_GETMAXSECTORSIZE,
  FS_EVTID_GETNUMFILESOPEN,
  FS_EVTID_GETNUMVOLUMES,
  FS_EVTID_GETPARTITIONINFO,
  FS_EVTID_GETVOLUMEFREESPACE,
  FS_EVTID_GETVOLUMEFREESPACEKB,
  FS_EVTID_GETVOLUMEINFO,
  FS_EVTID_GETVOLUMEINFOEX,
  FS_EVTID_GETVOLUMELABEL,
  FS_EVTID_GETVOLUMENAME,
  FS_EVTID_GETVOLUMESIZE,
  FS_EVTID_GETVOLUMESIZEKB,
  FS_EVTID_GETVOLUMESTATUS,
  FS_EVTID_ISVOLUMEMOUNTED,
  FS_EVTID_LOCK,
  FS_EVTID_LOCKVOLUME,
  FS_EVTID_SETBUSYLEDCALLBACK,
  FS_EVTID_SETMEMACCESSCALLBACK,
  FS_EVTID_SETVOLUMELABEL,
  FS_EVTID_TIMESTAMPTOFILETIME,
  FS_EVTID_UNLOCK,
  FS_EVTID_UNLOCKVOLUME,
  FS_EVTID_CLEARERR,
  FS_EVTID_ERRORNO2TEXT,
  FS_EVTID_FEOF,
  FS_EVTID_FERROR,
  FS_EVTID_FAT_GROWROOTDIR,
  FS_EVTID_FORMATSD,
  FS_EVTID_FAT_SUPPORTLFN,
  FS_EVTID_FAT_DISABLELFN,
  FS_EVTID_FAT_CONFIGFATCOPYMAINTENANCE,
  FS_EVTID_FAT_CONFIGFSINFOSECTORUSE,
  FS_EVTID_FAT_CONFIGROFILEMOVEPERMISSION,
  //
  // Events for the storage layer API functions
  //
  FS_EVTID_STORAGE_CLEAN,
  FS_EVTID_STORAGE_CLEANONE,
  FS_EVTID_STORAGE_FREESECTORS,
  FS_EVTID_STORAGE_GETCLEANCNT,
  FS_EVTID_STORAGE_GETCOUNTERS,
  FS_EVTID_STORAGE_GETDEVICEINFO,
  FS_EVTID_STORAGE_GETSECTORUSAGE,
  FS_EVTID_STORAGE_INIT,
  FS_EVTID_STORAGE_READSECTOR,
  FS_EVTID_STORAGE_READSECTORS,
  FS_EVTID_STORAGE_REFRESHSECTORS,
  FS_EVTID_STORAGE_RESETCOUNTERS,
  FS_EVTID_STORAGE_SYNC,
  FS_EVTID_STORAGE_SYNCSECTORS,
  FS_EVTID_STORAGE_UNMOUNT,
  FS_EVTID_STORAGE_WRITESECTOR,
  FS_EVTID_STORAGE_WRITESECTORS,
  //
  // Events for the cache API functions
  //
  FS_EVTID_ASSIGNCACHE,
  FS_EVTID_CACHE_CLEAN,
  FS_EVTID_CACHE_INVALIDATE,
  FS_EVTID_CACHE_SETASSOCLEVEL,
  FS_EVTID_CACHE_SETMODE,
  FS_EVTID_CACHE_SETQUOTA,
  //
  // Events for the journal API functions
  //
  FS_EVTID_JOURNAL_BEGIN,
  FS_EVTID_JOURNAL_CREATE,
  FS_EVTID_JOURNAL_CREATEEX,
  FS_EVTID_JOURNAL_DISABLE,
  FS_EVTID_JOURNAL_ENABLE,
  FS_EVTID_JOURNAL_END,
  //
  // Events for the encryption API functions
  //
  FS_EVTID_CRYPT_PREPARE,
  FS_EVTID_SETENCRYPTIONOBJECT,
  //
  // Events for the logical block layer.
  //
  FS_EVTID_LB_GETSTATUS,
  FS_EVTID_LB_GETDEVICEINFO,
  FS_EVTID_LB_INITMEDIUM,
  FS_EVTID_LB_IOCTL,
  FS_EVTID_LB_FREESECTORS,
  FS_EVTID_LB_READBURST,
  FS_EVTID_LB_READDEVICE,
  FS_EVTID_LB_WRITEBURST,
  FS_EVTID_LB_WRITEDEVICE,
  FS_EVTID_LB_WRITEMULTIPLE,
  FS_EVTID_LB_WRITEBACK,
  //
  // Make sure this is always the last entry.
  //
  FS_NUM_EVTIDS
};

#define FS_PROFILE_GET_EVENT_ID(EvtId)    ((EvtId) + FS_Global.Profile.IdOffset)

/*********************************************************************
*
*       FS_PROFILE_END_CALL
*/
#if FS_SUPPORT_PROFILE && FS_SUPPORT_PROFILE_END_CALL
  #define FS_PROFILE_END_CALL(EventId)                                             \
    do {                                                                           \
      if (FS_Global.Profile.pAPI) {                                                \
        FS_Global.Profile.pAPI->pfRecordEndCall(FS_PROFILE_GET_EVENT_ID(EventId)); \
      }                                                                            \
    } while (0)
#else
  #define FS_PROFILE_END_CALL(EventId)
#endif

/*********************************************************************
*
*       FS_PROFILE_END_CALL_U32
*/
#if FS_SUPPORT_PROFILE && FS_SUPPORT_PROFILE_END_CALL
  #define FS_PROFILE_END_CALL_U32(EventId, Para0)                                                   \
    do {                                                                                            \
      if (FS_Global.Profile.pAPI) {                                                                 \
        FS_Global.Profile.pAPI->pfRecordEndCallU32(FS_PROFILE_GET_EVENT_ID(EventId), (U32)(Para0)); \
      }                                                                                             \
    } while (0)
#else
  #define FS_PROFILE_END_CALL_U32(EventId, ReturnValue)
#endif

/*********************************************************************
*
*       FS_PROFILE_CALL_VOID
*/
#if FS_SUPPORT_PROFILE
  #define FS_PROFILE_CALL_VOID(EventId)                                        \
   do {                                                                        \
     if (FS_Global.Profile.pAPI) {                                             \
       FS_Global.Profile.pAPI->pfRecordVoid(FS_PROFILE_GET_EVENT_ID(EventId)); \
     }                                                                         \
   } while (0)
#else
  #define FS_PROFILE_CALL_VOID(EventId)
#endif

/*********************************************************************
*
*       FS_PROFILE_CALL_U32
*/
#if FS_SUPPORT_PROFILE
  #define FS_PROFILE_CALL_U32(EventId, Para0)                                                \
    do {                                                                                     \
      if (FS_Global.Profile.pAPI) {                                                          \
        FS_Global.Profile.pAPI->pfRecordU32(FS_PROFILE_GET_EVENT_ID(EventId), (U32)(Para0)); \
      }                                                                                      \
    } while (0)
#else
  #define FS_PROFILE_CALL_U32(EventId, Para0)
#endif

/*********************************************************************
*
*       FS_PROFILE_CALL_U32x2
*/
#if FS_SUPPORT_PROFILE
  #define FS_PROFILE_CALL_U32x2(EventId, Para0, Para1)                                                       \
    do {                                                                                                     \
      if (FS_Global.Profile.pAPI) {                                                                          \
        FS_Global.Profile.pAPI->pfRecordU32x2(FS_PROFILE_GET_EVENT_ID(EventId), (U32)(Para0), (U32)(Para1)); \
      }                                                                                                      \
    } while (0)
#else
  #define FS_PROFILE_CALL_U32x2(Id, Para0, Para1)
#endif

/*********************************************************************
*
*       FS_PROFILE_CALL_U32x3
*/
#if FS_SUPPORT_PROFILE
  #define FS_PROFILE_CALL_U32x3(EventId, Para0, Para1, Para2)                                                              \
    do {                                                                                                                   \
      if (FS_Global.Profile.pAPI) {                                                                                        \
        FS_Global.Profile.pAPI->pfRecordU32x3(FS_PROFILE_GET_EVENT_ID(EventId), (U32)(Para0), (U32)(Para1), (U32)(Para2)); \
      }                                                                                                                    \
    } while (0)
#else
  #define FS_PROFILE_CALL_U32x3(EventId, Para0, Para1, Para2)
#endif

/*********************************************************************
*
*       FS_PROFILE_CALL_U32x4
*/
#if FS_SUPPORT_PROFILE
  #define FS_PROFILE_CALL_U32x4(EventId, Para0, Para1, Para2, Para3)                                                                   \
  do {                                                                                                                                 \
    if (FS_Global.Profile.pAPI) {                                                                                                      \
      FS_Global.Profile.pAPI->pfRecordU32x4(FS_PROFILE_GET_EVENT_ID(EventId), (U32)(Para0), (U32)(Para1), (U32)(Para2), (U32)(Para3)); \
    }                                                                                                                                  \
  } while (0)
#else
  #define FS_PROFILE_CALL_U32x4(EventId, Para0, Para1, Para2, Para3)
#endif

/*********************************************************************
*
*       FS_PROFILE_RECORD_API_U32x5
*/
#if FS_SUPPORT_PROFILE
  #define FS_PROFILE_CALL_U32x5(EventId, Para0, Para1, Para2, Para3, Para4)                                                                          \
  do {                                                                                                                                               \
    if (FS_Global.Profile.pAPI) {                                                                                                                    \
      FS_Global.Profile.pAPI->pfRecordU32x5(FS_PROFILE_GET_EVENT_ID(EventId), (U32)(Para0), (U32)(Para1), (U32)(Para2), (U32)(Para3), (U32)(Para4)); \
    }                                                                                                                                                \
  } while (0)
#else
  #define FS_PROFILE_CALL_U32x5(EventId, Para0, Para1, Para2, Para3, Para4)
#endif

/*********************************************************************
*
*       FS_PROFILE_CALL_U32x6
*/
#if FS_SUPPORT_PROFILE
  #define FS_PROFILE_CALL_U32x6(EventId, Para0, Para1, Para2, Para3, Para4, Para5)                                                                                 \
  do {                                                                                                                                                             \
    if (FS_Global.Profile.pAPI) {                                                                                                                                  \
      FS_Global.Profile.pAPI->pfRecordU32x6(FS_PROFILE_GET_EVENT_ID(EventId), (U32)(Para0), (U32)(Para1), (U32)(Para2), (U32)(Para3), (U32)(Para4), (U32)(Para5)); \
    }                                                                                                                                                              \
  } while (0)
#else
  #define FS_PROFILE_CALL_U32x6(EventId, Para0, Para1, Para2, Para3, Para4, Para5)
#endif

/*********************************************************************
*
*       FS_PROFILE_CALL_U32x7
*/
#if FS_SUPPORT_PROFILE
  #define FS_PROFILE_CALL_U32x7(EventId, Para0, Para1, Para2, Para3, Para4, Para5, Para6)                                                                                        \
  do {                                                                                                                                                                           \
    if (FS_Global.Profile.pAPI) {                                                                                                                                                \
      FS_Global.Profile.pAPI->pfRecordU32x7(FS_PROFILE_GET_EVENT_ID(EventId), (U32)(Para0), (U32)(Para1), (U32)(Para2), (U32)(Para3), (U32)(Para4), (U32)(Para5), (U32)(Para6)); \
    }                                                                                                                                                                            \
  } while (0)
#else
  #define FS_PROFILE_CALL_U32x7(EventId, Para0, Para1, Para2, Para3, Para4, Para5, Para6)
#endif

/*********************************************************************
*
*       FS_PROFILE_CALL_STRING
*/
#if FS_SUPPORT_PROFILE
  #define FS_PROFILE_CALL_STRING(EventId, pPara0)                                                          \
    do {                                                                                                   \
      if (FS_Global.Profile.pAPI) {                                                                        \
        FS_Global.Profile.pAPI->pfRecordString(FS_PROFILE_GET_EVENT_ID(EventId), (const char *)(pPara0));  \
      }                                                                                                    \
    } while (0)
#else
  #define FS_PROFILE_CALL_STRING(EventId, pPara0)
#endif

/*********************************************************************
*
*       FS_PROFILE_RECORD_API_STRINGx2
*/
#if FS_SUPPORT_PROFILE
  #define FS_PROFILE_CALL_STRINGx2(EventId, pPara0, pPara1)                                                                          \
    do {                                                                                                                            \
      if (FS_Global.Profile.pAPI) {                                                                                                 \
        FS_Global.Profile.pAPI->pfRecordStringx2(FS_PROFILE_GET_EVENT_ID(EventId), (const char *)(pPara0), (const char *)(pPara1)); \
      }                                                                                                                             \
    } while (0)
#else
  #define FS_PROFILE_CALL_STRINGx2(EventId, pPara0, pPara1)
#endif

/*********************************************************************
*
*       Public const
*
**********************************************************************
*/
#if FS_SUPPORT_MULTIPLE_FS
  extern const FS_FS_API FS_FAT_API;
  extern const FS_FS_API FS_EFS_API;
#endif

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/
extern FS_STORAGE_COUNTERS FS_STORAGE_Counters;

#if defined(__cplusplus)
}                /* Make sure we have C-declarations in C++ programs */
#endif

#undef EXTERN

#endif // __FS_INT_H__

/*************************** End of file ****************************/
