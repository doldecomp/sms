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
			TInst* instRaw
			    = JSUConvertOffsetToPtr<TInst>(header, header->mInstOffsets[i]);
			if (instRaw != nullptr) {
				TBasicInst* instp = new (heap, 0) TBasicInst();
				instp->unk4       = instRaw->unk8;
				instp->unk8       = instRaw->unkC;

				instp->setOscCount(2);
				for (int oscIndex = 0, j = 0; j < 2; j++) {
					TOsc* oscRaw2 = JSUConvertOffsetToPtr<TOsc>(
					    header, instRaw->mOscOffsets[j]);
					TOsc* oscRaw = oscRaw2;
					if (oscRaw != nullptr) {
						TOscillator::Osc_* osc
						    = findOscPtr(bank, header, oscRaw);
						if (osc == nullptr) {
							osc           = new (heap, 0) TOscillator::Osc_;
							osc->unk0     = oscRaw->unk0;
							osc->unk4     = oscRaw->unk4;
							s16* oscTable = JSUConvertOffsetToPtr<s16>(
							    header, oscRaw->unk8);
							if (oscTable != nullptr) {
								s32 tableLength
								    = getOscTableEndPtr(oscTable) - oscTable;
								osc->unk8 = new (heap, 0) s16[tableLength];
								Calc::bcopy(oscTable, osc->unk8,
								            tableLength * sizeof(s16));
							} else {
								osc->unk8 = nullptr;
							}
							oscTable = JSUConvertOffsetToPtr<s16>(header,
							                                      oscRaw->unkC);
							if (oscTable != nullptr) {
								s32 tableLength
								    = getOscTableEndPtr(oscTable) - oscTable;
								osc->unkC = new (heap, 0) s16[tableLength];
								Calc::bcopy(oscTable, osc->unkC,
								            tableLength * sizeof(s16));
							} else {
								osc->unkC = nullptr;
							}
							osc->unk10 = oscRaw->unk10;
							osc->unk14 = oscRaw->unk14;
						}
						instp->setOsc(oscIndex, osc);
						oscIndex++;
					}
				}

				instp->setEffectCount(4);
				for (int j = 0; j < 2; j++) {
					TRand* randRaw = JSUConvertOffsetToPtr<TRand>(
					    header, instRaw->mRandOffsets[j]);
					if (randRaw != nullptr) {
						TInstRand* randp = new (heap, 0) TInstRand;
						randp->setTarget(randRaw->unk0);
						randp->unk8 = randRaw->unk4;
						randp->unkC = randRaw->unk8;
						instp->setEffect(j, randp);
					}
				}
				for (int j = 0; j < 2; j++) {
					TSense* senseRaw = JSUConvertOffsetToPtr<TSense>(
					    header, instRaw->mSenseOffsets[j]);
					if (senseRaw != nullptr) {
						TInstSense* sensep = new (heap, 0) TInstSense;
						sensep->setTarget(senseRaw->unk0);
						sensep->setParams(senseRaw->unk1, senseRaw->unk2,
						                  senseRaw->unk4, senseRaw->unk8);
						instp->setEffect(j + 2, sensep);
					}
				}

				instp->setKeyRegionCount(instRaw->mKeyRegionCount);
				for (int j = 0; j < instRaw->mKeyRegionCount; j++) {
					TBasicInst::TKeymap* instKeymap = instp->getKeyRegion(j);
					TKeymap* keymapRaw = JSUConvertOffsetToPtr<TKeymap>(
					    header, instRaw->mKeymapOffsets[j]);
					instKeymap->unk0 = keymapRaw->unk0;
					instKeymap->setVeloRegionCount(keymapRaw->unk4);
					for (int k = 0; k < keymapRaw->unk4; k++) {
						TVeloRegion* instVeloRegion
						    = instKeymap->getVeloRegion(k);
						TVmap* vmapRaw = JSUConvertOffsetToPtr<TVmap>(
						    header, keymapRaw->mVmapOffsets[k]);
						instVeloRegion->unk0 = vmapRaw->unk0;
						instVeloRegion->unk4 = vmapRaw->unk4 & 0xFFFF;
						instVeloRegion->unk8 = vmapRaw->unk8;
						instVeloRegion->unkC = vmapRaw->unkC;
					}
				}
				bank->setInst(i, instp);
			}
		}

		for (int i = 0; i < 12; i++) {
			TPerc* percRaw
			    = JSUConvertOffsetToPtr<TPerc>(header, header->mPercOffsets[i]);
			if (percRaw != nullptr) {
				TDrumSet* setp = new (heap, 0) TDrumSet;
				for (int j = 0; j < 0x80; j++) {
					TPmap* pmapRaw = JSUConvertOffsetToPtr<TPmap>(
					    header, percRaw->mPmapOffsets[j]);
					if (pmapRaw != nullptr) {
						TDrumSet::TPerc* drumSetPerc = setp->getPerc(j);
						drumSetPerc->unk0            = pmapRaw->unk0;
						drumSetPerc->unk4            = pmapRaw->unk4;
						if (percRaw->mMagic == 'PER2') {
							drumSetPerc->unk8 = percRaw->unk288[j] / 127.0f;
							drumSetPerc->setRelease(percRaw->unk308[j]);
						}
						drumSetPerc->setEffectCount(2);
						for (int effectIndex = 0, k = 0; k < 2; k++) {
							TRand* randRaw = JSUConvertOffsetToPtr<TRand>(
							    header, pmapRaw->mRandOffsets[k]);
							if (randRaw != nullptr) {
								TInstRand* randp = new (heap, 0) TInstRand();
								randp->setTarget(randRaw->unk0);
								randp->unk8 = randRaw->unk4;
								randp->unkC = randRaw->unk8;
								drumSetPerc->setEffect(effectIndex, randp);
								effectIndex++;
							}
						}
						drumSetPerc->setVeloRegionCount(
						    pmapRaw->mVeloRegionCount);
						for (int k = 0; k < pmapRaw->mVeloRegionCount; k++) {
							TVeloRegion* instVeloRegion
							    = drumSetPerc->getVeloRegion(k);
							TVmap* vmapRaw = JSUConvertOffsetToPtr<TVmap>(
							    header, pmapRaw->mVeloRegionOffsets[k]);
							instVeloRegion->unk0 = vmapRaw->unk0;
							instVeloRegion->unk4 = vmapRaw->unk4 & 0xFFFF;
							instVeloRegion->unk8 = vmapRaw->unk8;
							instVeloRegion->unkC = vmapRaw->unkC;
						}
					}
				}
				bank->setInst(i + 0xE4, setp);
			}
		}
		sUsedHeapSize += freeSize - heap->getFreeSize();
		return bank;
	}

	TOscillator::Osc_* findOscPtr(TBasicBank* bank, THeader* header, TOsc* osc)
	{
		u32* instOffsets = header->mInstOffsets - 1;
		for (int i = 0; i < 128; i++) {
			TInst* instRaw
			    = JSUConvertOffsetToPtr<TInst>(header, instOffsets[i + 1]);
			if (instRaw != nullptr) {
				for (int j = 0; j < 2; j++) {
					TOsc* oscRaw = JSUConvertOffsetToPtr<TOsc>(
					    header, instRaw->mOscOffsets[j]);
					if (oscRaw == osc) {
						JASystem::TInst* inst = bank->getInst(i);
						if (inst != nullptr) {
							TInstParam param;
							inst->getParam(60, 127, &param);
							if (j < param.mOscCount) {
								return param.mOscData[j];
							}
						}
					}
				}
			}
		}
		return nullptr;
	}

	s16* getOscTableEndPtr(s16* ptr)
	{
		s16 v1;
		do {
			v1 = *ptr;
			ptr += 3;
		} while (v1 <= 0xa);
		return ptr;
	}

	u32 getUsedHeapSize() { return 0; }

} // namespace BNKParser

} // namespace JASystem
