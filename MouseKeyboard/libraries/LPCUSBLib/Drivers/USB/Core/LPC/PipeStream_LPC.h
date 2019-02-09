/*
* Copyright(C) NXP Semiconductors, 2011
* All rights reserved.
*
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* LPC products.  This software is supplied "AS IS" without any warranties of
* any kind, and NXP Semiconductors and its licensor disclaim any and 
* all warranties, express or implied, including all implied warranties of 
* merchantability, fitness for a particular purpose and non-infringement of 
* intellectual property rights.  NXP Semiconductors assumes no responsibility
* or liability for the use of the software, conveys no license or rights under any
* patent, copyright, mask work right, or any other intellectual property rights in 
* or to any products. NXP Semiconductors reserves the right to make changes
* in the software without notification. NXP Semiconductors also makes no 
* representation or warranty that such application will be suitable for the
* specified use without further testing or modification.
* 
* Permission to use, copy, modify, and distribute this software and its 
* documentation is hereby granted, under NXP Semiconductors' and its 
* licensor's relevant copyrights in the software, without fee, provided that it 
* is used in conjunction with NXP Semiconductors microcontrollers.  This 
* copyright, permission, and disclaimer notice must appear in all copies of 
* this code.
*/

 
/** \file
 *  \brief Pipe data stream transmission and reception management for the LPC microcontrollers
 *  \copydetails Group_PipeStreamRW_LPC
 *
 *  \note This file should not be included directly. It is automatically included as needed by the USB driver
 *        dispatch header located in lpcroot/libraries/LPCUSBlib/Drivers/USB/USB.h.
 */

/** \ingroup Group_PipeStreamRW
 *  \defgroup Group_PipeStreamRW_LPC Read/Write of Multi-Byte Streams (LPC)
 *  \brief Pipe data stream transmission and reception management for the LPC architecture.
 *
 *  Functions, macros, variables, enums and types related to data reading and writing of data streams from
 *  and to pipes.
 *
 *  @{
 */

