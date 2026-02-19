#include <System/CardManager.hpp>
#include <macros.h>
#include <stdio.h>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>
#include <System/FlagManager.hpp>

f32 dummy[]  = { 1.0f, 1.0f, 1.0f };
f32 dummy2[] = { 1.0f, 1.0f, 1.0f };
u32 dummy3[] = { 0, 2, 1, 3 };

static u8 sDetach[2];
const char CardFileName[0x20] = "super_mario_sunshine\0\0\0\0\0\0\0\0\0\0\0";

const char* titles[] = {
	"スーパーマリオサンシャイン", "Super Mario Sunshine",
	"Super Mario Sunshine",       "Super Mario Sunshine",
	"Super Mario Sunshine",       "Super Mario Sunshine",
	"Super Mario Sunshine",
};

const char* comments[] = {
	"%d月%d日のセーブデータです", "Last saved on %d/%d", "Last saved on %d/%d",
	"Last saved on %d/%d",        "Last saved on %d/%d", "Last saved on %d/%d",
	"Last saved on %d/%d",
};

static u32 CalcCheckSum(const void* data, u32 size)
{
	u16* ptr = (u16*)data;
	u32 top, bottom;
	top = bottom = 0;
	for (u32 i = 0; i < size / 2; ++i, ++ptr) {
		top += ptr[0];
		bottom += ~ptr[0];
	}
	return (top << 16) | bottom & 0xffff;
}

void TCardSector::clearData()
{
	memset(&mHeader, 0, sizeof(mHeader) + sizeof(mOptionBlock));
}

void TCardSector::setCheckSum(u32 write_count)
{
	mWriteCount = write_count;
	u16* ptr    = (u16*)this;
	u32 top, bottom;
	top = bottom = 0;
	for (u32 i = 0; i < 0x1FFC / 2; ++i, ++ptr) {
		top += ptr[0];
		bottom += ~ptr[0];
	}
	mCheckSum = (top << 16) | bottom & 0xffff;
}

// TODO: incorrect
s32 TCardSector::read(CARDFileInfo* file, s32 index,
                      TCardManager::TCriteria* criteria)
{
	s32 errc = CARDRead(file, this, sizeof(TCardSector),
	                    index * sizeof(TCardSector));
	if (errc == CARD_RESULT_READY) {
		s32 writeCount   = mWriteCount;
		const void* data = &mHeader;
		bool eq          = !(CalcCheckSum(this, 0x1FFC) - mCheckSum);
		criteria->set(eq ? TCardManager::TCriteria::STATE_VALID
		                 : TCardManager::TCriteria::STATE_CHECKSUM_BAD,
		              writeCount, data);
	}
	return errc;
}

void TCardManager::TCriteria::set(TCardManager::TCriteria::TEBlockStat state,
                                  u32 write_count, const void* sector_data)
{
	(void)0;
	(void)0;
	mState = state;
	if (mState == STATE_VALID) {
		mWriteCount = write_count;
		memcpy(&mPreviewBytes, sector_data, sizeof(mPreviewBytes));
	} else {
		setEmpty();
	}
}

void TCardManager::TCriteria::setEmpty()
{
	if (mState == STATE_EMPTY) {
		mWriteCount = 0;
		memset(mPreviewBytes, 0, sizeof(mPreviewBytes));
	}
}

// TODO: incorrect
#pragma dont_inline on
s32 TCardManager::decideUseSector(TCardManager::TCriteria* criteria)
{
	if (criteria[0].getState() == TCriteria::STATE_EMPTY)
		return CARD_RESULT_WRONGDEVICE;

	if (criteria[0].getState() == TCriteria::STATE_CHECKSUM_BAD) {
		if (criteria[1].getState() == TCriteria::STATE_CHECKSUM_BAD)
			return CARD_RESULT_BUSY;
		return 1;
	}

	if (criteria[1].getState() == TCriteria::STATE_CHECKSUM_BAD)
		return 0;

	s32 idx;
	if (criteria[0].getWriteCount() >= criteria[1].getWriteCount())
		idx = 0;
	else
		idx = 1;
	return idx;
}
#pragma dont_inline off

