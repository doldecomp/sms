#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <JSystem/JAudio/JASystem/JASCmdStack.hpp>
#include <JSystem/JAudio/JASystem/JASVload.hpp>
#include <math.h>

void JAIBasic::stopSeq(JAISound* param_1)
{
	if (param_1->getSwBit() & 1) {
		for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
			JAISound* sound = unk0->unk180[i].unk48;
			if (param_1 != sound && sound) {
				if (sound->unk1 >= 3 && !(sound->getSwBit() & 2)) {
					sound->setSeqInterVolume(10, 1.0f, 10);
					JASystem::TrackMgr::handleToSeq(
					    sound->getSeqParameter()->unk0)
					    ->unPauseTrackAll();
				}
			}
		}
	}

	param_1->unk34                      = nullptr;
	param_1->getSeqParameter()->unk1850 = nullptr;
	if (param_1->unk1 >= 3) {
		unk0->releaseAutoHeapPointer(param_1->getSeqParameter()->unk1754);
	}
	param_1->unk1 = 0;
	releaseSeqParameterPointer(param_1->getSeqParameter());
	releaseControllerHandle(&unk0->unk210, param_1);
	unk0->unk180[param_1->unk0].unk48 = nullptr;
}

void JAIBasic::checkEntriedSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISeqUpdateData* sud = &unk0->unk180[i];
		JAISound** sound      = &sud->unk48;

		u32& r27 = sud->unk8;

		if (!*sound)
			continue;

		if (!(r27 & 1))
			continue;

		if (sud->unk3 != 0)
			return;

		u32 size = JASystem::Vload::checkSize(unk2C + ((*sound)->unk8 & 0x3FF));

		u8 pos;
		u8* ptr = (u8*)unk0->checkOnMemory((*sound)->unk8 & 0x3FF, &pos);

		if (ptr == nullptr) {
			if ((*sound)->checkSwBit(0x10)) {
				ptr = unk0->getFreeStayHeapPointer(size,
				                                   (*sound)->unk8 & 0x3FF);
				pos = 0xFF;
				(*sound)->getSeqParameter()->unk1754 = 0xFF;
				if (ptr == nullptr) {
					(*sound)->checkSwBit(0x20);
				}
			}

			if (ptr == nullptr) {
				if ((*sound)->checkSwBit(0x20) || !(*sound)->checkSwBit(0x10)) {
					pos = unk0->checkUsefulAutoHeapPosition();
					if (pos >= JAIGlobalParameter::autoHeapMax) {
						for (int ii = 0; ii < JAIGlobalParameter::autoHeapMax;
						     ++ii) {
							if (unk0->unk1EC[ii].unk10 == 0xFFFFFFFF)
								continue;

							u32 j;
							for (j = 0; j < JAIGlobalParameter::seqPlayTrackMax;
							     ++j) {
								JAISound* other = unk0->unk180[j].unk48;
								if (other
								    && unk0->unk1EC[ii].unk8
								           == (u8)other->unk8) {
									j = JAIGlobalParameter::seqPlayTrackMax;
								}
							}
							if (j == JAIGlobalParameter::seqPlayTrackMax) {
								unk0->releaseAutoHeapPointer(ii);
								pos = ii;
							}
						}
						if (pos >= JAIGlobalParameter::autoHeapMax) {
							(*sound)->stop(0);
							return;
						}
					} else if (size >= JAIGlobalParameter::autoHeapRoomSize) {
						(*sound)->stop(0);
						return;
					}

					(*sound)->getSeqParameter()->unk1754 = pos;
					JAISound* tmp                        = *sound;
					ptr = (u8*)unk0->getFreeAutoHeapPointer(
					    tmp->getSeqParameter()->unk1754, tmp->unk8 & 0x3FF);
				}
			}

			if (!(*sound)->checkSwBit(0x40)) {
				(*sound)->unk1 = 1;

				u32 swBit8 = (*sound)->unk8;
				u32 param  = (pos << 8) | i | ((swBit8 & 0x3FF) << 16);

				unk0->setAutoHeapLoadedFlag(pos, 1);
				JASystem::Vload::loadFileAsync(unk2C + (swBit8 & 0x3FF), ptr, 0,
				                               size, checkDvdLoadArc, param);
				sud->unk3 = 1;
			} else {
				JASystem::Vload::loadFile(unk2C + ((*sound)->unk8 & 0x3FF), ptr,
				                          0, size);
				(*sound)->unk1 = 2;
			}
		} else {
			if (ptr == (u8*)0xFFFFFFFF)
				return;

			JAISound* snd = *sound;
			if (pos != 0xFF) {
				unk0->getFreeAutoHeapPointer(pos, snd->unk8 & 0x3FF);
			}

			(*sound)->getSeqParameter()->unk1754 = pos;

			(*sound)->unk1 = 2;
		}

		if (ptr != nullptr) {
			sud->unk40 = ptr;
			r27 ^= 1;
		} else {
			stopSeq(*sound);
		}
	}
}

