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
	return JSUConvertOffsetToPtr<TCtrlGroup>(data, header->mCtrlGroupOffset)
	    ->mCtrlGroupCount;
}

TWaveBank* WSParser::createBasicWaveBank(void* data)
{
	TWaveArchive* archiveRaw;
	JKRHeap* heap           = TWaveBank::getCurrentHeap();
	const u32 priorFreeSize = heap->getFreeSize();
	const THeader* header   = (THeader*)data;
	TBasicWaveBank* bank    = new (heap, 0) TBasicWaveBank;
	if (bank == nullptr)
		return nullptr;

	const TCtrlGroup* ctrlGroupRaw
	    = JSUConvertOffsetToPtr<TCtrlGroup>(header, header->mCtrlGroupOffset);
	bank->setGroupCount(ctrlGroupRaw->mCtrlGroupCount);
	u32 maxSize = 0;
	for (int groupIndex = 0; groupIndex < ctrlGroupRaw->mCtrlGroupCount;
	     groupIndex++) {
		TCtrlScene* ctrlSceneRaw = JSUConvertOffsetToPtr<TCtrlScene>(
		    header, ctrlGroupRaw->mCtrlSceneOffsets[groupIndex]);
		TCtrl* ctrlRaw
		    = JSUConvertOffsetToPtr<TCtrl>(header, ctrlSceneRaw->mCtrlOffset);
		TBasicWaveBank::TWaveGroup* waveGroup = bank->getWaveGroup(groupIndex);
		TWaveArchiveBank* archiveBankRaw
		    = JSUConvertOffsetToPtr<TWaveArchiveBank>(
		        header, header->mArchiveBankOffset);
		archiveRaw = JSUConvertOffsetToPtr<TWaveArchive>(
		    header, archiveBankRaw->mArchiveOffsets[groupIndex]);
		waveGroup->setWaveCount(ctrlRaw->mWaveCount);
		for (int waveIndex = 0; waveIndex < ctrlRaw->mWaveCount; waveIndex++) {
			TWave* waveRaw = JSUConvertOffsetToPtr<TWave>(
			    header, archiveRaw->mWaveOffsets[waveIndex]);
			TWaveInfo info;

			info.unk0  = waveRaw->unk0;
			info.unk1  = waveRaw->unk1;
			info.unk2  = waveRaw->unk2;
			info.unk4  = waveRaw->unk4;
			info.unk8  = waveRaw->mOffset;
			info.unkC  = waveRaw->unkC;
			info.unk10 = waveRaw->unk10;
			info.unk14 = waveRaw->unk14;
			info.unk18 = waveRaw->unk18;
			info.unk1c = waveRaw->unk1c;
			info.unk20 = waveRaw->unk20;
			info.unk22 = waveRaw->unk22;
			info.unk28 = waveRaw->unk28;

			TCtrlWave* ctrlWaveRaw = JSUConvertOffsetToPtr<TCtrlWave>(
			    header, ctrlRaw->mCtrlWaveOffsets[waveIndex]);
			u32 size = (u16)ctrlWaveRaw->unk0;
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
	const TWaveArchive* archiveRaw;
	JKRHeap* heap           = TWaveBank::getCurrentHeap();
	const u32 priorFreeSize = heap->getFreeSize();
	const THeader* header   = (THeader*)data;
	const TCtrlGroup* ctrlGroupRaw
	    = JSUConvertOffsetToPtr<TCtrlGroup>(header, header->mCtrlGroupOffset);
	if (ctrlGroupRaw->mCtrlGroupCount != 1)
		return nullptr;

	TSimpleWaveBank* bank = new (heap, 0) TSimpleWaveBank;
	if (bank == nullptr)
		return nullptr;

	u32 maxSize = 0;

	const TCtrlScene* ctrlSceneRaw = JSUConvertOffsetToPtr<TCtrlScene>(
	    header, ctrlGroupRaw->mCtrlSceneOffsets[0]);
	const TCtrl* ctrlRaw
	    = JSUConvertOffsetToPtr<TCtrl>(header, ctrlSceneRaw->mCtrlOffset);
	const TWaveArchiveBank* archiveBankRaw
	    = JSUConvertOffsetToPtr<TWaveArchiveBank>(header,
	                                              header->mArchiveBankOffset);
	archiveRaw = JSUConvertOffsetToPtr<TWaveArchive>(
	    header, archiveBankRaw->mArchiveOffsets[0]);
	for (int waveIndex = 0; waveIndex < ctrlRaw->mWaveCount; waveIndex++) {
		TCtrlWave* ctrlWaveRaw = JSUConvertOffsetToPtr<TCtrlWave>(
		    header, ctrlRaw->mCtrlWaveOffsets[waveIndex]);
		u32 size = ctrlWaveRaw->unk0 & 0xFFFF;
		if (maxSize < size) {
			maxSize = size;
		}
	}
	bank->setWaveTableSize(maxSize + 1);
	for (int waveIndex = 0; waveIndex < ctrlRaw->mWaveCount; waveIndex++) {
		TWave* waveRaw = JSUConvertOffsetToPtr<TWave>(
		    header, archiveRaw->mWaveOffsets[waveIndex]);

		TWaveInfo info;
		info.unk0  = waveRaw->unk0;
		info.unk1  = waveRaw->unk1;
		info.unk2  = waveRaw->unk2;
		info.unk4  = waveRaw->unk4;
		info.unk8  = waveRaw->mOffset;
		info.unkC  = waveRaw->unkC;
		info.unk10 = waveRaw->unk10;
		info.unk14 = waveRaw->unk14;
		info.unk18 = waveRaw->unk18;
		info.unk1c = waveRaw->unk1c;
		info.unk20 = waveRaw->unk20;
		info.unk22 = waveRaw->unk22;
		info.unk28 = waveRaw->unk28;

		bank->setWaveInfo((u16)JSUConvertOffsetToPtr<TCtrlWave>(
		                      header, ctrlRaw->mCtrlWaveOffsets[waveIndex])
		                      ->unk0,
		                  info);
	}
	bank->setWaveArcFileName(archiveRaw->mFileName);
	sUsedHeapSize += priorFreeSize - heap->getFreeSize();
	return bank;
}

} // namespace JASystem
