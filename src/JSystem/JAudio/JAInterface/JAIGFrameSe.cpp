#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>
#include <math.h>

void JAIBasic::checkNextFrameSe()
{
	// TODO: gl matching this awfulness

	JAISound sound;

	struct Candidate {
		/* 0x0 */ u8 state;
		/* 0x4 */ u32 score;
		/* 0x8 */ JAISound* sound;
	};
	Candidate candidates[16];
	int _stackPad;

	f32 fVar6
	    = JAIGlobalParameter::distanceMax * JAIGlobalParameter::distanceMax;
	f32 fVar1 = JAIGlobalParameter::distanceMax / 1000.0f;
	if (fVar1 == 0.0f)
		fVar1 = 1.0f;

	for (int i = 0; i < JAIGlobalParameter::getParamSeCategoryMax(); ++i) {
		for (u8 j = 0; j < unk0->unk4[unk10][i * 2]; ++j) {
			candidates[j].score = 0x7fffffff;
			candidates[j].sound = nullptr;
			candidates[j].state = 0xff;
		}

		u8 bVar19 = 0;

		JAISound* it = unk0->unk1E8[i].unk4;
		while (it) {
			if (it->unk1 == 1 && (it->unk8 & 0xC00)) {
				--it->unk2;
			} else if (!(it->unk8 & 0xC00) && it->unk1 == 5) {
				sound.unk30 = it->unk30;
				releaseSeRegist(it);
				it = &sound;
			}

			if (it->unk2 == 0) {
				sound.unk30 = it->unk30;
				releaseSeRegist(it);
				it = &sound;
			} else if (it->unk1 != 0) {
				f32 fVar2 = 2.147484e+09f;
				u8 uVar14;
				u8 uVar8;
				if (it->unk4 == 4) {
					uVar14 = 0;
					uVar8  = JAIGlobalParameter::audioCameraMax;
				} else {
					uVar8  = it->unk4 + 1;
					uVar14 = it->unk4;
				}

				for (; uVar14 < uVar8; ++uVar14) {
					JAISound::FabricatedPositionInfo* pi = &it->unk1C[uVar14];

					pi->unkC = pi->unk0;
					if (it->unk24 == 0) {
						pi->unk0 = JAIConst::dummyZeroVec;
					} else {
						MTXMultVec(unk8[uVar14].unk8, (Vec*)it->unk24,
						           &pi->unk0);
					}

					pi->unk18 = pi->unk0.x * pi->unk0.x
					            + pi->unk0.y * pi->unk0.y
					            + pi->unk0.z * pi->unk0.z;
					s16 prio = it->getInfoPriority();
					if (it->unk6) {
						prio += it->unk6;
						if (prio < 0)
							prio = 0;
						else if (prio > 0xff)
							prio = 0xff;
					}

					it->unkC
					    = (u32)((0xff - prio) * (0xff - prio) * 0x1690 / fVar1)
					      + (u32)(pi->unk18 / fVar1);
					if (pi->unk0.z > 0.0f)
						it->unkC += (u32)(pi->unk0.z * 6.0f / fVar1);

					if (uVar14 == 0 || pi->unk18 < fVar2)
						fVar2 = pi->unk18;
				}

				if (it->unk4 == 4)
					it->unkC /= JAIGlobalParameter::audioCameraMax;

				f32 fVar3;
				if (it->getSwBit() & 0x20)
					fVar3 = fVar6;
				else
					fVar3 = 1e+10f;

				if (fVar2 > fVar3) {
					if (!(it->unk8 & 0xC00)) {
						if (it->unk1 != 1) {
							JAISystemInterface::writePortApp(
							    unk38->getSeqParameter()->unk0,
							    (it->unk0 >> 4) + 0x20000000
							        + ((it->unk0 & 0xf) << 4),
							    0);
							unk38->setTrackInterruptSwitch(it->unk0, 1);
						}
						it->unk1 = 1;
					} else {
						sound.unk30 = it->unk30;
						stopSoundHandle(it, 0);
						it = &sound;
					}
				} else {
					u8 bVar18
					    = unk0->unk4[unk10][(u8)it->getSeCategoryNumber() * 2];
					for (u8 j = 0; j < bVar18; ++j) {
						if (it->unkC < candidates[j].score
						    || (it->unkC == candidates[j].score
						        && candidates[j].state >= it->unk1)) {
							if (bVar19 < bVar18)
								++bVar19;
							for (u8 k = bVar18 - 1; k > j; --k) {
								candidates[k].score = candidates[k - 1].score;
								candidates[k].sound = candidates[k - 1].sound;
								candidates[j].state = candidates[k - 1].state;
							}
							candidates[j].score = it->unkC;
							candidates[j].sound = it;
							candidates[j].state = it->unk1;

							j = bVar18;
						}
					}
				}
			}
			if (it != nullptr)
				it = it->unk30;
		}

		for (u8 k = 0; k < bVar19; ++k) {
			JAISound* snd = candidates[k].sound;
			if (snd->unk1 == 1) {
				snd->unk1 = 2;
			} else if (snd->unk1 == 4) {
				snd->unk1 = 3;
			}
		}

		u8 bVar192 = unk0->unk4[unk10][i * 2];
		for (u8 j = 0; j < bVar192; ++j) {
			JAISound** cur = &unk0->unk8[i][j].unk8;
			JAISound* snd  = *cur;
			u8 bVar7       = 0;
			if (snd == nullptr) {
				bVar7 = 1;
			} else if (snd->unk1 == 4) {
				if (snd->unk8 & 0xC00) {
					releaseSeRegist(snd);
				} else {
					snd->unk1 = 1;
					snd->unk2 = 0;
				}
				bVar7 = 1;
			} else if (snd->unk1 == 0) {
				*cur  = nullptr;
				bVar7 = 1;
			} else {
				for (u8 k = 0; k < bVar192; ++k) {
					if (unk0->unk8[i][j].unk8 == candidates[k].sound) {
						candidates[k].sound = nullptr;
						k                   = bVar192;
					}
				}
			}

			if (bVar7 == 1) {
				u8 k;
				for (k = 0; k < bVar192; ++k) {
					JAISound* snd = candidates[k].sound;
					if (snd != nullptr && snd->unk1 != 3) {
						for (u8 l = 0; l < bVar192; ++l) {
							if (unk0->unk8[i][l].unk8
							    && snd == unk0->unk8[i][l].unk8) {
								bVar7 = 0;
								l     = bVar192;
							}
						}

						if (bVar7 == 1) {
							unk0->unk8[i][j].unk8 = snd;
							candidates[k].sound   = nullptr;
							k                     = bVar192 + 1;
						}
					}
				}
				if (k == bVar192) {
					unk0->unk8[i][j].unk8 = nullptr;
				}
			}
		}
	}
}