void JAIBasic::checkPlayingSeqTrack(unsigned long trackID)
{
	JAISeqUpdateData* sud = &unk0->unk180[trackID];
	JAISound** sound      = &sud->unk48;
	if ((*sound)->getSeqParameter()->unk1755 == 2)
		return;

	u32& r30 = sud->unk8;

	JAISeqParameter* seqParam = (*sound)->getSeqParameter();
	u32* portFlags            = sud->unk44;

	for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax + 1; ++j)
		portFlags[j] = 0;

	if (r30 & 2) {
		JAISound* snd = (*sound);
		if (snd->unk10 == 0 || snd->unk1 < 4) {
			if (snd->unk1 >= 3) {
				JAISystemInterface::stopSeq(seqParam->unk0);
			}
			(*sound)->clearMainSoundPPointer();
			stopSeq((*sound));
			r30 = 0;
			return;
		} else {
			snd->setSeqInterVolume(6, 0.0f, snd->unk10);
			(*sound)->unk1 = 5;
			r30 ^= 2;
		}
	}

	if ((*sound) != nullptr && (*sound)->unk20 != 0) {
		u32 i;
		u32 s, e;
		if ((*sound)->unk4 == 4) {
			s = 0;
			e = JAIGlobalParameter::audioCameraMax;
		} else {
			s = (*sound)->unk4;
			e = (*sound)->unk4 + 1;
		}

		for (i = s; i < e; ++i) {
			JAISound::FabricatedPositionInfo* pi = &(*sound)->unk1C[i];

			pi->unkC = pi->unk0;
			MTXMultVec(unk8[i].unk8, (Vec*)(*sound)->unk24, &pi->unk0);

			pi->unk18
			    = std::sqrtf(pi->unk0.x * pi->unk0.x + pi->unk0.y * pi->unk0.y
			                 + pi->unk0.z * pi->unk0.z);

			(*sound)->setSeqInterVolume(
			    4,
			    (f32)(u8)(127.0f
			              * (*sound)->setDistanceVolumeCommon(
			                  JAIGlobalParameter::distanceMax, 0)),
			    JAIGlobalParameter::distanceParameterMoveTime);

			(*sound)->setSeqInterPan(
			    4, (f32)(u8)(*sound)->setDistancePanCommon(),
			    JAIGlobalParameter::distanceParameterMoveTime);

			(*sound)->setSeqInterPitch(
			    4, (*sound)->setPositionDopplarCommon(0x100),
			    JAIGlobalParameter::dopplarMoveTime);
		}
	}

	if ((*sound) != nullptr)
		(*sound)->unk14++;

	if (r30 == 0)
		return;

	u8 envMax = JAIGlobalParameter::seqPlayTrackMax + 12;

	if (r30 & 0x40000) {
		f32 vol = 1.0f;
		for (u8 j = 0; j < envMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->unk114[j];
			if (seqParam->unk1760 & (1 << j)) {
				if (!unk0->moveParameter(mps)) {
					seqParam->unk1760 ^= 1 << j;
				}
			}
			vol *= mps->unk4;
		}
		if (sud->unkC != vol) {
			sud->unkC = vol;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->unk180[trackID], JAIGlobalParameter::seqTrackMax, 2,
			    vol);
			portFlags[JAIGlobalParameter::seqTrackMax] |= 1;
		}
		if (seqParam->unk1760 == 0) {
			r30 ^= 0x40000;
		}
	}

	if (r30 & 0x80000) {
		f32 pan = 0.0f;
		for (u8 j = 0; j < envMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->unk254[j];
			if (seqParam->unk1764 & (1 << j)) {
				if (!unk0->moveParameter(mps)) {
					seqParam->unk1764 ^= 1 << j;
				}
			}
			pan += mps->unk4 - 0.5f;
		}
		pan += 0.5f;
		if (pan > 1.0f)
			pan = 1.0f;
		else if (pan < 0.0f)
			pan = 0.0f;
		if (sud->unk18 != pan) {
			sud->unk18 = pan;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->unk180[trackID], JAIGlobalParameter::seqTrackMax, 4,
			    pan);
			portFlags[JAIGlobalParameter::seqTrackMax] |= 4;
		}
		if (seqParam->unk1764 == 0) {
			r30 ^= 0x80000;
		}
	}

	if (r30 & 0x100000) {
		f32 pitch = 1.0f;
		for (u8 j = 0; j < envMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->unk394[j];
			if (seqParam->unk1768 & (1 << j)) {
				if (!unk0->moveParameter(mps)) {
					seqParam->unk1768 ^= 1 << j;
				}
			}
			pitch *= mps->unk4;
		}
		if (sud->unk10 != pitch) {
			sud->unk10 = pitch;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->unk180[trackID], JAIGlobalParameter::seqTrackMax, 3,
			    pitch);
			portFlags[JAIGlobalParameter::seqTrackMax] |= 2;
		}
		if (seqParam->unk1768 == 0) {
			r30 ^= 0x100000;
		}
	}

	if (r30 & 0x200000) {
		f32 fxmix = 0.0f;
		for (u8 j = 0; j < envMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->unk4D4[j];
			if (seqParam->unk176C & (1 << j)) {
				if (!unk0->moveParameter(mps)) {
					seqParam->unk176C ^= 1 << j;
				}
				fxmix += mps->unk4;
			}
		}
		if (fxmix > 1.0f)
			fxmix = 1.0f;
		if (sud->unk14 != fxmix) {
			sud->unk14 = fxmix;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->unk180[trackID], JAIGlobalParameter::seqTrackMax, 5,
			    fxmix);
			portFlags[JAIGlobalParameter::seqTrackMax] |= 8;
		}
		if (seqParam->unk176C == 0) {
			r30 ^= 0x200000;
		}
	}

	if (r30 & 0x400000) {
		f32 dolby = 1.0f;
		for (u8 j = 0; j < envMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->unk614[j];
			if (seqParam->unk1770 & (1 << j)) {
				if (!unk0->moveParameter(mps)) {
					seqParam->unk1770 ^= 1 << j;
				}
				dolby *= mps->unk4;
			}
		}
		if (sud->unk1C != dolby) {
			sud->unk1C = dolby;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->unk180[trackID], JAIGlobalParameter::seqTrackMax, 6,
			    dolby);
			portFlags[JAIGlobalParameter::seqTrackMax] |= 0x10;
		}
		if (seqParam->unk1770 == 0) {
			r30 ^= 0x400000;
		}
	}

	if (r30 & 4) {
		if (!unk0->moveParameter(&seqParam->unk4)) {
			r30 ^= 4;
		}
		if (sud->unk20 != seqParam->unk4.unk4) {
			sud->unk20 = seqParam->unk4.unk4;
			JAISystemInterface::setSeqPortargsF32(
			    &unk0->unk180[trackID], JAIGlobalParameter::seqTrackMax, 9,
			    seqParam->unk4.unk4);
			portFlags[JAIGlobalParameter::seqTrackMax] |= 0x80;
		}
	}

	if (r30 & 0x10) {
		for (u8 j = 0; j < 16; ++j) {
			if (seqParam->unk175C & (1 << j))
				if (!unk0->moveParameter(&seqParam->unk14[j]))
					seqParam->unk175C ^= (1 << j);

			u16 readVal;
			JAISystemInterface::readPortApp(seqParam->unk0, j << 16, &readVal);
			if (readVal != (u16)seqParam->unk14[j].unk4)
				JAISystemInterface::writePortApp(seqParam->unk0, j << 16,
				                                 seqParam->unk14[j].unk4);
		}
		if (seqParam->unk175C == 0)
			r30 ^= 0x10;
	}

	if (r30 & 0x40) {
		for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->unk754[j];
			if (seqParam->unk1774 & (1 << j)) {
				if (!unk0->moveParameter(mps))
					seqParam->unk1774 ^= 1 << j;

				if (mps->unk4 != sud->unk24[j]) {
					sud->unk24[j] = mps->unk4;
					portFlags[j] |= 1;
					JAISystemInterface::setSeqPortargsF32(
					    &unk0->unk180[trackID], j, 2, mps->unk4);
				}
			}
		}
		if (seqParam->unk1774 == 0)
			r30 ^= 0x40;
	}

	if (r30 & 0x20) {
		r30 ^= 0x20;
		for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax; ++j) {
			if (seqParam->unk1830[j].flag3 == 1
			    && seqParam->unk1830[j].flag1 != seqParam->unk1830[j].flag2) {
				JASystem::TTrack* tt
				    = JAISystemInterface::trackToSeqp((*sound), j);
				if (tt != nullptr)
					tt->muteTrack(seqParam->unk1830[j].flag2);
				seqParam->unk1830[j].flag1 = seqParam->unk1830[j].flag2;
			}
		}
	}

	if (r30 & 0x80) {
		for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->unk954[j];
			if (seqParam->unk1778 & (1 << j)) {
				if (!unk0->moveParameter(mps))
					seqParam->unk1778 ^= 1 << j;

				if (mps->unk4 != sud->unk30[j]) {
					sud->unk30[j] = mps->unk4;
					portFlags[j] |= 4;
					JAISystemInterface::setSeqPortargsF32(
					    &unk0->unk180[trackID], j, 4, seqParam->unk954[j].unk4);
				}
			}
		}
		if (seqParam->unk1778 == 0)
			r30 ^= 0x80;
	}

	if (r30 & 0x200) {
		for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->unkB54[j];
			if (seqParam->unk177C & (1 << j)) {
				if (!unk0->moveParameter(mps)) {
					seqParam->unk177C ^= 1 << j;
				}
				if (mps->unk4 != sud->unk28[j]) {
					sud->unk28[j] = mps->unk4;
					portFlags[j] |= 2;
					JAISystemInterface::setSeqPortargsF32(
					    &unk0->unk180[trackID], j, 3, seqParam->unkB54[j].unk4);
				}
			}
		}
		if (seqParam->unk177C == 0)
			r30 ^= 0x200;
	}

	if (r30 & 0x800) {
		for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->unkD54[j];
			if (seqParam->unk1780 & (1 << j)) {
				if (!unk0->moveParameter(mps)) {
					seqParam->unk1780 ^= 1 << j;
				}
				if (mps->unk4 != sud->unk2C[j]) {
					sud->unk2C[j] = mps->unk4;
					portFlags[j] |= 8;
					JAISystemInterface::setSeqPortargsF32(
					    &unk0->unk180[trackID], j, 5, seqParam->unkD54[j].unk4);
				}
			}
		}
		if (seqParam->unk1780 == 0)
			r30 ^= 0x800;
	}

	if (r30 & 0x100) {
		for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax; ++j) {
			JAIMoveParaSet* mps = &seqParam->unkF54[j];
			if (seqParam->unk1784 & (1 << j)) {
				if (!unk0->moveParameter(mps)) {
					seqParam->unk1784 ^= 1 << j;
				}
				if (mps->unk4 != sud->unk34[j]) {
					sud->unk34[j] = mps->unk4;
					portFlags[j] |= 0x10;
					JAISystemInterface::setSeqPortargsF32(
					    &unk0->unk180[trackID], j, 6, seqParam->unkF54[j].unk4);
				}
			}
		}
		if (seqParam->unk1784 == 0)
			r30 ^= 0x100;
	}

	if (r30 & 0x800000) {
		r30 ^= 0x800000;
		for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax; ++j) {
			u8* slot = &seqParam->unk1810[j];
			if (*slot == 1) {
				portFlags[j] |= 0x40;
				JAISystemInterface::setSeqPortargsU32(&unk0->unk180[trackID], j,
				                                      8, 1);
				*slot = 0;
			}
		}
	}

	if (r30 & 0x1000) {
		for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax; ++j) {
			if (seqParam->unk178C & (1 << j)) {
				seqParam->unk178C ^= 1 << j;
				for (u8 k = 0; k < 16; ++k) {
					if (seqParam->unk1790[j] & (1 << k)) {
						JAISystemInterface::writePortApp(
						    seqParam->unk0, (*sound)->getTrackPortRoute(j, k),
						    seqParam->unk1354[j][k]);
						seqParam->unk1790[j] ^= 1 << k;
					}
				}
			}
		}
		if (seqParam->unk178C == 0)
			r30 ^= 0x1000;
	}
}

