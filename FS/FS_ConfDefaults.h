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
File    : FS_ConfDefaults.h
Purpose : File system configuration defaults
--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef FS_CONFDEFAULTS_H                   // Avoid recursive and multiple inclusion
#define FS_CONFDEFAULTS_H

#include "FS_Conf.h"
#include "SEGGER.h"
#include "FS_Debug.h"

/*********************************************************************
*
*       File system defaults
*
*/
#ifndef   FS_LOG_MASK_DEFAULT
  #define FS_LOG_MASK_DEFAULT           (1 << 0)   // FS_MTYPE_INIT
#endif

#ifndef   FS_MAX_PATH
  #define FS_MAX_PATH                   260
#endif

#ifndef   FS_SUPPORT_FAT
  #define FS_SUPPORT_FAT                1     // Support the FAT file system if enabled
#endif

#ifndef   FS_SUPPORT_EFS
  #define FS_SUPPORT_EFS                0     // Support the EFS file system if enabled
#endif

#ifndef   FS_SUPPORT_MULTIPLE_FS
  #define FS_SUPPORT_MULTIPLE_FS        ((FS_SUPPORT_EFS) && (FS_SUPPORT_FAT))
#endif

#ifndef   FS_SUPPORT_FREE_SECTOR
  #define FS_SUPPORT_FREE_SECTOR        1     // Informs lower layer of unused sectors ... Makes sense only for drivers which use it
#endif

#ifndef   FS_SUPPORT_CACHE
  #define FS_SUPPORT_CACHE              1     // Set to 1 to enable the support for sector caching.
                                              // The cache has to be enabled at runtime by calling FS_AssignCache().
#endif

#ifndef   FS_SUPPORT_ENCRYPTION
  #define FS_SUPPORT_ENCRYPTION         0     // Set to 1 to enable support for encryption at file level.
                                              // The encryption has to be enabled at runtime by calling FS_SetEncryptionObject().
#endif

#ifndef   FS_MULTI_HANDLE_SAFE
  #define FS_MULTI_HANDLE_SAFE          0     // Set to 1 to enable read and write access to same file from different tasks.
#endif

#ifndef   FS_MAX_LEN_FULL_FILE_NAME
  #define FS_MAX_LEN_FULL_FILE_NAME     256   // Maximum number of characters allowed in a full path to file. Used when FS_MULTI_HANDLE_SAFE is set to 1.
#endif

#ifndef   FS_DRIVER_ALIGNMENT
  //
  // Added for compatibility reasons.
  //
  #ifdef    FS_DRIVER_ALIGMENT
    #define FS_DRIVER_ALIGNMENT         FS_DRIVER_ALIGMENT
  #else
    #define FS_DRIVER_ALIGNMENT         4     // The file system performs a 0-copy operation if the alignment of data buffer passed by the application
                                              // matches this value. The value is specified in bytes.
  #endif
#endif

#ifndef   FS_DIRECTORY_DELIMITER
  #define FS_DIRECTORY_DELIMITER        '\\'  // This is the character used for separating directory names in a path. Typ. '\\' or '/'.
#endif

#ifndef   FS_VERIFY_WRITE
  #define FS_VERIFY_WRITE               0     // Set to 1 to verify every write sector operation (tests the driver and hardware)
#endif

#ifndef   FS_SUPPORT_BUSY_LED
  #define FS_SUPPORT_BUSY_LED           1     // If set to 1 the file system calls the function registered via FS_SetBusyLEDCallback()
                                              // at the beginning and at the end of an access to the storage medium.
#endif

#ifndef   FS_SUPPORT_CHECK_MEMORY
  #define FS_SUPPORT_CHECK_MEMORY       0   // Check whether the memory is accessible for device driver, otherwise burst operation are not used.
                                            // To check the memory, please make sure the FS_SetMemAccessCallback() is called for the appropriate
                                            // volume/device.
#endif

#ifndef   FS_SUPPORT_JOURNAL
  #define FS_SUPPORT_JOURNAL            0
#endif

#ifndef   FS_OPTIMIZE
  #define FS_OPTIMIZE                       // Allow optimizations such as "__arm __ramfunc" or similar. What works best depends on target.