void JAIBasic::sendPlayingSeCommand()
{
	u16 readStatus0;
	u16 readStatus1;
	u8 trackId = 0;
	// char _stackPad[8];

	for (u8 cat = 0; cat < JAIGlobalParameter::getParamSeCategoryMax(); ++cat) {
		for (u8 j = 0; j < unk0->unk4[unk10][(u8)cat * 2]; ++j, ++trackId) {
			JAISound* sound = unk0->unk8[cat][j].unk8;
			if (sound == nullptr)
				continue;

			sound->unk14++;

			u32 portAddr
			    = ((trackId >> 4) & 0xF) + 0x20000000 + ((trackId & 0xF) << 4);

			u32 seqPort = unk38->getSeqParameter()->unk0;

			JAISystemInterface::readPortApp(seqPort, portAddr + 0x20000,
			                                &readStatus0);
			JAISystemInterface::readPortApp(seqPort, portAddr, &readStatus1);

			JAISound::FabricatedPositionInfo* infos = sound->unk1C;
			for (u8 k = 0; k < JAIGlobalParameter::audioCameraMax; ++k) {
				f32* dPtr = &infos[k].unk18;
				*dPtr     = std::sqrtf(*dPtr);
			}

			u8 state = sound->unk1;
			if (state == 2) {
				u32 swBit   = sound->getSwBit();
				sound->unk0 = trackId;
				if (swBit & 8) {
					setSeqMuteFromSeStart(sound);
				}
				if (swBit & 0xC0) {
					s32 pitchVal
					    = (s32)(255.0f * JAIConst::random.get_ufloat_1());
					switch (swBit & 0xC0) {
					case 0x40:
						sound->unk3 = pitchVal & 0xF;
						break;
					case 0x80:
						sound->unk3 = pitchVal & 0x1F;
						break;
					case 0xC0:
						sound->unk3 = pitchVal & 0x3F;
						break;
					default:
						sound->unk3 = 0;
						break;
					}
				}

				u16* portMask = &sound->getSeParameter()->unk20;
				for (u8 i = 0; *portMask != 0; ++i) {
					u32 bit = 1 << i;
					if (*portMask & bit) {
						unk38->setTrackPortData(
						    sound->unk0, i, sound->getSeParameter()->unk0[i]);
						*portMask ^= bit;
					}
				}

				sound->setSeDistanceParameters();
				setSeExtParameter(sound);

				if (sound->unk10 > 1) {
					sound->setSeInterVolume(6, 0.0f, 0, 0);
					sound->setSeInterVolume(6, 127.0f, sound->unk10, 0);
					sound->unk10 = 0;
				}

				sendSeAllParameter(sound);

				u16 portValue = sound->unk8 & 0x3FF;
				if (sound->checkSwBit(0x800)) {
					u32 tmp = sound->unk18;
					portValue += getMapInfoGround(tmp);
				}

				u16 distArg;
				if (JAIGlobalParameter::audioCameraMax == 1
				    && sound->checkSwBit(0x1000)) {
					if (sound->unk1C[0].unk18
					    < JAIGlobalParameter::distanceMax) {
						distArg = JAIGlobalParameter::seDistanceWaitMax
						          * (u32)sound->unk1C[0].unk18
						          / (u32)JAIGlobalParameter::distanceMax;
					} else {
						distArg = JAIGlobalParameter::seDistanceWaitMax;
					}
				} else {
					distArg = 0;
				}

				JAISystemInterface::writePortApp(seqPort, portAddr + 0x30000,
				                                 distArg);
				JAISystemInterface::writePortApp(
				    seqPort, portAddr + 0x60000,
				    getMapInfoFxline(sound->unk18));
				JAISystemInterface::writePortApp(seqPort, portAddr + 0x40000,
				                                 portValue);
				JAISystemInterface::writePortApp(seqPort, portAddr, 1);

				if (sound->unk8 & 0xC00) {
					sound->unk1 = 4;
				} else {
					sound->unk1 = 5;
				}
			} else if (readStatus0 == 0 && readStatus1 != 1) {
				releaseSeRegist(sound);
			} else if (sound->unk10 != 0) {
				if (sound->getSeParameter()->unk124[6].unk4 != 0.0f) {
					sound->setSeDistanceParameters();
					sendSeAllParameter(sound);
					if (sound->unk8 & 0xC00) {
						sound->unk1 = 4;
					} else {
						sound->unk1 = 5;
					}
				} else {
					releaseSeRegist(sound);
				}
			} else if (state == 3) {
				sound->setSeDistanceParameters();
				sendSeAllParameter(sound);
				if (sound->unk8 & 0xC00) {
					sound->unk1 = 4;
				} else {
					sound->unk1 = 5;
				}
			}
		}
	}
}