void JAIBasic::checkPlayingSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISound** sound = &unk0->unk180[i].unk48;
		if (*sound && (*sound)->unk1 >= 4) {
			checkPlayingSeqTrack(i);
			for (u8 j = 0; j < JAIGlobalParameter::seqTrackMax + 1; ++j) {
				if (unk0->unk180[i].unk44[j] != 0) {
					JAISystemInterface::setSeqPortargsU32(
					    &unk0->unk180[i], j, 1, unk0->unk180[i].unk44[j]);

					unk0->unk180[i].unk4C[j].unk2C.unk0 = nullptr;
					unk0->unk180[i].unk4C[j].unk2C.addPortCmdOnce();
				}
			}
		}
	}
}

void JAIBasic::checkStoppedSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISound** sound = &unk0->unk180[i].unk48;

		if (!*sound)
			continue;

		if ((*sound)->unk1 == 4 || (*sound)->unk1 == 5) {
			u8 flag = JAISystemInterface::checkSeqActiveFlag(
			    (*sound)->getSeqParameter()->unk0);
			if (flag == 0) {
				(*sound)->clearMainSoundPPointer();
				stopSeq(*sound);
				unk0->unk180[i].unk8 = 0;
			}
		}
	}
}

void JAIBasic::checkStartedSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISound** sound = &unk0->unk180[i].unk48;

		if (!*sound)
			continue;

		if ((*sound)->unk1 == 3) {
			u8 flag = JAISystemInterface::checkSeqActiveFlag(
			    (*sound)->getSeqParameter()->unk0);
			if (flag != 0) {
				(*sound)->unk1 = 4;
				JAISystemInterface::trackInit(&unk0->unk180[i]);
			}
		}
	}
}