#endif

#ifndef   FS_SUPPORT_DEINIT
  #define FS_SUPPORT_DEINIT             0   // Allows to have a deinitialization of the file system
#endif

#ifndef   FS_SUPPORT_EXT_MEM_MANAGER
  #define FS_SUPPORT_EXT_MEM_MANAGER    0   // Set to 1 will use external alloc/free memory functions, these must be set with FS_SetMemFunc()
#endif

#ifndef   FS_JOURNAL_FILE_NAME
  #define FS_JOURNAL_FILE_NAME          "Journal.dat"   // This is the full path to the file to be used by the Journal module.
#endif

#ifndef   FS_USE_FILE_BUFFER
  #define FS_USE_FILE_BUFFER            1   // Enables file buffering, which makes file access faster when writing/reading small amounts of data.
#endif

#ifndef   FS_SUPPORT_FILE_BUFFER
  #define FS_SUPPORT_FILE_BUFFER        0   // Configures the type of file buffering at file handle level.
#endif

#ifndef   FS_SUPPORT_READ_AHEAD
  #define FS_SUPPORT_READ_AHEAD         0   // When set to 1 the file system activates the functionality which requests the FS_READAHEAD_Driver
                                            // to read sector data in advance. This is typically activated for search operations in the allocation table.
#endif

#ifndef   FS_HUGE
  #define FS_HUGE
#else
  #define FS_HUGE  huge
#endif

#ifndef     FS_X_PANIC
  #if       FS_DEBUG_LEVEL >= FS_DEBUG_LEVEL_CHECK_PARA
    #define FS_X_PANIC(ErrorCode)  FS_X_Panic(ErrorCode)
  #else
    #define FS_X_PANIC(ErrorCode)
  #endif
#endif

#ifndef   FS_SUPPORT_PROFILE
  #define FS_SUPPORT_PROFILE            0   // Profiling instrumentation (via SystemView by default). Disabled by default for performance reasons.
#endif

#ifndef   FS_SUPPORT_PROFILE_END_CALL
  #define FS_SUPPORT_PROFILE_END_CALL   0   // Profiling instrumentation of function return events. Disabled by default to avoid generating too many events.
#endif

/*********************************************************************
*
*       FAT File System Layer defines
*/
#ifndef   FS_FAT_SUPPORT_FAT32
  #define FS_FAT_SUPPORT_FAT32          1                   // 0 disables FAT32 support
#endif

#ifndef   FS_FAT_SUPPORT_UTF8
  #define FS_FAT_SUPPORT_UTF8           0                   // Use UTF-8 encoding to support Unicode characters
#endif

#ifndef   FS_UNICODE_UPPERCASE_EXT
  #define FS_UNICODE_UPPERCASE_EXT      {0x0000, 0x0000}    // Allow to extend the static Unicode lower to upper case table
#endif

#ifndef   FS_MAINTAIN_FAT_COPY
  #define FS_MAINTAIN_FAT_COPY          0                   // Shall the 2nd FAT (copy) be maintained
#endif

#ifndef   FS_FAT_USE_FSINFO_SECTOR
  #define FS_FAT_USE_FSINFO_SECTOR      1                   // Use and update FSInfo sector on FAT32 media. For FAT12/FAT116 there is no FSInfo sector
#endif

#ifndef   FS_FAT_OPTIMIZE_DELETE
  #define FS_FAT_OPTIMIZE_DELETE        1                   // Accelerate delete of large files
#endif

#ifndef   FS_FAT_PERMIT_RO_FILE_MOVE
  #define FS_FAT_PERMIT_RO_FILE_MOVE    0                   // When set to 1 read-only files and directories can be moved or renamed
#endif

#ifndef   FS_FAT_UPDATE_DIRTY_FLAG
  #define FS_FAT_UPDATE_DIRTY_FLAG      0                   // When set to 1 a flag is updated in the boot sector to indicate that the volume was correctly unmounted before reset
#endif

/*********************************************************************
*
*       EFS File System Layer defines
*/
#ifndef   FS_EFS_OPTIMIZE_DELETE
  #define FS_EFS_OPTIMIZE_DELETE        1                   // Accelerate delete of large files