void JAIBasic::setSeqMuteFromSeStart(JAISound* param_1)
{
	for (u32 i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISound* sound = unk0->unk180[i].unk48;
		if (i != unk38->unk0 && sound && !(sound->getSwBit() & 8)) {
			sound->setSeqInterVolume(
			    9, JAIGlobalParameter::seqMuteVolumeSePlay / 127.0f,
			    JAIGlobalParameter::seqMuteMoveSpeedSePlay);
			unk30 |= 1 << param_1->unk0;
		}
	}
}

void JAIBasic::clearSeqMuteFromSeStop(JAISound* sound)
{
	for (u32 i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		JAISound* seq = unk0->unk180[i].unk48;
		if (i != unk38->unk0 && seq && !(seq->getSwBit() & 8)) {
			seq->setSeqInterVolume(
			    9, JAIGlobalParameter::seqMuteVolumeSePlay / 127.0f,
			    JAIGlobalParameter::seqMuteMoveSpeedSePlay);
			unk30 &= ~(1 << sound->unk0);
		}
	}
}

void JAIBasic::checkSeMovePara()
{
	if (!unk38 || unk38->getSeqParameter()->unk1755 == 2)
		return;

	for (u8 i = 0; i < JAIGlobalParameter::getParamSeCategoryMax(); ++i) {
		for (JAISound* it = unk0->unk1E8[i].unk4; it != nullptr;
		     it           = it->unk30) {
			unk0->setSeMovePara(it->getSeParameter()->unk124);
			unk0->setSeMovePara(it->getSeParameter()->unk1A4);
			unk0->setSeMovePara(it->getSeParameter()->unk2A4);
			unk0->setSeMovePara(it->getSeParameter()->unk324);
			unk0->setSeMovePara(it->getSeParameter()->unk3A4);
			unk0->setSeMovePara(it->getSeParameter()->unk224);
		}
	}
}

