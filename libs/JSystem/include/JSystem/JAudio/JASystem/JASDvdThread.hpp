#ifndef JASDVDTHREAD_HPP
#define JASDVDTHREAD_HPP

#include <dolphin/dvd.h>
#include <dolphin/os.h>
#include <dolphin/types.h>
#include <stdint.h>

namespace JASystem {

namespace Dvd {
	typedef void (*DvdCallback)(u32);

	void init();
	void dvdProcInit();
	void* dvdProc(void*);

	void pauseDvdT();
	void unpauseDvdT();

	s32 loadToAramDvdT(u32 user_data, char* path, void* buffer, u32 offset,
	                   u32 size, u32* result_ptr, DvdCallback callback);
	s32 loadToDramDvdT(u32 user_data, char* path, void* buffer, u32 offset,
	                   u32 size, u32* result_ptr, DvdCallback callback);

	s32 aramToDramDvdT(u32 user_data, void* dst, void* src, u32 size,
	                   u32* result_ptr, DvdCallback callback);
	s32 dramToAramDvdT(u32 user_data, void* src, void* dst, u32 size,
	                   u32* result_ptr, DvdCallback callback);

	s32 checkPassDvdT(u32 user_data, u32* result_ptr, DvdCallback callback);
	u32 loadFileDvdT(char* path, void* buffer);

	s32 checkFile(char* path);
	void extendPath(char* path1, char* path2);
	u32 checkFileExtend(char* path);
	s32 loadFile(char* path, void* buffer);

	struct TDvdCall {
		/* 0x0 */ u32 mCbUserData;
		/* 0x4 */ char mPath[0x20];
		/* 0x24 */ void* mDramBuffer;
		/* 0x28 */ uintptr_t mOffsetOrAramAddress;
		/* 0x2C */ u32 mTransferSize;
		/* 0x30 */ u32* mResult;
		/* 0x34 */ DvdCallback mCallback;
	};

	s32 openDvd(char* path, DVDFileInfo* fileInfo);
	s32 addTask(s32 (*func)(void*), void* data, u32 size);
	s32 addTaskHigh(s32 (*func)(void*), void* data, u32 size);
	s32 loadToAramDvdTMain(void* task_data);
	s32 loadToDramDvdTMain(void* task_data);
	s32 aramToDramDvdTMain(void* task_data);
	s32 dramToAramDvdTMain(void* task_data);

	typedef void (*ErrorCallback)(char*, u8*);
	void registerDvdErrorCallback(ErrorCallback);
	void setFastOpenMaxDic(s32);
	void registerExtFastOpen(char* path);
	s32 registerFastOpen(char* path);

	void setNumOfMsgs(s32 num);
	void setDvdBufSize(u32 size);
	void setRootPath(char*);
	void setBufferDvdT(u8*, u32, u32);
	void closeBufferDvdT(u8*);
	void getCurrentBufferDvdT(u8**);
} // namespace Dvd

} // namespace JASystem

#endif // JASDVDTHREAD_HPP