void JAIBasic::checkFadeoutSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISound** sound = &unk0->unk180[i].unk48;

		if (!*sound)
			continue;

		if ((*sound)->unk1 == 5 && (*sound)->getSeqInterVolume(6) == 0.0f) {
			JAISystemInterface::stopSeq((*sound)->getSeqParameter()->unk0);
			(*sound)->clearMainSoundPPointer();
			stopSeq(*sound);
			unk0->unk180[i].unk8 = 0;
		}
	}
}

void JAIBasic::checkReadSeq()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISeqUpdateData* sud = &unk0->unk180[i];
		JAISound** sound      = &sud->unk48;
		if (!*sound)
			continue;
		if ((*sound)->unk1 != 2)
			continue;
		if ((*sound)->getSeqParameter()->unk1758 != -1)
			continue;
		if ((*sound)->getSeqParameter()->unk1850->unk2 != 0)
			continue;

		u32 lVar2
		    = JASystem::Vload::checkSize(unk2C + ((*sound)->unk8 & 0x3FF));
		int uVar3 = JAISystemInterface::setSeqData(
		    nullptr, sud->unk40, lVar2, JASystem::Player::SEQ_PLAYMODE_UNK_0);

		(*sound)->getSeqParameter()->unk0 = uVar3;
		(*sound)->getSeqParameter();
		if ((*sound)->getSeqParameter()->unk0 != -1) {
			unk0->initSeqTrackInfoParameter((*sound)->unk0);
			(*sound)->unk1 = 3;
			if ((*sound)->unk10 > 1) {
				(*sound)->setSeqInterVolume(6, 0.0f, 0);
				(*sound)->setSeqInterVolume(6, 1.0f, (*sound)->unk10);
			}
			if (sud->unk0 != 0) {
				(*sound)->setPauseMode(sud->unk0, sud->unk1);
				sud->unkC = 1.1f;
			}
			setSeExtParameter(*sound);
			checkPlayingSeqTrack(i);
			if (*sound != nullptr) {
				JAISystemInterface::rootInit(sud);
				JAISystemInterface::startSeq((*sound)->getSeqParameter()->unk0);
			}
		} else {
			JAISound* snd = *sound;
			stopSeq(snd);
		}
	}
}