// TODO: what is this?
s32 TCardManager::getLoadIndex(TCardManager::TCriteria* criteria) { }

s32 TCardManager::getWriteCount(TCardManager::TCriteria* criteria)
{
	s32 count  = 0;
	int sector = decideUseSector(criteria);
	switch (sector) {
	case 0:
		count = criteria[0].getWriteCount();
		break;
	case 1:
		count = criteria[1].getWriteCount();
		break;
	}
	return count;
}

void TCardManager::copyTo(TCardManager::TCriteria* param_1,
                          TCardBookmarkInfo* param_2)
{
	int sector = decideUseSector(param_1);
	switch (sector) {
	case -2:
		param_2->unk0  = 0;
		param_2->unk4  = 0;
		param_2->unk8  = 0;
		param_2->unk10 = 0;
		param_2->unk18 = 0;
		param_2->unk1C = 0;
		param_2->unk1E = 0;
		break;
	case 0:
	case 1: {
		param_2->unk0 = 0;
		JSUMemoryInputStream stream(param_1[sector].getPreviewBytes(), 0x1C);

		param_2->unk4  = stream.readU32();
		param_2->unk8  = stream.readU64();
		param_2->unk10 = stream.readU64();
		param_2->unk18 = stream.readU32();
		param_2->unk1C = stream.readU16();
		param_2->unk1E = stream.readU16();
	} break;
	case -1:
		param_2->unk0 = 1;
		break;
	}
}

static void* cardmain(void* param_1) { ((TCardManager*)param_1)->cmdLoop(); }

TCardManager::TCardManager(void* sector_work_area, void* card_work_area,
                           s32 channel, s32 thread_prio, void* thread_stack,
                           u32 thread_stack_size)
    : mChannel(channel)
    , mIcons(nullptr)
    , mBanner(nullptr)
    , mNeedsInit(false)
    , mIsMounted(false)
    , mFsCheckedOk(false)
    , mLastStatus(CARD_RESULT_NOCARD)
    , mCardMountWorkArea(card_work_area)
    , mSector((TCardSector*)sector_work_area)
    , mPendingCommand(CMD_NONE)
{
	OSInitMutex(&mMutex);
	OSInitCond(&mCommandIsPending);
	OSCreateThread(&mWorkerThread, &cardmain, this,
	               (u8*)thread_stack + thread_stack_size, thread_stack_size,
	               thread_prio, 0);
	OSResumeThread(&mWorkerThread);
}

TCardManager::~TCardManager()
{
	unmount_();
	issue(CMD_THREAD_EXIT);
	void* retcode;
	OSJoinThread(&mWorkerThread, &retcode);
}

void TCardManager::issue(s32 command)
{
	OSLockMutex(&mMutex);
	mPendingCommand = command;
	mLastStatus     = CARD_RESULT_BUSY;
	OSUnlockMutex(&mMutex);
	OSSignalCond(&mCommandIsPending);
}

u32 TCardManager::probe()
{
	if (OSTryLockMutex(&mMutex)) {
		u32 result = mLastStatus = probe_();
		OSUnlockMutex(&mMutex);
		return result;
	}

	return CARD_RESULT_BUSY;
}

s32 TCardManager::unmount()
{
	OSLockMutex(&mMutex);
	s32 uVar1 = mLastStatus = unmount_();
	OSUnlockMutex(&mMutex);
	return uVar1;
}

void TCardManager::format() { issue(CMD_FORMAT); }

void TCardManager::createFile() { issue(CMD_CREATE); }

void TCardManager::getBookmarkInfos(TCardBookmarkInfo* param_1)
{
	OSLockMutex(&mMutex);
	mGetBookmarkInfosArg = param_1;
	mPendingCommand      = CMD_BOOKMARK_SCAN;
	mLastStatus          = CARD_RESULT_BUSY;
	OSUnlockMutex(&mMutex);
	OSSignalCond(&mCommandIsPending);
}

