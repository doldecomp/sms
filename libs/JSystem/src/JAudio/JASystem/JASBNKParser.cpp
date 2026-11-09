#include <JSystem/JAudio/JASystem/JASBNKParser.hpp>
#include <JSystem/JAudio/JASystem/JASBasicInst.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASInstRand.hpp>
#include <JSystem/JAudio/JASystem/JASInstSense.hpp>
#include <JSystem/JAudio/JASystem/JASBasicInst.hpp>
#include <JSystem/JAudio/JASystem/JASDrumSet.hpp>
#include <JSystem/JSupport.hpp>

namespace JASystem {

namespace BNKParser {

	u32 sUsedHeapSize = 0;

	static TOscillator::Osc_* findOscPtr(TBasicBank* bank, THeader* header,
	                                     TOsc* osc);
	static s16* getOscTableEndPtr(s16* ptr);

	TBasicBank* createBasicBank(void* data)
	{
		JKRHeap* heap      = TBank::getCurrentHeap();
		const u32 freeSize = heap->getFreeSize();
		THeader* header    = (THeader*)data;
		TBasicBank* bank   = new (heap, 0) TBasicBank();
		if (bank == nullptr) {
			return nullptr;
		}
		bank->setInstCount(0x100);

		for (int i = 0; i < 0x80; i++) {
			TInst* instRaw = header->mInstOffsets[i].ptr(header);
			if (instRaw != nullptr) {
				TBasicInst* instp = new (heap, 0) TBasicInst();
				instp->setVolume(instRaw->mVolume);
				instp->setPitch(instRaw->mPitch);

				instp->setOscCount(2);
				for (int oscIndex = 0, j = 0; j < 2; j++) {
					TOsc* oscRaw = instRaw->mOscOffsets[j].ptr(header);
					if (oscRaw != nullptr) {
						TOscillator::Osc_* osc
						    = findOscPtr(bank, header, oscRaw);
						if (osc == nullptr) {
							osc           = new (heap, 0) TOscillator::Osc_;
							osc->mTarget  = oscRaw->mTarget;
							osc->mRate    = oscRaw->mRate;
							s16* oscTable = oscRaw->mAdsTableOffset.ptr(header);
							if (oscTable != nullptr) {
								s32 tableLength
								    = getOscTableEndPtr(oscTable) - oscTable;
								osc->mAdsTable = new (heap, 0) s16[tableLength];
								Calc::bcopy(oscTable, osc->mAdsTable,
								            tableLength * sizeof(s16));
							} else {
								osc->mAdsTable = nullptr;
							}
							oscTable = oscRaw->mRelTableOffset.ptr(header);
							if (oscTable != nullptr) {
								s32 tableLength
								    = getOscTableEndPtr(oscTable) - oscTable;
								osc->mRelTable = new (heap, 0) s16[tableLength];
								Calc::bcopy(oscTable, osc->mRelTable,
								            tableLength * sizeof(s16));
							} else {
								osc->mRelTable = nullptr;
							}
							osc->mWidth  = oscRaw->mWidth;
							osc->mVertex = oscRaw->mVertex;
						}
						instp->setOsc(oscIndex, osc);
						oscIndex++;
					}
				}

				instp->setEffectCount(4);
				for (int j = 0; j < 2; j++) {
					TRand* randRaw = instRaw->mRandOffsets[j].ptr(header);
					if (randRaw != nullptr) {
						TInstRand* randp = new (heap, 0) TInstRand;
						randp->setTarget(randRaw->mTarget);
						randp->setBase(randRaw->mBase);
						randp->setWidth(randRaw->mWidth);
						instp->setEffect(j, randp);
					}
				}
				for (int j = 0; j < 2; j++) {
					TSense* senseRaw = instRaw->mSenseOffsets[j].ptr(header);
					if (senseRaw != nullptr) {
						TInstSense* sensep = new (heap, 0) TInstSense;
						sensep->setTarget(senseRaw->mTarget);
						sensep->setParams(senseRaw->mSource, senseRaw->mCenter,
						                  senseRaw->mLowValue,
						                  senseRaw->mHighValue);
						instp->setEffect(j + 2, sensep);
					}
				}

				instp->setKeyRegionCount(instRaw->mKeyRegionCount);
				for (int j = 0; j < instRaw->mKeyRegionCount; j++) {
					TBasicInst::TKeymap* instKeymap = instp->getKeyRegion(j);
					TKeymap* keymapRaw = instRaw->mKeymapOffsets[j].ptr(header);
					instKeymap->setHighKey(keymapRaw->unk0);
					instKeymap->setVeloRegionCount(keymapRaw->mVeloRegionCount);
					for (int k = 0; k < keymapRaw->mVeloRegionCount; k++) {
						TVeloRegion* instVeloRegion
						    = instKeymap->getVeloRegion(k);
						TVmap* vmapRaw
						    = keymapRaw->mVeloRegionOffsets[k].ptr(header);
						instVeloRegion->unk0    = vmapRaw->mHighVelocity;
						instVeloRegion->mWaveId = JSULoHalf(vmapRaw->mWaveId);
						instVeloRegion->mVolume = vmapRaw->mVolume;
						instVeloRegion->mPitch  = vmapRaw->mPitch;
					}
				}
				bank->setInst(i, instp);
			}
		}

		for (int i = 0; i < 12; i++) {
			TPerc* percRaw = header->mPercOffsets[i].ptr(header);
			if (percRaw != nullptr) {
				TDrumSet* setp = new (heap, 0) TDrumSet;
				for (int j = 0; j < 0x80; j++) {
					TPmap* pmapRaw = percRaw->mPmapOffsets[j].ptr(header);
					if (pmapRaw != nullptr) {
						TDrumSet::TPerc* drumSetPerc = setp->getPerc(j);
						drumSetPerc->setVolume(pmapRaw->mVolume);
						drumSetPerc->setPitch(pmapRaw->mPitch);
						if (percRaw->mMagic == 'PER2') {
							drumSetPerc->setPan(percRaw->mPan[j] / 127.0f);
							drumSetPerc->setRelease(percRaw->mRelease[j]);
						}
						drumSetPerc->setEffectCount(2);
						for (int effectIndex = 0, k = 0; k < 2; k++) {
							TRand* randRaw
							    = pmapRaw->mRandOffsets[k].ptr(header);
							if (randRaw != nullptr) {
								TInstRand* randp = new (heap, 0) TInstRand();
								randp->setTarget(randRaw->mTarget);
								randp->setBase(randRaw->mBase);
								randp->setWidth(randRaw->mWidth);
								drumSetPerc->setEffect(effectIndex, randp);
								effectIndex++;
							}
						}
						drumSetPerc->setVeloRegionCount(
						    pmapRaw->mVeloRegionCount);
						for (int k = 0; k < pmapRaw->mVeloRegionCount; k++) {
							TVeloRegion* instVeloRegion
							    = drumSetPerc->getVeloRegion(k);
							TVmap* vmapRaw
							    = pmapRaw->mVeloRegionOffsets[k].ptr(header);
							instVeloRegion->unk0 = vmapRaw->mHighVelocity;
							instVeloRegion->mWaveId
							    = JSULoHalf(vmapRaw->mWaveId);
							instVeloRegion->mVolume = vmapRaw->mVolume;
							instVeloRegion->mPitch  = vmapRaw->mPitch;
						}
					}
				}
				bank->setInst(i + 0xE4, setp);
			}
		}
		sUsedHeapSize += freeSize - heap->getFreeSize();
		return bank;
	}

	static TOscillator::Osc_* findOscPtr(TBasicBank* bank, THeader* header,
	                                     TOsc* osc)
	{
		TOffset<TInst>* instOffsets = header->mInstOffsets - 1;
		for (int i = 0; i < 128; i++) {
			TInst* instRaw = instOffsets[i + 1].ptr(header);
			if (instRaw == nullptr) {
				continue;
			}
			for (int j = 0; j < 2; j++) {
				if (instRaw->mOscOffsets[j].ptr(header) != osc) {
					continue;
				}
				JASystem::TInst* inst = bank->getInst(i);
				if (inst == nullptr) {
					continue;
				}
				TInstParam param;
				inst->getParam(60, 127, &param);
				if (j < param.mOscCount) {
					return param.mOscData[j];
				}
			}
		}
		return nullptr;
	}

	static s16* getOscTableEndPtr(s16* ptr)
	{
		s16 v1;
		do {
			v1 = *ptr;
			ptr += 3;
		} while (v1 <= 0xa);
		return ptr;
	}

	u32 getUsedHeapSize() { return sUsedHeapSize; }

} // namespace BNKParser

} // namespace JASystem