void JAIBasic::checkSeqWave()
{
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISeqUpdateData* sud = &unk0->unk180[i];
		JAISound** sound      = &sud->unk48;
		if (!*sound)
			continue;
		if (!(*sound)->getSeqParameter())
			continue;
		if ((*sound)->getSeqParameter()->unk1758 == 0xffffffff)
			continue;
		if (unk34 == 0xffffffff)
			continue;

		u32 uVar3 = (*sound)->getSeqParameter()->unk1758;

		JAISound* snd = *sound;
		if (unk34 == uVar3 || uVar3 == 0xff00ff00
		    || (((unk34 & 0xffff0000) == (uVar3 & 0xffff0000)
		         && uVar3 == 0xffff))
		    || (((unk34 & 0xffff) == (uVar3 & 0xffff)
		         && uVar3 == 0xffff0000))) {
			snd->getSeqParameter()->unk1758 = 0xffffffff;
		}
	}
}

void JAIBasic::checkDvdLoadArc(u32 param_1)
{
	u8 hi   = param_1 >> 8;
	u8 lo   = param_1 & 0xff;
	u16 hi2 = ((param_1 >> 16) & 0x3FF);
	if (hi != 0xff)
		basic->unk0->setAutoHeapLoadedFlag(hi, 0);

	if (lo < 0xFE) {
		JAISound* sound              = basic->unk0->unk180[lo].unk48;
		basic->unk0->unk180[lo].unk3 = 0;
		if (sound && sound->unk1 == 1 && hi2 == (sound->unk8 & 0x3FF))
			sound->unk1 = 2;
		else
			basic->unk0->releaseAutoHeapPointer(hi);
	} else if (lo == 0xFE) {
		basic->unk0->releaseAutoHeapPointer(hi);
	}
}