void JAIBasic::sendSeAllParameter(JAISound* sound)
{
	// char _stackPad[0x30];

	JAIData::FabricatedUnk0Struct* slot = &unk0->unk0[sound->unk0];
	JAISeParameter* seParam             = sound->getSeParameter();

	// Volume
	f32 vol;
	if (seParam->unk124[7].unk4 == -1.0f) {
		if (seParam->unk424 != 0) {
			seParam->unk124[0].unk4 = *(f32*)seParam->unk424;
		}
		vol = 1.0f;
		for (int i = 0; i < 7; ++i)
			vol *= seParam->unk124[i].unk4;
	} else {
		vol = seParam->unk124[7].unk4;
	}
	vol *= unk28[(u8)sound->getSeCategoryNumber()];
	if (slot->unk4 != vol) {
		slot->unk4 = vol;
		if (sound->unk1 != 2) {
			unk0->unk180[unk38->unk0].unk44[sound->unk0] |= 0x1;
			JAISystemInterface::setSeqPortargsF32(&unk0->unk180[unk38->unk0],
			                                      sound->unk0, 2, vol);
		}
	}

	// Pan
	f32 pan;
	if (seParam->unk1A4[7].unk4 == -1.0f) {
		if (seParam->unk428 != 0) {
			seParam->unk1A4[0].unk4 = *(f32*)seParam->unk428;
		}
		pan = 0.0f;
		for (int i = 0; i < 7; ++i)
			if (seParam->unk1A4[i].unk4 != 0.5f)
				pan += seParam->unk1A4[i].unk4 - 0.5f;
		pan += 0.5f;
		if (pan < 0.0f)
			pan = 0.0f;
		else if (pan > 1.0f)
			pan = 1.0f;
	} else {
		pan = seParam->unk1A4[7].unk4;
	}
	if (slot->unk10 != pan) {
		slot->unk10 = pan;
		if (sound->unk1 != 2) {
			unk0->unk180[unk38->unk0].unk44[sound->unk0] |= 0x4;
			JAISystemInterface::setSeqPortargsF32(&unk0->unk180[unk38->unk0],
			                                      sound->unk0, 4, pan);
		}
	}

	// Pitch
	f32 pitch;
	if (seParam->unk224[7].unk4 == -1.0f) {
		if (seParam->unk42C != 0) {
			seParam->unk224[0].unk4 = *(f32*)seParam->unk42C;
		}
		pitch = 1.0f;
		for (int i = 0; i < 7; ++i)
			pitch *= seParam->unk224[i].unk4;
	} else {
		pitch = seParam->unk224[7].unk4;
	}
	if (slot->unk8 != pitch) {
		slot->unk8 = pitch;
		if (sound->unk1 != 2) {
			unk0->unk180[unk38->unk0].unk44[sound->unk0] |= 0x2;
			JAISystemInterface::setSeqPortargsF32(&unk0->unk180[unk38->unk0],
			                                      sound->unk0, 3, pitch);
		}
	}

	// FxMix
	f32 fxmix;
	if (seParam->unk2A4[7].unk4 == -1.0f) {
		if (seParam->unk430 != 0) {
			seParam->unk2A4[0].unk4 = *(f32*)seParam->unk430;
		}
		fxmix = 0.0f;
		fxmix += seParam->unk2A4[0].unk4;
		fxmix += seParam->unk2A4[1].unk4;
		fxmix += seParam->unk2A4[2].unk4;
		fxmix += seParam->unk2A4[3].unk4;
		fxmix += seParam->unk2A4[4].unk4;
		fxmix += seParam->unk2A4[5].unk4;
		fxmix += seParam->unk2A4[6].unk4;
	} else {
		fxmix = seParam->unk2A4[7].unk4;
	}
	if (slot->unkC != fxmix) {
		slot->unkC = fxmix;
		if (sound->unk1 != 2) {
			unk0->unk180[unk38->unk0].unk44[sound->unk0] |= 0x8;
			JAISystemInterface::setSeqPortargsF32(&unk0->unk180[unk38->unk0],
			                                      sound->unk0, 5, fxmix);
		}
	}

	// Dolby
	f32 dolby;
	if (seParam->unk3A4[7].unk4 == -1.0f) {
		if (seParam->unk438 != 0) {
			seParam->unk3A4[0].unk4 = *(f32*)seParam->unk438;
		}
		f32 center = JAIGlobalParameter::seDolbyCenterValue / 127.0f;
		dolby      = 0.0f;
		dolby += seParam->unk3A4[0].unk4 - center;
		dolby += seParam->unk3A4[1].unk4 - center;
		dolby += seParam->unk3A4[2].unk4 - center;
		dolby += seParam->unk3A4[3].unk4 - center;
		dolby += seParam->unk3A4[4].unk4 - center;
		dolby += seParam->unk3A4[5].unk4 - center;
		dolby += seParam->unk3A4[6].unk4 - center;
		dolby += center;
		if (dolby < 0.0f)
			dolby = 0.0f;
		else if (dolby > 1.0f)
			dolby = 1.0f;
	} else {
		dolby = seParam->unk3A4[7].unk4;
	}
	if (slot->unk14 != dolby) {
		slot->unk14 = dolby;
		if (sound->unk1 != 2) {
			unk0->unk180[unk38->unk0].unk44[sound->unk0] |= 0x10;
			JAISystemInterface::setSeqPortargsF32(&unk0->unk180[unk38->unk0],
			                                      sound->unk0, 6, dolby);
		}
	}

	// Final block: U32 param
	if (unk0->unk180[unk38->unk0].unk44[sound->unk0] != 0) {
		JAISystemInterface::setSeqPortargsU32(
		    &unk0->unk180[unk38->unk0], sound->unk0, 1,
		    unk0->unk180[unk38->unk0].unk44[sound->unk0]);
		unk0->unk180[unk38->unk0].unk4C[sound->unk0].unk2C.unk0 = 0;
		unk0->unk180[unk38->unk0].unk4C[sound->unk0].unk2C.addPortCmdOnce();
	}
}

