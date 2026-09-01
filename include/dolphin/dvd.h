#ifndef _DOLPHIN_DVD_H_
#define _DOLPHIN_DVD_H_

#include <dolphin/types.h>

typedef struct DVDDiskID DVDDiskID;
struct DVDDiskID {
	char gameName[4];
	char company[2];
	u8 diskNumber;
	u8 gameVersion;
	u8 streaming;
	u8 streamingBufSize;
	u8 padding[22];
};

typedef struct DVDCommandBlock DVDCommandBlock;
typedef void (*DVDCBCallback)(s32 result, DVDCommandBlock* block);
struct DVDCommandBlock {
	/*0x00*/ DVDCommandBlock* next;
	/*0x04*/ DVDCommandBlock* prev;
	/*0x08*/ u32 command;
	/*0x0C*/ s32 state;
	/*0x10*/ u32 offset;
	/*0x14*/ u32 length;
	/*0x18*/ void* addr;
	/*0x1C*/ u32 currTransferSize;
	/*0x20*/ u32 transferredSize;
	/*0x24*/ DVDDiskID* id;
	/*0x28*/ DVDCBCallback callback;
	/*0x2C*/ void* userData;
};

typedef struct DVDFileInfo DVDFileInfo;
typedef void (*DVDCallback)(s32 result, DVDFileInfo* fileInfo);
struct DVDFileInfo {
	/*0x00*/ DVDCommandBlock cb;
	/*0x30*/ u32 startAddr;
	/*0x34*/ u32 length;
	/*0x38*/ DVDCallback callback;
};

typedef struct {
	u32 entryNum;
	u32 location;
	u32 next;
} DVDDir;

typedef struct {
	u32 entryNum;
	BOOL isDir;
	char* name;
} DVDDirEntry;

typedef struct DVDBB2 {
	/* 0x00 */ u32 bootFilePosition;
	/* 0x04 */ u32 FSTPosition;
	/* 0x08 */ u32 FSTLength;
	/* 0x0C */ u32 FSTMaxLength;
	/* 0x10 */ void* FSTAddress;
	/* 0x14 */ u32 userPosition;
	/* 0x18 */ u32 userLength;
	/* 0x1C */ u32 padding0;
} DVDBB2;

typedef struct DVDDriveInfo {
	/* 0x00 */ u16 revisionLevel;
	/* 0x02 */ u16 deviceCode;
	/* 0x04 */ u32 releaseDate;
	/* 0x08 */ u8 padding[24];
} DVDDriveInfo;

#ifdef __cplusplus
extern "C" {
#endif

// dvd.c
void DVDInit();
BOOL DVDReadAbsAsyncPrio(DVDCommandBlock* block, void* addr, s32 length,
                         s32 offset, DVDCBCallback callback, s32 prio);
BOOL DVDSeekAbsAsyncPrio(DVDCommandBlock* block, s32 offset,
                         DVDCBCallback callback, s32 prio);
BOOL DVDReadAbsAsyncForBS(DVDCommandBlock* block, void* addr, s32 length,
                          s32 offset, DVDCBCallback callback);
BOOL DVDReadDiskID(DVDCommandBlock* block, struct DVDDiskID* diskID,
                   DVDCBCallback callback);
BOOL DVDPrepareStreamAbsAsync(DVDCommandBlock* block, u32 length, u32 offset,
                              DVDCBCallback callback);
BOOL DVDCancelStreamAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDCancelStream(DVDCommandBlock* block);
BOOL DVDStopStreamAtEndAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDStopStreamAtEnd(DVDCommandBlock* block);
BOOL DVDGetStreamErrorStatusAsync(DVDCommandBlock* block,
                                  DVDCBCallback callback);
s32 DVDGetStreamErrorStatus(DVDCommandBlock* block);
BOOL DVDGetStreamPlayAddrAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDGetStreamPlayAddr(DVDCommandBlock* block);
BOOL DVDGetStreamStartAddrAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDGetStreamStartAddr(DVDCommandBlock* block);
BOOL DVDGetStreamLengthAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDGetStreamLength(DVDCommandBlock* block);
BOOL DVDChangeDiskAsyncForBS(DVDCommandBlock* block, DVDCBCallback callback);
BOOL DVDChangeDiskAsync(DVDCommandBlock* block, DVDDiskID* id,
                        DVDCBCallback callback);
s32 DVDChangeDisk(DVDCommandBlock* block, DVDDiskID* id);
BOOL DVDInquiryAsync(DVDCommandBlock* block, struct DVDDriveInfo* info,
                     DVDCBCallback callback);
s32 DVDInquiry(DVDCommandBlock* block, struct DVDDriveInfo* info);
void DVDReset();
BOOL DVDResetRequired();
s32 DVDGetCommandBlockStatus(DVDCommandBlock* block);
s32 DVDGetDriveStatus();
BOOL DVDCheckDisk(void);
BOOL DVDSetAutoInvalidation(BOOL autoInval);
void DVDPause();
void DVDResume();
BOOL DVDCancelAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDCancel(DVDCommandBlock* block);
BOOL DVDCancelAllAsync(DVDCBCallback callback);
s32 DVDCancelAll();
DVDDiskID* DVDGetCurrentDiskID();

// dvdfs.c
s32 DVDConvertPathToEntrynum(char* pathPtr);
BOOL DVDFastOpen(s32 entrynum, DVDFileInfo* fileInfo);
BOOL DVDOpen(char* fileName, DVDFileInfo* fileInfo);
BOOL DVDClose(DVDFileInfo* fileInfo);
BOOL DVDGetCurrentDir(char* path, u32 maxlen);
BOOL DVDChangeDir(char* dirName);
BOOL DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset,
                      DVDCallback callback, s32 prio);
