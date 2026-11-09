#include <JSystem/JAudio/JASystem/JASWSParser.hpp>
#include <JSystem/JAudio/JASystem/JASWaveBank.hpp>
#include <JSystem/JAudio/JASystem/JASBasicWaveBank.hpp>
#include <JSystem/JAudio/JASystem/JASSimpleWaveBank.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JSupport.hpp>

namespace JASystem {

u32 WSParser::sUsedHeapSize = 0;

u32 WSParser::getGroupCount(void* data)
{
	THeader* header = (THeader*)data;
	return header->mCtrlGroupOffset.ptr(data)->mCtrlGroupCount;
}

TWaveBank* WSParser::createBasicWaveBank(void* data)
{
	JKRHeap* heap           = TWaveBank::getCurrentHeap();
	const u32 priorFreeSize = heap->getFreeSize();
	const THeader* header   = (THeader*)data;
	TBasicWaveBank* bank    = new (heap, 0) TBasicWaveBank;
	if (bank == nullptr)
		return nullptr;

	const TCtrlGroup* ctrlGroupRaw = header->mCtrlGroupOffset.ptr(header);
	bank->setGroupCount(ctrlGroupRaw->mCtrlGroupCount);
	u32 maxSize = 0;
	for (int groupIndex = 0; groupIndex < ctrlGroupRaw->mCtrlGroupCount;
	     groupIndex++) {
		TCtrlScene* ctrlSceneRaw
		    = ctrlGroupRaw->mCtrlSceneOffsets[groupIndex].ptr(header);
		TCtrl* ctrlRaw = ctrlSceneRaw->mCtrlOffset.ptr(header);
		TBasicWaveBank::TWaveGroup* waveGroup = bank->getWaveGroup(groupIndex);
		TWaveArchiveBank* archiveBankRaw
		    = header->mArchiveBankOffset.ptr(header);
		TWaveArchive* archiveRaw
		    = archiveBankRaw->mArchiveOffsets[groupIndex].ptr(header);
		waveGroup->setWaveCount(ctrlRaw->mWaveCount);
		for (int waveIndex = 0; waveIndex < ctrlRaw->mWaveCount; waveIndex++) {
			TWave* waveRaw = archiveRaw->mWaveOffsets[waveIndex].ptr(header);
			TWaveInfo info;

			info.unk0          = waveRaw->unk0;
			info.mFormat       = waveRaw->mFormat;
			info.mKey          = waveRaw->mKey;
			info.mSampleRate   = waveRaw->mSampleRate;
			info.mOffset       = waveRaw->mOffset;
			info.mDataSize     = waveRaw->mDataSize;
			info.mLoop         = waveRaw->mLoop;
			info.mLoopStart    = waveRaw->mLoopStart;
			info.mLoopEnd      = waveRaw->mLoopEnd;
			info.mSampleCount  = waveRaw->mSampleCount;
			info.mLoopHistory1 = waveRaw->mLoopHistory1;
			info.mLoopHistory2 = waveRaw->mLoopHistory2;
			info.unk28         = waveRaw->unk28;

			TCtrlWave* ctrlWaveRaw
			    = ctrlRaw->mCtrlWaveOffsets[waveIndex].ptr(header);
			u32 size = JSULoHalf(ctrlWaveRaw->unk0);
			waveGroup->setWaveInfo(waveIndex, size, info);
			if (maxSize < size) {
				maxSize = size;
			}
		}
		waveGroup->setWaveArcFileName(archiveRaw->mFileName);
	}
	bank->setWaveTableSize(maxSize + 1);
	sUsedHeapSize += priorFreeSize - heap->getFreeSize();
	return bank;
}

TWaveBank* WSParser::createSimpleWaveBank(void* data)
{
	JKRHeap* heap                  = TWaveBank::getCurrentHeap();
	const u32 priorFreeSize        = heap->getFreeSize();
	const THeader* header          = (THeader*)data;
	const TCtrlGroup* ctrlGroupRaw = header->mCtrlGroupOffset.ptr(header);
	if (ctrlGroupRaw->mCtrlGroupCount != 1)
		return nullptr;

	TSimpleWaveBank* bank = new (heap, 0) TSimpleWaveBank;
	if (bank == nullptr)
		return nullptr;

	u32 maxSize = 0;

	const TCtrlScene* ctrlSceneRaw
	    = ctrlGroupRaw->mCtrlSceneOffsets[0].ptr(header);
	const TCtrl* ctrlRaw = ctrlSceneRaw->mCtrlOffset.ptr(header);
	const TWaveArchiveBank* archiveBankRaw
	    = header->mArchiveBankOffset.ptr(header);
	const TWaveArchive* archiveRaw
	    = archiveBankRaw->mArchiveOffsets[0].ptr(header);
	for (int waveIndex = 0; waveIndex < ctrlRaw->mWaveCount; waveIndex++) {
		TCtrlWave* ctrlWaveRaw
		    = ctrlRaw->mCtrlWaveOffsets[waveIndex].ptr(header);
		u32 size = JSULoHalf(ctrlWaveRaw->unk0);
		if (maxSize < size) {
			maxSize = size;
		}
	}
	bank->setWaveTableSize(maxSize + 1);
	for (int waveIndex = 0; waveIndex < ctrlRaw->mWaveCount; waveIndex++) {
		TWave* waveRaw = archiveRaw->mWaveOffsets[waveIndex].ptr(header);
		TWaveInfo info;
		info.unk0          = waveRaw->unk0;
		info.mFormat       = waveRaw->mFormat;
		info.mKey          = waveRaw->mKey;
		info.mSampleRate   = waveRaw->mSampleRate;
		info.mOffset       = waveRaw->mOffset;
		info.mDataSize     = waveRaw->mDataSize;
		info.mLoop         = waveRaw->mLoop;
		info.mLoopStart    = waveRaw->mLoopStart;
		info.mLoopEnd      = waveRaw->mLoopEnd;
		info.mSampleCount  = waveRaw->mSampleCount;
		info.mLoopHistory1 = waveRaw->mLoopHistory1;
		info.mLoopHistory2 = waveRaw->mLoopHistory2;
		info.unk28         = waveRaw->unk28;

		TCtrlWave* ctrlWaveRaw
		    = ctrlRaw->mCtrlWaveOffsets[waveIndex].ptr(header);
		u32 size = JSULoHalf(ctrlWaveRaw->unk0);
		bank->setWaveInfo(size, info);
	}
	bank->setWaveArcFileName(archiveRaw->mFileName);
	sUsedHeapSize += priorFreeSize - heap->getFreeSize();
	return bank;
}

u32 WSParser::getUsedHeapSize() { return sUsedHeapSize; }

} // namespace JASystem