void JAIBasic::releaseSeRegist(JAISound* sound)
{
	if (sound->unk1 != 1) {
		JAISystemInterface::writePortApp(
		    unk38->getSeqParameter()->unk0,
		    (sound->unk0 >> 4) + 0x20000000 + ((sound->unk0 & 0xf) << 4), 0);
		unk38->setTrackInterruptSwitch(sound->unk0, 1);
	}

	if (unk30 != 0 && (sound->getSwBit() & 8)) {
		for (u32 i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
			JAISound* seq = unk0->unk180[i].unk48;
			if (i != unk38->unk0 && seq && !(seq->getSwBit() & 8)) {
				unk30 &= (1 << sound->unk0) ^ 0xffffffff;
				if (unk30 == 0) {
					seq->setSeqInterVolume(
					    9, 1.0f, JAIGlobalParameter::seqMuteMoveSpeedSePlay);
				}
			}
		}
	}

	u8 maxCount = unk0->unk4[unk10][(u8)sound->getSeCategoryNumber() * 2];
	u8 cat      = sound->getSeCategoryNumber();
	for (u8 j = 0; j < maxCount; ++j) {
		JAISound** slot = &unk0->unk8[cat][j].unk8;
		if (*slot == sound) {
			*slot = nullptr;
			j     = maxCount;
		}
	}

	sound->clearMainSoundPPointer();
	sound->unk1 = 0;
	releaseSeParameterPointer(sound->getSeParameter());
	releaseControllerHandle(&unk0->unk1E8[cat], sound);
}