#endif

#ifndef   FS_EFS_CASE_SENSITIVE
  #define FS_EFS_CASE_SENSITIVE         0                   // Should the file/directory names be case sensitively compared
#endif

#ifndef   FS_EFS_MAX_DIR_ENTRY_SIZE
  #define FS_EFS_MAX_DIR_ENTRY_SIZE     255                 // Maximum number of bytes of an EFS directory entry
#endif

/*********************************************************************
*
*       CLib
*/
#ifndef   FS_NO_CLIB
  #define FS_NO_CLIB  0
#endif

/*********************************************************************
*
*       OS Layer
*/
#ifndef   FS_OS_LOCKING
  #define FS_OS_LOCKING  0
#endif

#ifdef FS_OS_LOCKING
  #if   FS_OS_LOCKING == 0
    #define FS_OS                 0
    #define FS_OS_LOCK_PER_DRIVER 0
  #elif FS_OS_LOCKING == 1
    #define FS_OS                 1
    #define FS_OS_LOCK_PER_DRIVER 0
  #elif FS_OS_LOCKING == 2
    #define FS_OS                 1
    #define FS_OS_LOCK_PER_DRIVER 1
  #else
  #error FS_OS_LOCKING has an invalid value
  #endif
#endif

#ifndef   FS_OS
  #define FS_OS                 0
#endif

#ifndef   FS_OS_LOCK_PER_DRIVER
  #define FS_OS_LOCK_PER_DRIVER 0        // 0 = means a single lock for all files, 1 means one lock per file
#endif

/*********************************************************************
*
*       FS_USE_PARA
*
*   Typ. used to avoid warnings for undefined parameters.
*/
#ifndef FS_USE_PARA
  #if defined(NC30) || defined(NC308) || defined(NC100)
    #define FS_USE_PARA(para)
  #else
    #define FS_USE_PARA(para) (void)(para)
  #endif
#endif

/*********************************************************************
*
*       Number of
*/
#ifndef   FS_NUM_VOLUMES
  #define FS_NUM_VOLUMES  4
#endif

#ifndef   FS_NUM_DIR_HANDLES
  #define FS_NUM_DIR_HANDLES  1
#endif

#ifndef FS_NUM_MEMBLOCKS_PER_OPERATION
  //
  // FAT file system needs at least 2 sector buffers for a FS operation
  //
  #if (FS_SUPPORT_EFS == 0)
    #if FS_FAT_UPDATE_DIRTY_FLAG
      //
      // An additional sector buffer is required to update the "dirty" flag in the boot sector.
      //
      #define FS_NUM_SECTOR_BUFFERS_FS  3
    #else
      #define FS_NUM_SECTOR_BUFFERS_FS  2
    #endif
  //
  // EFS needs at least 3 sector buffers for a FS operation
  //
  #else
    #define FS_NUM_SECTOR_BUFFERS_FS  3
  #endif
  //
  // File encryption requires one additional sector buffer.
  //
  #if FS_SUPPORT_ENCRYPTION
    #define FS_NUM_SECTOR_BUFFERS_ENCRYPTION  1
  #else
    #define FS_NUM_SECTOR_BUFFERS_ENCRYPTION  0
  #endif
  //
  // Journaling requires one additional sector buffer.
  //
  #if FS_SUPPORT_JOURNAL
    #define FS_NUM_SECTOR_BUFFERS_JOURNAL     1
  #else
    #define FS_NUM_SECTOR_BUFFERS_JOURNAL     0
  #endif
  //
  // Total number of sector buffers.
  //
  #define FS_NUM_MEMBLOCKS_PER_OPERATION      (FS_NUM_SECTOR_BUFFERS_FS + FS_NUM_SECTOR_BUFFERS_ENCRYPTION + FS_NUM_SECTOR_BUFFERS_JOURNAL)
#endif

/*********************************************************************
*
*       Testing (for internal use only)
*/
#ifndef   FS_SUPPORT_TEST
  #define FS_SUPPORT_TEST  0
#endif

#endif // __FS_CONFDEFAULTS_H__

/****** End of file *************************************************/
