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
File        : FS_Types.h
Purpose     : Common types and defines.
---------------------------END-OF-HEADER------------------------------
*/

#ifndef __FS_TYPES_H__               // Avoid recursive and multiple inclusion
#define __FS_TYPES_H__

#include "SEGGER.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Defines non configurable
*
**********************************************************************
*/
#define FS_FILE_ACCESS_FLAG_A (1 << 0)
#define FS_FILE_ACCESS_FLAG_B (1 << 1)
#define FS_FILE_ACCESS_FLAG_C (1 << 2)
#define FS_FILE_ACCESS_FLAG_R (1 << 3)
#define FS_FILE_ACCESS_FLAG_W (1 << 4)
//
// Combined access modes which are frequently used
//
#define FS_FILE_ACCESS_FLAGS_AW     (FS_FILE_ACCESS_FLAG_A | FS_FILE_ACCESS_FLAG_W)
#define FS_FILE_ACCESS_FLAGS_BR     (FS_FILE_ACCESS_FLAG_B | FS_FILE_ACCESS_FLAG_R)
#define FS_FILE_ACCESS_FLAGS_CW     (FS_FILE_ACCESS_FLAG_C | FS_FILE_ACCESS_FLAG_W)
#define FS_FILE_ACCESS_FLAGS_RW     (FS_FILE_ACCESS_FLAG_R | FS_FILE_ACCESS_FLAG_W)

#define FS_FILE_ACCESS_FLAGS_ACW    (FS_FILE_ACCESS_FLAG_C | FS_FILE_ACCESS_FLAGS_AW)
#define FS_FILE_ACCESS_FLAGS_ARW    (FS_FILE_ACCESS_FLAG_A | FS_FILE_ACCESS_FLAGS_RW)
#define FS_FILE_ACCESS_FLAGS_BCW    (FS_FILE_ACCESS_FLAG_B | FS_FILE_ACCESS_FLAGS_CW)
#define FS_FILE_ACCESS_FLAGS_BRW    (FS_FILE_ACCESS_FLAG_W | FS_FILE_ACCESS_FLAGS_BR)
#define FS_FILE_ACCESS_FLAGS_CRW    (FS_FILE_ACCESS_FLAG_C | FS_FILE_ACCESS_FLAG_R | FS_FILE_ACCESS_FLAG_W)

#define FS_FILE_ACCESS_FLAGS_ABCW   (FS_FILE_ACCESS_FLAG_B | FS_FILE_ACCESS_FLAGS_ACW)
#define FS_FILE_ACCESS_FLAGS_ACRW   (FS_FILE_ACCESS_FLAG_A | FS_FILE_ACCESS_FLAGS_CRW)
#define FS_FILE_ACCESS_FLAGS_BCRW   (FS_FILE_ACCESS_FLAG_B | FS_FILE_ACCESS_FLAGS_CRW)

#define FS_FILE_ACCESS_FLAGS_ABCRW  (FS_FILE_ACCESS_FLAGS_ACRW | FS_FILE_ACCESS_FLAG_B)

//
// Directory entry get/set macros 
//
#define FS_DIRENTRY_GET_ATTRIBUTES             (1 << 0)
#define FS_DIRENTRY_GET_TIMESTAMP_CREATE       (1 << 1)
#define FS_DIRENTRY_GET_TIMESTAMP_MODIFY       (1 << 2)
#define FS_DIRENTRY_GET_TIMESTAMP_ACCESS       (1 << 3)
#define FS_DIRENTRY_SET_ATTRIBUTES             (1 << 0)
#define FS_DIRENTRY_SET_TIMESTAMP_CREATE       (1 << 1)
#define FS_DIRENTRY_SET_TIMESTAMP_MODIFY       (1 << 2)
#define FS_DIRENTRY_SET_TIMESTAMP_ACCESS       (1 << 3)

//
//  Smart buffer (SB) type defines
//
#define FS_SB_TYPE_DATA               FS_SECTOR_TYPE_DATA
#define FS_SB_TYPE_MANAGEMENT         FS_SECTOR_TYPE_MAN
#define FS_SB_TYPE_DIRECTORY          FS_SECTOR_TYPE_DIR

/*********************************************************************
*
*       Sector type defines
*/
#define FS_SECTOR_TYPE_DATA    0
#define FS_SECTOR_TYPE_DIR     1
#define FS_SECTOR_TYPE_MAN     2
#define FS_SECTOR_TYPE_COUNT   3   /* Number of sector types */

/*********************************************************************
*
*       Sector type defines
*/
#define FS_SECTOR_TYPE_MASK_DATA    (1 << FS_SECTOR_TYPE_DATA)
#define FS_SECTOR_TYPE_MASK_DIR     (1 << FS_SECTOR_TYPE_DIR)
#define FS_SECTOR_TYPE_MASK_MAN     (1 << FS_SECTOR_TYPE_MAN)
#define FS_SECTOR_TYPE_MASK_ALL     (FS_SECTOR_TYPE_MASK_DATA | FS_SECTOR_TYPE_MASK_DIR | FS_SECTOR_TYPE_MASK_MAN)

/*********************************************************************
*
*       Data types, opaque
*
**********************************************************************
*/
typedef struct FS_FAT_INFO    FS_FAT_INFO;
typedef struct FS_EFS_INFO    FS_EFS_INFO;
typedef struct FS_SB          FS_SB;

/*********************************************************************
*
*       Internal data types, used as opaque types for pointers
*
**********************************************************************
*/
typedef struct FS_CACHE_API       FS_CACHE_API;
typedef struct FS_DEVICE          FS_DEVICE;
typedef struct FS_DIR             FS_DIR;
typedef struct FS_FILE            FS_FILE;
typedef struct FS_PARTITION       FS_PARTITION;
typedef struct FS_VOLUME          FS_VOLUME;
typedef struct FS_DEVICE_TYPE     FS_DEVICE_TYPE;
typedef struct FS_DIRENT          FS_DIRENT;
typedef struct FS_DEV_INFO        FS_DEV_INFO;
typedef struct FS_FORMAT_INFO_EX  FS_FORMAT_INFO_EX;

#if defined(__cplusplus)
}                /* Make sure we have C-declarations in C++ programs */
#endif

#endif // __FS_TYPES_H__

/*************************** End of file ****************************/
