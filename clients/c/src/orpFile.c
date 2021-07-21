/**
 * @file:    orpFile.c
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

#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "orpFile.h"
#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Maximum file name length
 */
//--------------------------------------------------------------------------------------------------
#define FILE_NAME_MAX_LEN   128

//--------------------------------------------------------------------------------------------------
/**
 * Maximum data to be read
 */
//--------------------------------------------------------------------------------------------------
#define FILE_DATA_MAX_LEN   (100 * 1024)

//--------------------------------------------------------------------------------------------------
/**
 * Static file descriptor
 */
//--------------------------------------------------------------------------------------------------
static int Fd = -1;

//--------------------------------------------------------------------------------------------------
/**
 * Static for auto mode
 */
//--------------------------------------------------------------------------------------------------
static int AutoMode = false;

//--------------------------------------------------------------------------------------------------
/**
 * Static for file name
 */
//--------------------------------------------------------------------------------------------------
static char FileName[FILE_NAME_MAX_LEN] = {0};

//--------------------------------------------------------------------------------------------------
/**
 * Static buffer for incoming file data
 */
//--------------------------------------------------------------------------------------------------
static uint8_t IncomingFileData[FILE_DATA_MAX_LEN];

//--------------------------------------------------------------------------------------------------
/**
 * Static for incoming file data length
 */
//--------------------------------------------------------------------------------------------------
static size_t IncomingFileDataLen = 0;

//--------------------------------------------------------------------------------------------------
/**
 * Total bytes received for the current file
 */
//--------------------------------------------------------------------------------------------------
static size_t ReceivedFileBytes = 0;

//--------------------------------------------------------------------------------------------------
/**
 * Total bytes expected for the current file
 */
//--------------------------------------------------------------------------------------------------
static ssize_t ExpectedFileBytes = -1;

//--------------------------------------------------------------------------------------------------
/**
 * Function to close the file
 */
//--------------------------------------------------------------------------------------------------
int orp_FileClose
(
    void
)
{
    int result = close(Fd);
    Fd = -1;
    return result;
}

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
)
{
    if ((Fd == -1) && (strlen(FileName)))
    {
        int fd;
        fd = open(FileName, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
        if (fd == -1)
        {
            perror("Cannot open output file\n");
            return -1;
        }
        else
        {
            Fd = fd;
        }
    }

    if (Fd != -1)
    {
        return write(Fd, dataPtr, dataLen);
    }

    return -1;
}


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
)
{
    if (!dataPtr)
    {
        return;
    }

    memset(IncomingFileData, 0, FILE_DATA_MAX_LEN);
    memcpy(IncomingFileData, dataPtr, dataLen);
    IncomingFileDataLen = dataLen;
}

//--------------------------------------------------------------------------------------------------
/**
 * Function to flush saved data in RAM to the file
 * This is used is auto mode is not set
 * This function is called when the user acks the file data
 */
//--------------------------------------------------------------------------------------------------
void orp_FileDataFlush
(
    void
)
{
    if (!AutoMode && IncomingFileDataLen)
    {
        orp_FileDataWrite(IncomingFileData, IncomingFileDataLen);
        IncomingFileDataLen = 0;
        memset(IncomingFileData, 0, FILE_DATA_MAX_LEN);
    }
}

//--------------------------------------------------------------------------------------------------
/**
 * Function to set the file name (from the 'file control start/auto <filename>' command)
 */
//--------------------------------------------------------------------------------------------------
void orp_FileTransferSetName
(
    char* namePtr               ///< [IN] File name
)
{
    if (!namePtr)
    {
        LE_ERROR("Invalid file name");
        return;
    }
    memset(FileName, 0, FILE_NAME_MAX_LEN);
    snprintf(FileName, FILE_NAME_MAX_LEN, "%s", namePtr);
}

//--------------------------------------------------------------------------------------------------
/**
 * Function to set the auto mode
 */
//--------------------------------------------------------------------------------------------------
void orp_FileTransferSetAuto
(
    bool isAuto                 ///< [IN] Is auto mode set ?
)
{
    AutoMode = isAuto;
}


//--------------------------------------------------------------------------------------------------
/**
 * Function to check if the auto mode is activated
 */
//--------------------------------------------------------------------------------------------------
bool orp_FileTransferGetAuto
(
    void
)
{
    return AutoMode;
}


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
)
{
    orp_FileTransferSetName(namePtr);
    AutoMode = isAuto;
    ReceivedFileBytes = 0;
    ExpectedFileBytes = fileSize;
}

//--------------------------------------------------------------------------------------------------
/**
 * Save or cache inbound file data
 */
//--------------------------------------------------------------------------------------------------
ssize_t orp_FileDataCache
(
    void   *dataPtr,
    size_t  dataLen
)
{
    ReceivedFileBytes += dataLen;

    if (AutoMode)
    {
        orp_FileDataWrite(dataPtr, dataLen);
    }
    else
    {
        orp_FileDataKeep(dataPtr, dataLen);
    }

    // Once all bytes are received, disable auto mode
    if ((ExpectedFileBytes > 0) && (ReceivedFileBytes >= ExpectedFileBytes))
    {
        AutoMode = false;
    }
}