void TCardManager::readBlock(u32 index)
{
	OSLockMutex(&mMutex);
	mReadWriteBlockArg = index;
	mPendingCommand    = CMD_READ_BLOCK;
	mLastStatus        = CARD_RESULT_BUSY;
	OSUnlockMutex(&mMutex);
	OSSignalCond(&mCommandIsPending);
}

void TCardManager::getReadStream(JSUMemoryInputStream* stream)
{
	TCardSector* sector = (TCardSector*)mSector;
	stream->setBuffer(sector->getData(), sector->getDataSize());
}

void TCardManager::getWriteStream(JSUMemoryOutputStream* stream)
{
	TCardSector* sector = (TCardSector*)mSector;
	sector->clearData();
	stream->setBuffer(sector->getData(), sector->getDataSize());
}

void TCardManager::writeBlock(u32 index)
{
	OSLockMutex(&mMutex);
	mReadWriteBlockArg = index;
	mPendingCommand    = CMD_WRITE_BLOCK;
	mLastStatus        = CARD_RESULT_BUSY;
	OSUnlockMutex(&mMutex);
	OSSignalCond(&mCommandIsPending);
}

void TCardManager::readOptionBlock() { issue(CMD_READ_OPTION); }

void TCardManager::getOptionReadStream(JSUMemoryInputStream* stream)
{
	TCardSector* sector = (TCardSector*)mSector;
	stream->setBuffer(sector->mOptionBlock, sizeof(sector->mOptionBlock));
}

void TCardManager::getOptionWriteStream(JSUMemoryOutputStream* stream)
{
	TCardSector* sector = (TCardSector*)mSector;
	sector->clearData();
	buildHeader_(sector->getHeader());
	stream->setBuffer(sector->mOptionBlock, sizeof(sector->mOptionBlock));
}

void TCardManager::writeOptionBlock() { issue(CMD_WRITE_OPTION); }

s32 TCardManager::getLastStatus()
{
	if (OSTryLockMutex(&mMutex)) {
		int result = mLastStatus;
		OSUnlockMutex(&mMutex);
		return result;
	}

	return CARD_RESULT_BUSY;
}

s32 TCardManager::probe_()
{
	s32 sectorSize;
	s32 result = CARDProbeEx(mChannel, nullptr, &sectorSize);

	// unsupported sector size
	if (result == CARD_RESULT_READY && sectorSize != 0x2000)
		result = -256;

	return result;
}

static void detachCallback(s32 param_1, s32) { sDetach[param_1] = 1; }

s32 TCardManager::mount_(bool param_1)
{
	if (sDetach[mChannel])
		unmount_();

	if (param_1 && mIsMounted && !mFsCheckedOk)
		unmount_();

	s32 result = 0;

	if (!mIsMounted) {
		mFsCheckedOk = false;

		for (;;) {
			if ((result = probe_()) != CARD_RESULT_BUSY)
				break;
			OSYieldThread();
		}

		if (result == CARD_RESULT_READY) {
			for (u32 i = 0; i < ARRAY_COUNT(mSectorCriteria); ++i)
				mSectorCriteria[i].set(TCriteria::STATE_UNREAD, 0, nullptr);

			result = CARDMount(mChannel, mCardMountWorkArea, &detachCallback);

			switch (result) {
			case CARD_RESULT_BROKEN:
			case CARD_RESULT_READY: {
				result = CARDCheck(mChannel);

				switch (result) {
				case CARD_RESULT_READY:
					mIsMounted   = true;
					mFsCheckedOk = true;
					break;

				case CARD_RESULT_BROKEN:
					mIsMounted = true;
					break;

				case CARD_RESULT_IOERROR:
					unmount_();
					break;
				}
			} break;

			case CARD_RESULT_ENCODING:
				mIsMounted = true;
				break;
			}
		}
	}

	return result;
}

s32 TCardManager::unmount_()
{
	s32 result = 0;
	if (mIsMounted) {
		result            = CARDUnmount(mChannel);
		mIsMounted        = false;
		mFsCheckedOk      = false;
		sDetach[mChannel] = 0;
	}
	return result;
}

