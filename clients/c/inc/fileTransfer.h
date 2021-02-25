//--------------------------------------------------------------------------------------------------
/**
 * File transfer event types
 */
//--------------------------------------------------------------------------------------------------
enum fileTransferEventE
{
    FILES_TRANSFER_EVENT_INFO,                ///< Informational (ignored by ORP service)
    FILES_TRANSFER_EVENT_READY,               ///< Client ready for file transfer
    FILES_TRANSFER_EVENT_PENDING,             ///< Transfer pending
    FILES_TRANSFER_EVENT_SUSPEND,             ///< File transfer suspend
    FILES_TRANSFER_EVENT_RESUME,              ///< File transfer resume
    FILES_TRANSFER_EVENT_COMPLETE,            ///< File transfer complete
    FILES_TRANSFER_EVENT_ABORT,               ///< File transfer aborted
};