#ifndef __PIPE_STREAM_LPC_H__
#define __PIPE_STREAM_LPC_H__

	/* Includes: */
		#include "../../../../Common/Common.h"
		#include "../USBMode.h"		
		#include "../USBTask.h"

	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_USB_DRIVER)
			#error Do not include this file directly. Include lpcroot/libraries/LPCUSBlib/Drivers/USB/USB.h instead.
		#endif
		
	/* Public Interface - May be used in end-application: */
		/* Function Prototypes: */
			/** \name Stream functions for null data */
			//@{

			/** Reads and discards the given number of bytes from the pipe, discarding fully read packets from the host
			 *  as needed. The last packet is not automatically discarded once the remaining bytes has been read; the
			 *  user is responsible for manually discarding the last packet from the device via the \ref Pipe_ClearIN() macro.
			 *
			 *  If the BytesProcessed parameter is \c NULL, the entire stream transfer is attempted at once, failing or
			 *  succeeding as a single unit. If the BytesProcessed parameter points to a valid storage location, the transfer
			 *  will instead be performed as a series of chunks. Each time the pipe bank becomes empty while there is still data
			 *  to process (and after the current packet has been acknowledged) the BytesProcessed location will be updated with
			 *  the total number of bytes processed in the stream, and the function will exit with an error code of
			 *  \ref PIPE_RWSTREAM_IncompleteTransfer. This allows for any abort checking to be performed in the user code - to
			 *  continue the transfer, call the function again with identical parameters and it will resume until the BytesProcessed
			 *  value reaches the total transfer length.
			 *
			 *  <b>Single Stream Transfer Example:</b>
			 *  \code
			 *  uint8_t ErrorCode;
			 *
			 *  if ((ErrorCode = Pipe_Discard_Stream(512, NULL)) != PIPE_RWSTREAM_NoError)
			 *  {
			 *       // Stream failed to complete - check ErrorCode here
			 *  }
			 *  \endcode
			 *
			 *  <b>Partial Stream Transfers Example:</b>
			 *  \code
			 *  uint8_t  ErrorCode;
			 *  uint16_t BytesProcessed;
			 *
			 *  BytesProcessed = 0;
			 *  while ((ErrorCode = Pipe_Discard_Stream(512, &BytesProcessed)) == PIPE_RWSTREAM_IncompleteTransfer)
			 *  {
			 *      // Stream not yet complete - do other actions here, abort if required
			 *  }
			 *
			 *  if (ErrorCode != PIPE_RWSTREAM_NoError)
			 *  {
			 *      // Stream failed to complete - check ErrorCode here
			 *  }
			 *  \endcode
			 *
			 *  \note The pipe token is set automatically, thus this can be used on bi-directional pipes directly without
			 *        having to explicitly change the data direction with a call to \ref Pipe_SetPipeToken().
			 *
			 *  \param[in] Length          Number of bytes to discard via the currently selected pipe.
			 *  \param[in] BytesProcessed  Pointer to a location where the total number of bytes already processed should
			 *                             updated, \c NULL if the entire stream should be processed at once.
			 *
			 *  \return A value from the \ref Pipe_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Pipe_Discard_Stream(const uint8_t corenum,
										uint16_t Length,
			                            uint16_t* const BytesProcessed) /*ATTR_DEPRECATED*/;

			/** Writes a given number of zeroed bytes to the pipe, sending full pipe packets from the host to the device
			 *  as needed. The last packet is not automatically sent once the remaining bytes has been written; the
			 *  user is responsible for manually discarding the last packet from the device via the \ref Pipe_ClearOUT() macro.
			 *
			 *  If the BytesProcessed parameter is \c NULL, the entire stream transfer is attempted at once, failing or
			 *  succeeding as a single unit. If the BytesProcessed parameter points to a valid storage location, the transfer
			 *  will instead be performed as a series of chunks. Each time the pipe bank becomes full while there is still data
			 *  to process (and after the current packet transmission has been initiated) the BytesProcessed location will be
			 *  updated with the total number of bytes processed in the stream, and the function will exit with an error code of
			 *  \ref PIPE_RWSTREAM_IncompleteTransfer. This allows for any abort checking to be performed in the user code - to
			 *  continue the transfer, call the function again with identical parameters and it will resume until the BytesProcessed
			 *  value reaches the total transfer length.
			 *
			 *  <b>Single Stream Transfer Example:</b>
			 *  \code
			 *  uint8_t ErrorCode;
			 *
			 *  if ((ErrorCode = Pipe_Null_Stream(512, NULL)) != PIPE_RWSTREAM_NoError)
			 *  {
			 *       // Stream failed to complete - check ErrorCode here
			 *  }
			 *  \endcode
			 *
			 *  <b>Partial Stream Transfers Example:</b>
			 *  \code
			 *  uint8_t  ErrorCode;
			 *  uint16_t BytesProcessed;
			 *
			 *  BytesProcessed = 0;
			 *  while ((ErrorCode = Pipe_Null_Stream(512, &BytesProcessed)) == PIPE_RWSTREAM_IncompleteTransfer)
			 *  {
			 *      // Stream not yet complete - do other actions here, abort if required
			 *  }
			 *
			 *  if (ErrorCode != PIPE_RWSTREAM_NoError)
			 *  {
			 *      // Stream failed to complete - check ErrorCode here
			 *  }
			 *  \endcode
			 *
			 *  \note The pipe token is set automatically, thus this can be used on bi-directional pipes directly without
			 *        having to explicitly change the data direction with a call to \ref Pipe_SetPipeToken().
			 *
			 *  \param[in] Length          Number of zero bytes to write via the currently selected pipe.
			 *  \param[in] BytesProcessed  Pointer to a location where the total number of bytes already processed should
			 *                             updated, \c NULL if the entire stream should be processed at once.
			 *
			 *  \return A value from the \ref Pipe_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Pipe_Null_Stream(const uint8_t corenum,
									 uint16_t Length,
			                         uint16_t* const BytesProcessed);

			//@}

			/** \name Stream functions for RAM source/destination data */
			//@{
			
			/** Writes the given number of bytes to the pipe from the given buffer in little endian,
			 *  sending full packets to the device as needed. The last packet filled is not automatically sent;
			 *  the user is responsible for manually sending the last written packet to the host via the
			 *  \ref Pipe_ClearOUT() macro. Between each USB packet, the given stream callback function is
			 *  executed repeatedly until the next packet is ready, allowing for early aborts of stream transfers.
			 *
			 *  If the BytesProcessed parameter is \c NULL, the entire stream transfer is attempted at once,
			 *  failing or succeeding as a single unit. If the BytesProcessed parameter points to a valid
			 *  storage location, the transfer will instead be performed as a series of chunks. Each time
			 *  the pipe bank becomes full while there is still data to process (and after the current
			 *  packet transmission has been initiated) the BytesProcessed location will be updated with the
			 *  total number of bytes processed in the stream, and the function will exit with an error code of
			 *  \ref PIPE_RWSTREAM_IncompleteTransfer. This allows for any abort checking to be performed
			 *  in the user code - to continue the transfer, call the function again with identical parameters
			 *  and it will resume until the BytesProcessed value reaches the total transfer length.
			 *
			 *  <b>Single Stream Transfer Example:</b>
			 *  \code
			 *  uint8_t DataStream[512];
			 *  uint8_t ErrorCode;
			 *
			 *  if ((ErrorCode = Pipe_Write_Stream_LE(DataStream, sizeof(DataStream),
			 *                                        NULL)) != PIPE_RWSTREAM_NoError)
			 *  {
			 *       // Stream failed to complete - check ErrorCode here
			 *  }
			 *  \endcode
			 *
			 *  <b>Partial Stream Transfers Example:</b>
			 *  \code
			 *  uint8_t  DataStream[512];
			 *  uint8_t  ErrorCode;
			 *  uint16_t BytesProcessed;
			 *
			 *  BytesProcessed = 0;
			 *  while ((ErrorCode = Pipe_Write_Stream_LE(DataStream, sizeof(DataStream),
			 *                                           &BytesProcessed)) == PIPE_RWSTREAM_IncompleteTransfer)
			 *  {
			 *      // Stream not yet complete - do other actions here, abort if required
			 *  }
			 *
			 *  if (ErrorCode != PIPE_RWSTREAM_NoError)
			 *  {
			 *      // Stream failed to complete - check ErrorCode here
			 *  }
			 *  \endcode
			 *
			 *  \note The pipe token is set automatically, thus this can be used on bi-directional pipes directly without
			 *        having to explicitly change the data direction with a call to \ref Pipe_SetPipeToken().
			 *
			 *  \param[in] Buffer          Pointer to the source data buffer to read from.
			 *  \param[in] Length          Number of bytes to read for the currently selected pipe into the buffer.
			 *  \param[in] BytesProcessed  Pointer to a location where the total number of bytes already processed should
			 *                             updated, \c NULL if the entire stream should be written at once.
			 *
			 *  \return A value from the \ref Pipe_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Pipe_Write_Stream_LE(const uint8_t corenum,
										 const void* const Buffer,
			                             uint16_t Length,
			                             uint16_t* const BytesProcessed);

			/** Writes the given number of bytes to the pipe from the given buffer in big endian,
			 *  sending full packets to the device as needed. The last packet filled is not automatically sent;
			 *  the user is responsible for manually sending the last written packet to the host via the
			 *  \ref Pipe_ClearOUT() macro. Between each USB packet, the given stream callback function is
			 *  executed repeatedly until the next packet is ready, allowing for early aborts of stream transfers.
			 *
			 *  \note The pipe token is set automatically, thus this can be used on bi-directional pipes directly without
			 *        having to explicitly change the data direction with a call to \ref Pipe_SetPipeToken().
			 *
			 *  \param[in] Buffer          Pointer to the source data buffer to read from.
			 *  \param[in] Length          Number of bytes to read for the currently selected pipe into the buffer.
			 *  \param[in] BytesProcessed  Pointer to a location where the total number of bytes already processed should
			 *                             updated, \c NULL if the entire stream should be written at once.
			 *
			 *  \return A value from the \ref Pipe_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Pipe_Write_Stream_BE(const void* const Buffer,
			                             uint16_t Length,
			                             uint16_t* const BytesProcessed) ATTR_NON_NULL_PTR_ARG(1) ATTR_ERROR("Function is not implemented yet");

			/** Reads the given number of bytes from the pipe into the given buffer in little endian,
			 *  sending full packets to the device as needed. The last packet filled is not automatically sent;
			 *  the user is responsible for manually sending the last written packet to the host via the
			 *  \ref Pipe_ClearIN() macro. Between each USB packet, the given stream callback function is
			 *  executed repeatedly until the next packet is ready, allowing for early aborts of stream transfers.
			 *
			 *  If the BytesProcessed parameter is \c NULL, the entire stream transfer is attempted at once,
			 *  failing or succeeding as a single unit. If the BytesProcessed parameter points to a valid
			 *  storage location, the transfer will instead be performed as a series of chunks. Each time
			 *  the pipe bank becomes empty while there is still data to process (and after the current
			 *  packet has been acknowledged) the BytesProcessed location will be updated with the total number
			 *  of bytes processed in the stream, and the function will exit with an error code of
			 *  \ref PIPE_RWSTREAM_IncompleteTransfer. This allows for any abort checking to be performed
			 *  in the user code - to continue the transfer, call the function again with identical parameters
			 *  and it will resume until the BytesProcessed value reaches the total transfer length.
			 *
			 *  <b>Single Stream Transfer Example:</b>
			 *  \code
			 *  uint8_t DataStream[512];
			 *  uint8_t ErrorCode;
			 *
			 *  if ((ErrorCode = Pipe_Read_Stream_LE(DataStream, sizeof(DataStream),
			 *                                       NULL)) != PIPE_RWSTREAM_NoError)
			 *  {
			 *       // Stream failed to complete - check ErrorCode here
			 *  }
			 *  \endcode
			 *
			 *  <b>Partial Stream Transfers Example:</b>
			 *  \code
			 *  uint8_t  DataStream[512];
			 *  uint8_t  ErrorCode;
			 *  uint16_t BytesProcessed;
			 *
			 *  BytesProcessed = 0;
			 *  while ((ErrorCode = Pipe_Read_Stream_LE(DataStream, sizeof(DataStream),
			 *                                          &BytesProcessed)) == PIPE_RWSTREAM_IncompleteTransfer)
			 *  {
			 *      // Stream not yet complete - do other actions here, abort if required
			 *  }
			 *
			 *  if (ErrorCode != PIPE_RWSTREAM_NoError)
			 *  {
			 *      // Stream failed to complete - check ErrorCode here
			 *  }
			 *  \endcode
			 *
			 *  \note The pipe token is set automatically, thus this can be used on bi-directional pipes directly without
			 *        having to explicitly change the data direction with a call to \ref Pipe_SetPipeToken().
			 *
			 *  \param[out] Buffer          Pointer to the source data buffer to write to.
			 *  \param[in]  Length          Number of bytes to read for the currently selected pipe to read from.
			 *  \param[in]  BytesProcessed  Pointer to a location where the total number of bytes already processed should
			 *                              updated, \c NULL if the entire stream should be read at once.
			 *
			 *  \return A value from the \ref Pipe_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Pipe_Read_Stream_LE(const uint8_t corenum,
										void* const Buffer,
			                            uint16_t Length,
			                            uint16_t* const BytesProcessed);

			/** Reads the given number of bytes from the pipe into the given buffer in big endian,
			 *  sending full packets to the device as needed. The last packet filled is not automatically sent;
			 *  the user is responsible for manually sending the last written packet to the host via the
			 *  \ref Pipe_ClearIN() macro. Between each USB packet, the given stream callback function is
			 *  executed repeatedly until the next packet is ready, allowing for early aborts of stream transfers.
			 *
			 *  \note The pipe token is set automatically, thus this can be used on bi-directional pipes directly without
			 *        having to explicitly change the data direction with a call to \ref Pipe_SetPipeToken().
			 *
			 *  \param[out] Buffer          Pointer to the source data buffer to write to.
			 *  \param[in]  Length          Number of bytes to read for the currently selected pipe to read from.
			 *  \param[in]  BytesProcessed  Pointer to a location where the total number of bytes already processed should
			 *                              updated, \c NULL if the entire stream should be read at once.
			 *
			 *  \return A value from the \ref Pipe_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Pipe_Read_Stream_BE(void* const Buffer,
			                            uint16_t Length,
			                            uint16_t* const BytesProcessed) ATTR_NON_NULL_PTR_ARG(1) ATTR_ERROR("Function is not implemented yet");
			//@}

			/** \name Stream functions for EEPROM source/destination data */
			//@{
			
			/** EEPROM buffer source version of \ref Pipe_Write_Stream_LE().
			 *
			 *  \param[in] Buffer          Pointer to the source data buffer to read from.
			 *  \param[in] Length          Number of bytes to read for the currently selected pipe into the buffer.
			 *  \param[in] BytesProcessed  Pointer to a location where the total number of bytes already processed should
			 *                             updated, \c NULL if the entire stream should be written at once.
			 *
			 *  \return A value from the \ref Pipe_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Pipe_Write_EStream_LE(const void* const Buffer,
			                              uint16_t Length,
			                              uint16_t* const BytesProcessed) ATTR_NON_NULL_PTR_ARG(1) ATTR_ERROR("Function is not implemented yet");
			
			/** EEPROM buffer source version of \ref Pipe_Write_Stream_BE().
			 *
			 *  \param[in] Buffer          Pointer to the source data buffer to read from.
			 *  \param[in] Length          Number of bytes to read for the currently selected pipe into the buffer.
			 *  \param[in] BytesProcessed  Pointer to a location where the total number of bytes already processed should
			 *                             updated, \c NULL if the entire stream should be written at once.
			 *
			 *  \return A value from the \ref Pipe_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Pipe_Write_EStream_BE(const void* const Buffer,
			                              uint16_t Length,
			                              uint16_t* const BytesProcessed) ATTR_NON_NULL_PTR_ARG(1) ATTR_ERROR("Function is not implemented yet");

			/** EEPROM buffer source version of \ref Pipe_Read_Stream_LE().
			 *
			 *  \param[out] Buffer          Pointer to the source data buffer to write to.
			 *  \param[in]  Length          Number of bytes to read for the currently selected pipe to read from.
			 *  \param[in]  BytesProcessed  Pointer to a location where the total number of bytes already processed should
			 *                              updated, \c NULL if the entire stream should be read at once.
			 *
			 *  \return A value from the \ref Pipe_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Pipe_Read_EStream_LE(void* const Buffer,
			                             uint16_t Length,
			                             uint16_t* const BytesProcessed) ATTR_NON_NULL_PTR_ARG(1) ATTR_ERROR("Function is not implemented yet");
			
			/** EEPROM buffer source version of \ref Pipe_Read_Stream_BE().
			 *
			 *  \param[out] Buffer          Pointer to the source data buffer to write to.
			 *  \param[in]  Length          Number of bytes to read for the currently selected pipe to read from.
			 *  \param[in]  BytesProcessed  Pointer to a location where the total number of bytes already processed should
			 *                              updated, \c NULL if the entire stream should be read at once.
			 *
			 *  \return A value from the \ref Pipe_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Pipe_Read_EStream_BE(void* const Buffer,
			                             uint16_t Length,
			                             uint16_t* const BytesProcessed) ATTR_NON_NULL_PTR_ARG(1) ATTR_ERROR("Function is not implemented yet");
			//@}

			/** \name Stream functions for PROGMEM source/destination data */
			//@{
			
			/** FLASH buffer source version of \ref Pipe_Write_Stream_LE().
			 *
			 *  \pre The FLASH data must be located in the first 64KB of FLASH for this function to work correctly.
			 *
			 *  \param[in] Buffer          Pointer to the source data buffer to read from.
			 *  \param[in] Length          Number of bytes to read for the currently selected pipe into the buffer.
			 *  \param[in] BytesProcessed  Pointer to a location where the total number of bytes already processed should
			 *                             updated, \c NULL if the entire stream should be written at once.
			 *
			 *  \return A value from the \ref Pipe_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Pipe_Write_PStream_LE(const void* const Buffer,
			                              uint16_t Length,
			                              uint16_t* const BytesProcessed) ATTR_NON_NULL_PTR_ARG(1) ATTR_ERROR("Function is not implemented yet");
			
			/** FLASH buffer source version of \ref Pipe_Write_Stream_BE().
			 *
			 *  \pre The FLASH data must be located in the first 64KB of FLASH for this function to work correctly.
			 *
			 *  \param[in] Buffer          Pointer to the source data buffer to read from.
			 *  \param[in] Length          Number of bytes to read for the currently selected pipe into the buffer.
			 *  \param[in] BytesProcessed  Pointer to a location where the total number of bytes already processed should
			 *                             updated, \c NULL if the entire stream should be written at once.
			 *
			 *  \return A value from the \ref Pipe_Stream_RW_ErrorCodes_t enum.
			 */
			uint8_t Pipe_Write_PStream_BE(const void* const Buffer,
			                              uint16_t Length,
			                              uint16_t* const BytesProcessed) ATTR_NON_NULL_PTR_ARG(1)ATTR_ERROR("Function is not implemented yet");
			//@}

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif
	
#endif

/** @} */