s32 DVDReadPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset,
                s32 prio);
BOOL DVDSeekAsyncPrio(DVDFileInfo* fileInfo, s32 offset, DVDCallback callback,
                      s32 prio);
s32 DVDSeekPrio(DVDFileInfo* fileInfo, s32 offset, s32 prio);
s32 DVDGetFileInfoStatus(DVDFileInfo* fileInfo);
BOOL DVDOpenDir(char* dirName, DVDDir* dir);
BOOL DVDReadDir(DVDDir* dir, DVDDirEntry* dirent);
BOOL DVDCloseDir(DVDDir* dir);
void* DVDGetFSTLocation();
BOOL DVDPrepareStreamAsync(DVDFileInfo* fileInfo, u32 length, u32 offset,
                           DVDCallback callback);
s32 DVDPrepareStream(DVDFileInfo* fileInfo, u32 length, u32 offset);
s32 DVDGetTransferredSize(DVDFileInfo* fileinfo);

#define DVDReadAsync(fileInfo, addr, length, offset, callback)                 \
	DVDReadAsyncPrio((fileInfo), (addr), (length), (offset), (callback), 2)

#define DVD_RESULT_GOOD        0
#define DVD_RESULT_FATAL_ERROR -1
#define DVD_RESULT_IGNORED     -2
#define DVD_RESULT_CANCELED    -3

#define DVD_STATE_FATAL_ERROR   -1
#define DVD_STATE_END           0
#define DVD_STATE_BUSY          1
#define DVD_STATE_WAITING       2
#define DVD_STATE_COVER_CLOSED  3
#define DVD_STATE_NO_DISK       4
#define DVD_STATE_COVER_OPEN    5
#define DVD_STATE_WRONG_DISK    6
#define DVD_STATE_MOTOR_STOPPED 7
#define DVD_STATE_PAUSING       8
#define DVD_STATE_IGNORED       9
#define DVD_STATE_CANCELED      10
#define DVD_STATE_RETRY         11

#define DVD_MIN_TRANSFER_SIZE 32

// could be bitfields
#define DVD_INTTYPE_TC 1
#define DVD_INTTYPE_DE 2
// unk type 3
#define DVD_INTTYPE_CVR 4

// DVD Commands

#define DVD_COMMAND_NONE                0
#define DVD_COMMAND_READ                1
#define DVD_COMMAND_SEEK                2
#define DVD_COMMAND_CHANGE_DISK         3
#define DVD_COMMAND_BSREAD              4
#define DVD_COMMAND_READID              5
#define DVD_COMMAND_INITSTREAM          6
#define DVD_COMMAND_CANCELSTREAM        7
#define DVD_COMMAND_STOP_STREAM_AT_END  8
#define DVD_COMMAND_REQUEST_AUDIO_ERROR 9
#define DVD_COMMAND_REQUEST_PLAY_ADDR   10
#define DVD_COMMAND_REQUEST_START_ADDR  11
#define DVD_COMMAND_REQUEST_LENGTH      12
#define DVD_COMMAND_AUDIO_BUFFER_CONFIG 13
#define DVD_COMMAND_INQUIRY             14
#define DVD_COMMAND_BS_CHANGE_DISK      15

typedef void (*DVDLowCallback)(u32 intType);

BOOL DVDLowRead(void* addr, u32 length, u32 offset, void (*callback)(u32));
BOOL DVDLowSeek(u32 offset, void (*callback)(u32));
BOOL DVDLowWaitCoverClose(void (*callback)(u32));
BOOL DVDLowReadDiskID(struct DVDDiskID* diskID, void (*callback)(u32));
BOOL DVDLowStopMotor(void (*callback)(u32));
BOOL DVDLowRequestError(void (*callback)(u32));
BOOL DVDLowInquiry(struct DVDDriveInfo* info, void (*callback)(u32));
BOOL DVDLowAudioStream(u32 subcmd, u32 length, u32 offset,
                       void (*callback)(u32));
BOOL DVDLowRequestAudioStatus(u32 subcmd, void (*callback)(u32));
BOOL DVDLowAudioBufferConfig(BOOL enable, u32 size, void (*callback)(u32));
void DVDLowReset();
void (*DVDLowSetResetCoverCallback(void (*callback)(u32)))(u32);
BOOL DVDLowBreak();
void (*DVDLowClearCallback())(u32);
u32 DVDLowGetCoverStatus();

#ifdef __cplusplus
}
#endif

#endif
