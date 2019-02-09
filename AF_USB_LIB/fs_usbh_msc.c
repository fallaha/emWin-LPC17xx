/*------------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *------------------------------------------------------------------------------
 *      Name:    fs_usbh_msc.c 
 *      Purpose: USB Host Mass Storage Class Interface for the FlashFS
 *      Rev.:    V4.70
 *------------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "mFileConfig.h"
#include <rl_usb.h>

#include <rl_usb.h>
/*------------------------------------------------------------------------------
  USB-MSC FAT Driver instance definition
   usb0_drv: First USB Flash drive  [U0:]
   usb1_drv: Second USB Flash drive [U1:]
 *----------------------------------------------------------------------------*/

#define __DRV_ID  usb0_drv
#define __CPUCLK  100000000

/* USB-MSC Driver Interface functions */
static BOOL Init        (U32 mode);
static BOOL UnInit      (U32 mode);
static BOOL ReadSector  (U32 sect, U8 *buf, U32 cnt);
static BOOL WriteSector (U32 sect, U8 *buf, U32 cnt);
static BOOL ReadInfo    (Media_INFO *info);
static U32  DeviceCtrl  (U32 code, void *p);

/* USB-MSC Device Driver Control Block */
FAT_DRV __DRV_ID = {
  Init,
  UnInit,
  ReadSector,
  WriteSector,
  ReadInfo,
  DeviceCtrl
};


/* Local definitions */
#define WAIT_CNT(ck,us)   ((ck/3000000)*us)

/* Local variables */
static BIT media_ok;

/* Local Functions */
static void Delay (U32 us);

/*--------------------------- Init -------------------------------------------*/

static BOOL Init (U32 mode) {
  /* Initialize USB Host. */
  U32 cnt;

  if (mode == DM_IO) {
    /* Initialise USB hardware. */
    media_ok = __FALSE;
    return (usbh_init(0));
  }

  if (mode == DM_MEDIA) {
    for (cnt = 0; cnt < 2500; cnt++) {
      usbh_engine(0);
      if (usbh_msc_status(0, 0) == __TRUE) {
        media_ok = __TRUE;
        return (__TRUE);
      }
      Delay (1000);
    }
  }
  return (__FALSE);
}


/*--------------------------- UnInit -----------------------------------------*/

static BOOL UnInit (U32 mode) {
  /* UnInitialize USB Host. */

  if (mode == DM_IO) {
    /* UnInitialize USB hardware. */
    return (usbh_uninit(0));
  }
  if (mode == DM_MEDIA) {
    return (__TRUE);
  }
  return (__FALSE);
}


/*--------------------------- ReadSector -------------------------------------*/

static BOOL ReadSector (U32 sect, U8 *buf, U32 cnt) {
  /* Read single/multiple sectors from Mass Storage Device. */

  return (usbh_msc_read(0, 0, sect, buf, cnt));
}


/*--------------------------- WriteSector ------------------------------------*/

static BOOL WriteSector (U32 sect, U8 *buf, U32 cnt) {
  /* Write single/multiple sectors to Mass Storage Device. */

  return (usbh_msc_write(0, 0, sect, buf, cnt));
}


/*--------------------------- ReadInfo ---------------------------------------*/

static BOOL ReadInfo (Media_INFO *info) {
  /* Read Mass Storage Device configuration. */
  U32 blen;

  if (!usbh_msc_read_config(0, 0, &info->block_cnt, &blen)) { 
    /* Fail, Mass Storage Device configuration was not read. */
    return (__FALSE);
  }
  info->write_blen = info->read_blen = (U16)blen;
  return (__TRUE);
}


/*--------------------------- DeviceCtrl -------------------------------------*/

static U32 DeviceCtrl (U32 code, void *p) {
  /* Device Control system call. */

  if (code != DC_CHKMEDIA) {
    return (0);
  }
  /* Read Device Detected status. */
  if (media_ok == __FALSE) {
    /* Allow to initialize the media first. */
    return (M_INSERTED);
  }
  /* Allow USB Host to detect and enumerate the device. */
  usbh_engine(0);
  if (usbh_msc_status(0, 0) == __TRUE) {
    return (M_INSERTED);
  }
  return (0);
}


/*--------------------------- Delay ------------------------------------------*/

static void Delay (U32 us) {
  /* Approximate delay in micro seconds. */
  U32 i;

  i = WAIT_CNT(__CPUCLK, us);
  while (i--);
}

/*------------------------------------------------------------------------------
 * end of file
 *----------------------------------------------------------------------------*/