s32 TCardManager::format_()
{
	s32 result = mount_(false);
	if (mIsMounted) {
		result = CARDFormat(mChannel);
		if (result == CARD_RESULT_READY)
			mFsCheckedOk = true;
		if (result == CARD_RESULT_IOERROR)
			unmount_();
	}
	return result;
}

s32 TCardManager::createFile_()
{
	s32 result = mount_(true);
	if (result == CARD_RESULT_READY) {
		CARDFileInfo info;
		result = CARDCreate(mChannel, (char*)CardFileName, 0xE000, &info);
		if (result == CARD_RESULT_READY) {
			for (u32 i = 0; i < ARRAY_COUNT(mSectorCriteria); ++i)
				mSectorCriteria[i].set(TCriteria::STATE_EMPTY, 0, nullptr);
			result   = filledInitData_(&info);
			s32 errc = CARDClose(&info);
			if (result == CARD_RESULT_READY)
				result = errc;
		}
	}
	if (result == CARD_RESULT_IOERROR)
		unmount_();

	return result;
}

s32 TCardManager::filledInitData_(CARDFileInfo* file)
{
	TCardSector* sector = (TCardSector*)mSector;
	if (mSectorCriteria[0].getState() == TCriteria::STATE_EMPTY) {
		sector->clearData();

		buildHeader_(sector->getHeader());

		sector->setCheckSum(0);

		s32 errc = writeCardSector_(file, 0, sector, &mSectorCriteria[0]);

		if (errc != 0)
			return errc;
	}

	sector->clearData();
	sector->setCheckSum(0);

	for (int i = 1; i < ARRAY_COUNT(mSectorCriteria); ++i) {
		TCriteria* crit = &mSectorCriteria[i];
		if (crit->getState() != TCriteria::STATE_EMPTY)
			continue;

		s32 errc = writeCardSector_(file, i, sector, crit);

		if (errc != 0)
			return errc;
	}

	return setCardStat_(file);
}

s32 TCardManager::setCardStat_(CARDFileInfo* file)
{
	s32 fileNo = CARDGetFileNo(file);

	CARDStat stat;

	int result = CARDGetStatus(mChannel, fileNo, &stat);
	if (result == 0) {
		CARDSetIconAddress(&stat, 0x44);
		CARDSetCommentAddress(&stat, 0x4);
		CARDSetBannerFormat(&stat, CARD_STAT_BANNER_C8);
		CARDSetIconAnim(&stat, CARD_STAT_ANIM_LOOP);

		CARDSetIconFormat(&stat, 0, CARD_STAT_ICON_C8);
		CARDSetIconSpeed(&stat, 0, CARD_STAT_SPEED_SLOW);
		CARDSetIconFormat(&stat, 1, CARD_STAT_ICON_C8);
		CARDSetIconSpeed(&stat, 1, CARD_STAT_SPEED_SLOW);

		for (u16 i = 2; i < CARD_ICON_MAX; ++i) {
			CARDSetIconFormat(&stat, i, CARD_STAT_ICON_NONE);
			CARDSetIconSpeed(&stat, i, CARD_STAT_SPEED_END);
		}
		result = CARDSetStatus(mChannel, file->fileNo, &stat);
	}
	return result;
}

void TCardManager::buildHeader_(HeaderData* header)
{
	int iVar8 = 0;
	if (TFlagManager::getInstance()->getFlag(0xA0001) != 0x100) {
		iVar8 = TFlagManager::getInstance()->getFlag(0xA0001);
		++iVar8;
	}

	snprintf(header->mTitle, 0x20, titles[iVar8]);
	OSCalendarTime auStack_54;
	OSTicksToCalendarTime(OSGetTime(), &auStack_54);
	if (iVar8 == 0) {
		snprintf(header->mComment, 0x20, comments[iVar8], auStack_54.mon + 1,
		         auStack_54.mday);
	} else {
		snprintf(header->mComment, 0x20, comments[iVar8], auStack_54.mday,
		         auStack_54.mon + 1);
	}
	memcpy(header->mBanner, mBanner, sizeof(header->mBanner));
	memcpy(header->mIcons, mIcons, sizeof(header->mIcons));
}

