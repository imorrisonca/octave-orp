/**
 * @file:    orpFile.h
 *
 * Purpose:  File transfer utility for the Octave Resource Protocol
 *
 * MIT License
 *
 * Copyright (c) 2021 Sierra Wireless Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *----------------------------------------------------------------------------
 *
 * NOTES:
 *
 */

#ifndef ORP_FILE_H_INCLUDE_GUARD
#define ORP_FILE_H_INCLUDE_GUARD

#include "orpProtocol.h"


//--------------------------------------------------------------------------------------------------
/**
 * Function to close the file
 */
//--------------------------------------------------------------------------------------------------
int orp_FileClose
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to write data in the file
 * This function creates the file if it does not exist
 */
//--------------------------------------------------------------------------------------------------
ssize_t orp_FileDataWrite
(
    void*   dataPtr,            ///< [IN] Data pointer
    size_t  dataLen             ///< [IN] Data length
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to keep data in RAM before storing it
 * This is used if auto mode is not set
 */
//--------------------------------------------------------------------------------------------------
void orp_FileDataKeep
(
    void*   dataPtr,            ///< [IN] Data pointer
    size_t  dataLen             ///< [IN] Data length
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to flush saved data in RAM to the file
 * This is used if auto mode is not set
 * This function is called when the user acks the file data
 */
//--------------------------------------------------------------------------------------------------
void orp_FileDataFlush
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to set the file name (from the 'file control start/auto <filename>' command)
 */
//--------------------------------------------------------------------------------------------------
void orp_FileTransferSetName
(
    char* namePtr               ///< [IN] File name
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to set the auto mode
 */
//--------------------------------------------------------------------------------------------------
void orp_FileTransferSetAuto
(
    bool isAuto                 ///< [IN] Is auto mode set ?
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to check if the auto mode is activated
 */
//--------------------------------------------------------------------------------------------------
bool orp_FileTransferGetAuto
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Setup data storage for inbound file transfer
 */
//--------------------------------------------------------------------------------------------------
void orp_FileDataSetup
(
    char   *namePtr,
    size_t  fileSize,
    bool    isAuto
);

//--------------------------------------------------------------------------------------------------
/**
 * Save or cache inbound file data
 */
//--------------------------------------------------------------------------------------------------
ssize_t orp_FileDataCache
(
    void   *dataPtr,
    size_t  dataLen
);

#endif // ORP_FILE_H_INCLUDE_GUARD