s32 TCardManager::open_(CARDFileInfo* file)
{
	s32 result = mount_(true);
	if (result == CARD_RESULT_READY) {
		bool valid;
		mNeedsInit = valid = false;
		result             = CARDOpen(mChannel, (char*)CardFileName, file);
		if (result == CARD_RESULT_READY) {
			CARDStat stat;
			result = CARDGetStatus(mChannel, CARDGetFileNo(file), &stat);
			if (result == CARD_RESULT_READY) {
				if (stat.iconAddr != 0xffffffff
				    && stat.commentAddr != 0xffffffff)
					valid = true;
				if (!valid) {
					mNeedsInit = true;
					for (u32 i = 0; i < ARRAY_COUNT(mSectorCriteria); ++i)
						mSectorCriteria[i].set(TCriteria::STATE_EMPTY, 0,
						                       nullptr);
				}
			}
		} else if (result == CARD_RESULT_NOFILE) {
			s32 byteNotUsed;
			s32 filesNotUsed;
			result = CARDFreeBlocks(mChannel, &byteNotUsed, &filesNotUsed);
			if (result == CARD_RESULT_READY) {
				if (byteNotUsed < 0xE000) {
					result = CARD_RESULT_INSSPACE;
				} else if (filesNotUsed < 1) {
					result = CARD_RESULT_NOENT;
				} else {
					result = CARD_RESULT_NOFILE;
				}
			}
		}
	}

	return result;
}

s32 TCardManager::getBookmarkInfos_()
{
	s32 result = mount_(true);
	if (result != 0)
		return result;

	bool any = false;

	for (u32 i = 1; i < ARRAY_COUNT(mSectorCriteria); ++i)
		if (mSectorCriteria[i].getState() == TCriteria::STATE_UNREAD) {
			any = true;
			break;
		}

	if (any) {
		CARDFileInfo info;
		result = open_(&info);
		if (result == CARD_RESULT_READY) {
			if (!mNeedsInit) {
				for (u32 i = 1; i < ARRAY_COUNT(mSectorCriteria); ++i) {
					if (mSectorCriteria[i].getState()
					    != TCriteria::STATE_UNREAD)
						continue;

					result = ((TCardSector*)mSector)
					             ->read(&info, i, &mSectorCriteria[i]);
					if (result != CARD_RESULT_READY)
						break;
				}
			}

			s32 errc = CARDClose(&info);
			if (result == CARD_RESULT_READY)
				result = errc;
		}
	}

	if (result == 0) {
		TCardBookmarkInfo* marks = mGetBookmarkInfosArg;
		for (int i = 0; i < 3; ++i)
			copyTo(&mSectorCriteria[2 * i + 1], &marks[i]);
	}

	return result;
}

s32 TCardManager::readBlock_(u32 index)
{
	CARDFileInfo info;
	s32 result = open_(&info);
	if (result != CARD_RESULT_READY)
		return result;

	TCardSector* sector = (TCardSector*)mSector;

	u32 crit_idx = index * 2 + 1;

	if (mSectorCriteria[crit_idx].mState == TCriteria::STATE_UNREAD)
		result = sector->read(&info, crit_idx, mSectorCriteria + crit_idx);

	if (result == CARD_RESULT_READY)
		if (mSectorCriteria[crit_idx + 1].mState == TCriteria::STATE_UNREAD)
			result = sector->read(&info, crit_idx + 1,
			                      mSectorCriteria + (crit_idx + 1));

	if (result == CARD_RESULT_READY) {
		int sec = decideUseSector(&mSectorCriteria[crit_idx]);
		if (sec == -1) {
			result = -0x80;
		} else if (sec == -2) {
			sector->clearData();
			sector->setCheckSum(0);
		} else {
			result = sector->read(&info, crit_idx + sec,
			                      mSectorCriteria + (crit_idx + sec));
		}
	}

	s32 errc = CARDClose(&info);
	if (result == CARD_RESULT_READY)
		result = errc;

	return result;
}

s32 TCardManager::readOptionBlock_()
{
	CARDFileInfo info;
	s32 result = open_(&info);
	if (result == CARD_RESULT_READY) {
		TCardSector* sector = (TCardSector*)mSector;

		if (mSectorCriteria[0].mState == TCriteria::STATE_UNREAD) {
			sector->clearData();
			sector->setCheckSum(0);
		} else {
			result   = sector->read(&info, 0, &mSectorCriteria[0]);
			s32 errc = CARDClose(&info);
			if (result == CARD_RESULT_READY)
				result = errc;
		}
	}

	return result;
}

s32 TCardManager::writeBlock_(u32 index)
{
	s32 crit_idx = index * 2 + 1;
	if (mSectorCriteria[crit_idx].getState() == TCriteria::STATE_UNREAD
	    || mSectorCriteria[crit_idx + 1].getState() == TCriteria::STATE_UNREAD)
		return -0x80;

	CARDFileInfo info;
	s32 result = open_(&info);
	if (result != CARD_RESULT_READY)
		return result;

	TCardSector* sector = (TCardSector*)mSector;

	u32 wc = getWriteCount(&mSectorCriteria[crit_idx]);
	sector->setCheckSum(wc + 1);

	result = writeCardSector_(&info, crit_idx + ((wc + 1) % 2), sector,
	                          &mSectorCriteria[crit_idx + ((wc + 1) % 2)]);

	if (result == CARD_RESULT_READY && mNeedsInit)
		result = filledInitData_(&info);

	s32 errc2 = CARDClose(&info);
	if (result == CARD_RESULT_READY)
		result = errc2;
	else if (result == CARD_RESULT_IOERROR)
		unmount_();

	return result;
}

s32 TCardManager::writeOptionBlock_()
{
	CARDFileInfo info;
	s32 result = open_(&info);
	if (result != CARD_RESULT_READY)
		return result;

	TCardSector* sector = (TCardSector*)mSector;

	sector->setCheckSum(0);

	result = writeCardSector_(&info, 0, sector, mSectorCriteria);

	if (result == CARD_RESULT_READY && mNeedsInit)
		result = filledInitData_(&info);

	s32 errc = CARDClose(&info);
	if (result == CARD_RESULT_READY)
		result = errc;
	else if (result == CARD_RESULT_IOERROR)
		unmount_();

	return result;
}

// TODO: incorrect
s32 TCardManager::writeCardSector_(CARDFileInfo* file, s32 index,
                                   TCardSector* sector,
                                   TCardManager::TCriteria* criteria)
{
	s32 errc = CARDWrite(file, sector, sizeof(TCardSector),
	                     index * sizeof(TCardSector));

	criteria->set(TCriteria::STATE_UNREAD, 0, nullptr);
	if (errc != CARD_RESULT_READY)
		return errc;

	errc = sector->read(file, index, criteria);

	return errc;
}

s32 TCardManager::cmdLoop()
{
	bool doLoop = true;
	while (doLoop) {
		OSLockMutex(&mMutex);
		while (mPendingCommand == CMD_NONE)
			OSWaitCond(&mCommandIsPending, &mMutex);
		switch (mPendingCommand) {
		case CMD_FORMAT:
			mLastStatus = format_();
			break;
		case CMD_CREATE:
			mLastStatus = createFile_();
			break;
		case CMD_BOOKMARK_SCAN:
			mLastStatus = getBookmarkInfos_();
			break;
		case CMD_READ_BLOCK:
			mLastStatus = readBlock_(mReadWriteBlockArg);
			break;
		case CMD_WRITE_BLOCK:
			mLastStatus = writeBlock_(mReadWriteBlockArg);
			break;
		case CMD_READ_OPTION:
			mLastStatus = readOptionBlock_();
			break;
		case CMD_WRITE_OPTION:
			mLastStatus = writeOptionBlock_();
			break;
		case CMD_THREAD_EXIT:
			doLoop = false;
			break;
		}
		mPendingCommand = CMD_NONE;
		OSUnlockMutex(&mMutex);
	}
	return 0;
}
