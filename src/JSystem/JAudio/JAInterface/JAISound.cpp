#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>
#include <JSystem/JAudio/JAInterface/JAIInter.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>
#include <math.h>

JAIBasic* JAISound::interPointer;

JAISound::JAISound()
{
	unk38 = 0;
	unk1  = 0;
	unk2  = 10;
	unk20 = 0;
}

int JAISound::initMoveParameter(JAIMoveParaSet* param_1, f32 param_2,
                                u32 param_3)
{
	if (param_1->unkC == 0 && param_1->unk4 == param_2)
		return 2;

	if (param_1->unkC != 0 && param_1->unk0 == param_2)
		return 2;

	param_1->unk0 = param_2;
	if (param_3 == 0) {
		param_1->unk4 = param_2;
		return 0;
	}

	if (param_3 == 1)
		param_1->unk8 = param_1->unk4 - param_1->unk0;
	else
		param_1->unk8 = (param_1->unk4 - param_1->unk0) / (f32)param_3;
	param_1->unkC = param_3;

	return 1;
}

void JAISound::initMultiMoveParameter(JAIMoveParaSet* param_1, u8 param_2,
                                      u32 param_3, f32 param_4, f32 param_5,
                                      u32 param_6)
{
}

u32 JAISound::getSeCategoryNumber()
{
	return interPointer->changeIDToCategory(unk8);
}

void JAISound::getDataInfoHeader() { }

u32 JAISound::getSwBit() { return interPointer->getSoundSwBit(unk3C); }

u32 JAISound::checkSwBit(u32 bit)
{
	return bit & interPointer->getSoundSwBit(unk3C);
}

u8 JAISound::getInfoPriority() { return interPointer->getSoundPrioity(unk3C); }

void JAISound::clearMainSoundPPointer()
{
	if (unk34 == nullptr)
		return;
	*unk34 = nullptr;
}

void JAISound::release()
{
	*unk34 = nullptr;
	unk34  = nullptr;
}

void JAISound::start(u32) { }

void JAISound::stop(u32 param_1)
{
	interPointer->stopSoundHandle(this, param_1);
}

void JAISound::setVolume(f32 param_1, u32 param_2, u8 param_3)
{
	switch (unk8 & 0xC0000000) {
	case 0x80000000:
		setSeqInterVolume(param_3, param_1, param_2);
		break;
	case 0x00000000:
		setSeInterVolume(param_3, param_1, param_2, 0);
		break;
	case 0xC0000000:
		setStreamInterVolume(param_3, param_1, param_2);
		break;
	}
}

void JAISound::setDirectVolume(f32, u32) { }

void JAISound::setPan(f32 param_1, u32 param_2, u8 param_3)
{
	switch (unk8 & 0xC0000000) {
	case 0x80000000:
		setSeqInterPan(param_3, param_1, param_2);
		break;
	case 0x00000000:
		setSeInterPan(param_3, param_1, param_2, 0);
		break;
	case 0xC0000000:
		setStreamInterPan(param_3, param_1, param_2);
		break;
	}
}

void JAISound::setDirectPan(f32, u32) { }

void JAISound::setPitch(f32 param_1, u32 param_2, u8 param_3)
{
	switch (unk8 & 0xC0000000) {
	case 0x80000000:
		setSeqInterPitch(param_3, param_1, param_2);
		break;
	case 0x00000000:
		setSeInterPitch(param_3, param_1, param_2, 0.0f);
		break;
	case 0xC0000000:
		setStreamInterPitch(param_3, param_1, param_2);
		break;
	}
}

void JAISound::setDirectPitch(f32, u32) { }

void JAISound::setFxmix(f32 param_1, u32 param_2, u8 param_3)
{
	switch (unk8 & 0xC0000000) {
	case 0x80000000:
		setSeqInterFxmix(param_3, param_1, param_2);
		break;
	case 0x00000000:
		setSeInterFxmix(param_3, param_1, param_2, 0.0f);
		break;
	case 0xC0000000:
		break;
	}
}

void JAISound::setDirectFxmix(f32, u32) { }

void JAISound::setDolby(f32 param_1, u32 param_2, u8 param_3)
{
	switch (unk8 & 0xC0000000) {
	case 0x80000000:
		setSeqInterDolby(param_3, param_1, param_2);
		break;
	case 0x00000000:
		setSeInterDolby(param_3, param_1, param_2, 0.0f);
		break;
	case 0xC0000000:
		break;
	}
}

void JAISound::setDirectDolby(f32, u32) { }

void JAISound::setTempoProportion(f32 param_1, u32 param_2)
{
	switch (unk8 & 0xC0000000) {
	case 0x80000000:
		setSeqTempoProportion(param_1, param_2);
		break;
	case 0x00000000:
		break;
	case 0xC0000000:
		break;
	}
}

void JAISound::setPortData(u8 param_1, u16 param_2)
{
	switch (unk8 & 0xC0000000) {
	case 0x80000000:
		setSeqPortData(param_1, param_2, 0);
		break;
	case 0x00000000:
		setSePortData(param_1, param_2);
		break;
	case 0xC0000000:
		break;
	}
}

void JAISound::setPrepareFlag(u8) { }

void JAISound::checkReady() { }

f32 JAISound::setDistanceVolumeCommon(f32 param_1, u8 param_2)
{
	f32 var1;
	if (unk4 != 4) {
		var1 = unk1C[unk4].unk18;
	} else {
		var1 = unk1C[0].unk18;
		for (u8 i = 1; i < JAIGlobalParameter::audioCameraMax; i++)
			if (unk1C[i].unk18 < var1)
				var1 = unk1C[i].unk18;
	}

	if (var1 < JAIGlobalParameter::maxVolumeDistance)
		return 1.0f;

	var1 -= JAIGlobalParameter::maxVolumeDistance;
	f32 var2 = param_1 - JAIGlobalParameter::maxVolumeDistance;

	if (param_2 > 3)
		var2 /= 1 << ((param_2 & 3) + 1);
	else if (param_2 != 0)
		var2 *= 1 << (param_2 & 3);

	if (var1 < var2) {
		if (param_2 > 3)
			return 1.0f - var1 / var2;
		else
			return JAIGlobalParameter::minDistanceVolume
			       + (1.0f - JAIGlobalParameter::minDistanceVolume)
			             * (1.0f - var1 / var2);

	} else {
		if (param_2 > 3)
			return 0.0f;
		else
			return JAIGlobalParameter::minDistanceVolume;
	}
}

f32 JAISound::setDistancePanCommon()
{
	if (JAIGlobalParameter::audioCameraMax == 1) {
		FabricatedPositionInfo& info = unk1C[0];

		f32 fVar3 = std::fabsf(info.unk0.x);
		f32 fVar2 = std::fabsf(info.unk0.z);

		if (fVar3 < 1.0f && fVar2 < 1.0f)
			return 0.5f;

		if (JAIGlobalParameter::panDistanceMax < fVar3)
			fVar3 = JAIGlobalParameter::panDistanceMax;

		if (JAIGlobalParameter::panDistanceMax < fVar2)
			fVar2 = JAIGlobalParameter::panDistanceMax;

		f32 result;

		if (info.unk0.x == 0.0f && info.unk0.z == 0.0f) {
			result = 0.5f;
		} else if (info.unk0.x > 0.0f && fVar3 >= fVar2) {
			result = 1.0f
			         - (JAIGlobalParameter::panDistance2Max - fVar3)
			               / (JAIGlobalParameter::panAngleParameter
			                  * (JAIGlobalParameter::panDistance2Max - fVar2));
		} else if (info.unk0.x <= 0.0f && fVar3 >= fVar2) {
			result = (JAIGlobalParameter::panDistance2Max - fVar3)
			         / (JAIGlobalParameter::panAngleParameter
			            * (JAIGlobalParameter::panDistance2Max - fVar2));
		} else {
			result
			    = info.unk0.x / (JAIGlobalParameter::panAngleParameter2 * fVar2)
			      + 0.5f;
		}

		return result;
	} else {
		if (unk4 != 4) {
			return unk4 & 1;
		} else {
			return 0.5f;
		}
	}
}

f32 JAISound::setPositionDopplarCommon(u32 param_1)
{
	VecPtr pVVar7 = interPointer->unk8->unk0;
	VecPtr pVVar6 = interPointer->unk8->unk4;

	f32 diff_x = pVVar7->x - unk1C->unk0.x;
	f32 diff_y = pVVar7->y - unk1C->unk0.y;
	f32 diff_z = pVVar7->z - unk1C->unk0.z;

	f32 diff2_x = (pVVar7->x - pVVar6->x) - (unk1C->unk0.x - unk1C->unkC.x);
	f32 diff2_y = (pVVar7->y - pVVar6->y) - (unk1C->unk0.y - unk1C->unkC.y);
	f32 diff2_z = (pVVar7->z - pVVar6->z) - (unk1C->unk0.z - unk1C->unkC.z);

	f32 lenSq = diff_x * diff_x + diff_y * diff_y + diff_z * diff_z;
	f32 len   = std::sqrtf(lenSq);

	f32 diff3_x = diff_x + diff2_x;
	f32 diff3_y = diff_y + diff2_y;
	f32 diff3_z = diff_z + diff2_z;

	f32 lenSq2 = diff3_x * diff3_x + diff3_y * diff3_y + diff3_z * diff3_z;
	f32 len2   = std::sqrtf(lenSq2);

	f32 result = 1.0f
	             / (1.0f
	                - (len - len2)
	                      / (JAIGlobalParameter::dopplarParameter
	                         / ((param_1 >> 8) * (param_1 >> 8))));

	if (result < 0.1f)
		return 0.1f;
	if (result > 2.0f)
		return 2.0f;
	return result;
}

void JAISound::setSeqInterVolume(u8 param_1, f32 param_2, u32 param_3)
{
	if ((unk8 & 0xC0000000) != 0x80000000)
		return;

	if (!getSeqParameter())
		return;

	int ret = initMoveParameter(&getSeqParameter()->unk114[param_1], param_2,
	                            param_3);
	if (ret == 1)
		getSeqParameter()->unk1760 |= 1 << param_1;

	if (getSeqParameter()->unk1850 && ret != 2)
		getSeqParameter()->unk1850->unk8 |= 0x40000;
}

void JAISound::setSeqInterPan(u8 param_1, f32 param_2, u32 param_3)
{
	if ((unk8 & 0xC0000000) != 0x80000000)
		return;

	if (!getSeqParameter())
		return;

	int ret = initMoveParameter(&getSeqParameter()->unk254[param_1], param_2,
	                            param_3);
	if (ret == 1)
		getSeqParameter()->unk1764 |= 1 << param_1;

	if (getSeqParameter()->unk1850 && ret != 2)
		getSeqParameter()->unk1850->unk8 |= 0x80000;
}

void JAISound::setSeqInterPitch(u8 param_1, f32 param_2, u32 param_3)
{
	if ((unk8 & 0xC0000000) != 0x80000000)
		return;

	if (!getSeqParameter())
		return;

	int ret = initMoveParameter(&getSeqParameter()->unk394[param_1], param_2,
	                            param_3);
	if (ret == 1)
		getSeqParameter()->unk1768 |= 1 << param_1;

	if (getSeqParameter()->unk1850 && ret != 2)
		getSeqParameter()->unk1850->unk8 |= 0x100000;
}

void JAISound::setSeqInterFxmix(u8 param_1, f32 param_2, u32 param_3)
{
	if ((unk8 & 0xC0000000) != 0x80000000)
		return;

	if (!getSeqParameter())
		return;

	int ret = initMoveParameter(&getSeqParameter()->unk4D4[param_1], param_2,
	                            param_3);
	if (ret == 1)
		getSeqParameter()->unk176C |= 1 << param_1;

	if (getSeqParameter()->unk1850 && ret != 2)
		getSeqParameter()->unk1850->unk8 |= 0x200000;
}

void JAISound::setSeqInterDolby(u8 param_1, f32 param_2, u32 param_3)
{
	if ((unk8 & 0xC0000000) != 0x80000000)
		return;

	if (!getSeqParameter())
		return;

	if (interPointer->unk14 != 2) {
		if (getSeqParameter()->unk614[param_1].unk4 != 0.0f)
			param_2 = 0.0f;
		else
			return;
	}

	int ret = initMoveParameter(&getSeqParameter()->unk614[param_1], param_2,
	                            param_3);
	if (ret == 1)
		getSeqParameter()->unk1770 |= 1 << param_1;

	if (getSeqParameter()->unk1850 && ret != 2)
		getSeqParameter()->unk1850->unk8 |= 0x400000;
}

void JAISound::setSeqTempoProportion(f32 param_1, u32 param_2)
{

	if ((unk8 & 0xC0000000) != 0x80000000)
		return;

	if (!getSeqParameter())
		return;

	int ret = initMoveParameter(&getSeqParameter()->unk4, param_1, param_2);

	if (getSeqParameter()->unk1850)
		getSeqParameter()->unk1850->unk8 |= 0x4;
}

void JAISound::setSeqPortData(u8 param_1, u16 param_2, u32 param_3)
{
	if ((unk8 & 0xC0000000) != 0x80000000)
		return;

	if (!getSeqParameter())
		return;

	if (getSeqParameter()->unk14[param_1].unk4 == 0.0f && unk1 >= 3) {
		u16 local_38;
		JAISystemInterface::readPortApp(getSeqParameter()->unk0, param_1 << 16,
		                                &local_38);
		getSeqParameter()->unk14[param_1].unk4 = local_38;
	}

	int ret = initMoveParameter(&getSeqParameter()->unk14[param_1], param_2,
	                            param_3);
	if (ret == 1)
		getSeqParameter()->unk175C |= 1 << param_1;

	if (getSeqParameter()->unk1850 && ret != 2)
		getSeqParameter()->unk1850->unk8 |= 0x10;
}

void JAISound::setWaveReadMode(s32 param_1, s32 param_2)
{
	if ((unk8 & 0xC0000000) != 0x80000000)
		return;

	if (!getSeqParameter())
		return;

	u32 id = param_1 << 16 | param_2;
	if (interPointer->unk60[param_1] == param_2
	    && interPointer->unk64[param_1] == 1)
		getSeqParameter()->unk1758 = id;
}

void JAISound::setTrackVolume(u8 param_1, f32 param_2, u32 param_3) { }

void JAISound::setTrackVolumeMulti(u8, u32, f32, u32) { }

void JAISound::setTrackMuteSwitch(u8, u8) { }

void JAISound::setTrackMuteSwitchMulti(u32, u8) { }

void JAISound::setTrackInterruptSwitch(u8 param_1, u8 param_2)
{
	if ((unk8 & 0xC0000000) != 0x80000000)
		return;

	if (!getSeqParameter())
		return;

	getSeqParameter()->unk1810[param_1] = param_2;
	if (getSeqParameter()->unk1850)
		getSeqParameter()->unk1850->unk8 |= 0x800000;
}

void JAISound::setTrackPan(u8, f32, u32) { }

void JAISound::setTrackPanMulti(u8, u32, f32, u32) { }

void JAISound::setTrackPitch(u8, f32, u32) { }

void JAISound::setTrackPitchMulti(u8, u32, f32, u32) { }

void JAISound::setTrackFxmix(u8, f32, u32) { }

void JAISound::setTrackFxmixMulti(u8, u32, f32, u32) { }

void JAISound::setTrackDolby(u8, f32, u32) { }

void JAISound::setTrackDolbyMulti(u8, u32, f32, u32) { }

void JAISound::setTrackFirU7(u8, u8, u32) { }

void JAISound::setTrackFirMultiU7(u8, u32, u8, u32) { }

void JAISound::setTrackPortData(u8 param_1, u8 param_2, u16 param_3)
{
	if ((unk8 & 0xC0000000) != 0x80000000)
		return;

	if (!getSeqParameter())
		return;

	if (!getSeqParameter()->unk1850)
		return;

	if (unk1 >= 4) {
		JAISystemInterface::writePortApp(getSeqParameter()->unk0,
		                                 getTrackPortRoute(param_1, param_2),
		                                 param_3);
	} else {
		getSeqParameter()->unk1850->unk8 |= 0x1000;
		getSeqParameter()->unk178C |= 1 << param_1;
		getSeqParameter()->unk1790[param_1] |= 1 << param_2;
	}
	getSeqParameter()->unk1354[param_1][param_2] = param_3;
}

void JAISound::setSeInterMovePara(JAIMoveParaSet* set, u32 value)
{
	if (set->unk4 != set->unk0) {
		if (value == 0) {
			set->unk4 = set->unk0;
		} else {
			set->unkC = value;
			set->unk8 = (set->unk0 - set->unk4) / value;
		}
	}
}

static inline u32 rand_u32()
{
	return JAIConst::random.get_ufloat_1() * 4294967300.0f;
}

void JAISound::setSeInterRandomPara(f32* param_1, u32 param_2, f32 param_3,
                                    f32 param_4)
{
	f32 fVar1
	    = ((f32)((rand_u32() % (param_2 * 2)) + 1) - (f32)param_2) / 1000.0f;
	if (*param_1 + fVar1 > param_4) {
		*param_1 = param_4;
	} else if (*param_1 < param_3 - fVar1) {
		*param_1 = param_3;
	} else {
		*param_1 += fVar1;
	}
}

void JAISound::setSeInterVolume(u8 param_1, f32 param_2, u32 param_3,
                                u8 param_4)
{
	if ((unk8 & 0xC0000000) != 0x00000000)
		return;

	if (!getSeParameter())
		return;

	JAIMoveParaSet* set = &getSeParameter()->unk124[param_1];
	set->unk0           = param_2;
	if (param_4 != 0) {
		u32 uVar5 = (param_4 * 1000) / 127U;
		setSeInterRandomPara(&set->unk0, uVar5, 0.0f, 1.0f);
	}

	setSeInterMovePara(set, param_3);
}

void JAISound::setSeInterPan(u8 param_1, f32 param_2, u32 param_3, u8 param_4)
{
	if ((unk8 & 0xC0000000) != 0x00000000)
		return;

	if (!getSeParameter())
		return;

	JAIMoveParaSet* set = &getSeParameter()->unk1A4[param_1];
	set->unk0           = param_2;
	if (param_4 != 0) {
		u32 uVar5 = (param_4 * 1000) / 127U;
		setSeInterRandomPara(&set->unk0, uVar5, 0.0f, 1.0f);
	}

	setSeInterMovePara(set, param_3);
}

void JAISound::setSeInterFxmix(u8 param_1, f32 param_2, u32 param_3, u8 param_4)
{
	if ((unk8 & 0xC0000000) != 0x00000000)
		return;

	if (!getSeParameter())
		return;

	JAIMoveParaSet* set = &getSeParameter()->unk2A4[param_1];
	set->unk0           = param_2;
	if (param_4 != 0) {
		u32 uVar5 = (param_4 * 1000) / 127U;
		setSeInterRandomPara(&set->unk0, uVar5, 0.0f, 1.0f);
	}

	setSeInterMovePara(set, param_3);
}

void JAISound::setSeInterFir(u8, u8, u32, u8) { }

void JAISound::setSeInterDolby(u8 param_1, f32 param_2, u32 param_3, u8 param_4)
{
	if ((unk8 & 0xC0000000) != 0x00000000)
		return;

	if (!getSeParameter())
		return;

	JAIMoveParaSet* set = &getSeParameter()->unk3A4[param_1];
	set->unk0           = param_2;
	if (param_4 != 0) {
		u32 uVar5 = (param_4 * 1000) / 127U;
		setSeInterRandomPara(&set->unk0, uVar5, 0.0f, 1.0f);
	}

	setSeInterMovePara(set, param_3);
}

void JAISound::setSeInterPitch(u8 param_1, f32 param_2, u32 param_3,
                               f32 param_4)
{
	if ((unk8 & 0xC0000000) != 0x00000000)
		return;

	if (!getSeParameter())
		return;

	JAIMoveParaSet* set = &getSeParameter()->unk224[param_1];
	set->unk0           = param_2;
	if (param_4 != 0) {
		u32 uVar5 = (param_4 * 1000);
		setSeInterRandomPara(&set->unk0, uVar5, 0.1f, 16.0f);
	}

	setSeInterMovePara(set, param_3);
}

void JAISound::setSePortData(u8 param_1, u16 param_2)
{
	if ((unk8 & 0xC0000000) != 0x00000000)
		return;

	if (!getSeParameter())
		return;

	if (unk1 == 1) {
		getSeParameter()->unk0[param_1] = param_2;
		getSeParameter()->unk20 |= 1 << param_1;
	} else {
		interPointer->unk38->setTrackPortData(unk0, param_1, param_2);
	}
}

void JAISound::getSePortData(u8) { }

void JAISound::setSeDistanceParameters()
{
	u8 uVar2 = unk5;
	if (unk1 == 2)
		uVar2 = 0;

	setSeDistanceVolume(uVar2);
	setSeDistancePan(uVar2);
	setSeDistancePitch(uVar2);
	setSePositionDopplar();
	setSeDistanceFxmix(uVar2);
	setSeDistanceFir(uVar2);

	if (!checkSwBit(0x400)) {
		f32 fxParam = interPointer->getMapInfoFxParameter(unk18);
		setFxmix(fxParam, 0, 2);
	}
	setSeDistanceDolby(uVar2);
}

void JAISound::setSeDistanceVolume(u8 param_1)
{
	f32 dVar4;
	if (!checkSwBit(2)) {
		dVar4 = setDistanceVolumeCommon(
		    JAIGlobalParameter::distanceMax,
		    (interPointer->getSoundSwBit(unk3C) & 0x70000) >> 16);
	} else {
		dVar4 = 1.0f;
	}

	setSeInterVolume(4, dVar4, param_1, 0);
}

void JAISound::setSeDistancePan(u8 param_1)
{
	f32 dVar4 = setDistancePanCommon();

	setSeInterPan(4, dVar4, param_1, 0);
}

void JAISound::setSeDistancePitch(u8 param_1)
{
	f32 fVar2 = 1.0f;
	if (checkSwBit(0x10)) {
		fVar2 = 1.0f
		        - (f32)((u8)(JAIConst::random.get_ufloat_1() * 16.0f) & 0xF)
		              / 192.0f;
	}

	if (checkSwBit(0x4000) && !checkSwBit(0x2) && !checkSwBit(0x300)) {
		if (JAIGlobalParameter::audioCameraMax == 1) {
			// TODO: likely an inline
			if (unk1C->unk18 >= JAIGlobalParameter::distanceMax) {
				fVar2 += JAIGlobalParameter::seDistancepitchMax;
			} else {
				fVar2 += JAIGlobalParameter::seDistancepitchMax
				         * (unk1C->unk18 / JAIGlobalParameter::distanceMax);
			}
		}
	}
	if (checkSwBit(0xC0)) {
		fVar2 += unk3 / 192.0f;
	}
	setSeInterPitch(4, fVar2, param_1, 0);
}

void JAISound::setSePositionDopplar()
{
	u32 uVar4 = JAIGlobalParameter::dopplarMoveTime;
	if (unk1 == 2)
		uVar4 = 1;

	// TODO: fakematch, wtf is actually happening here? Uninitialized variable?
	u32 bit;
	if (!(bit = getSwBit() & 0x300))
		return;

	if (JAIGlobalParameter::audioCameraMax != 1)
		return;

	u32 uninitializedMeme;
	f32 dVar5 = setPositionDopplarCommon(bit);
	setSeInterPitch(1, dVar5, uVar4, 0);
}

#pragma dont_inline on
void JAISound::setSeDistanceFxmix(u8 param_1)
{
	u16 fx = JAIGlobalParameter::seDefaultFx;
	if (!checkSwBit(0x4) && JAIGlobalParameter::audioCameraMax == 1) {
		// TODO: likely an inline
		if (unk1C->unk18 < JAIGlobalParameter::distanceMax) {
			fx = JAIGlobalParameter::seDistanceFxParameter
			     * (unk1C->unk18 / JAIGlobalParameter::distanceMax);
		} else {
			fx = JAIGlobalParameter::seDistanceFxParameter;
		}
	}
	if (fx > 0x7F)
		fx = 0x7F;
	// TODO: how do we make this not get inlined? :(
	setSeInterFxmix(4, (u8)fx / 127.0f, param_1, 0);
}
#pragma dont_inline off

void JAISound::setSeDistanceFir(u8) { }

#pragma dont_inline on
void JAISound::setSeDistanceDolby(u8 param_1)
{
	FabricatedPositionInfo* pi = unk1C;
	f32 fVar1;
	if (unk24 == 0
	    || pi->unk0.z < JAIGlobalParameter::seDolbyFrontDistanceMax) {
		fVar1 = 0.0f;
	} else {
		if (pi->unk0.z < 0.0f) {
			fVar1 = JAIGlobalParameter::seDolbyCenterValue
			        * (JAIGlobalParameter::seDolbyFrontDistanceMax - pi->unk0.z)
			        / JAIGlobalParameter::seDolbyFrontDistanceMax;
		} else {
			if (pi->unk0.z < JAIGlobalParameter::seDolbyBehindDistanceMax) {
				fVar1 = (127.0f - JAIGlobalParameter::seDolbyCenterValue)
				            * (pi->unk0.z
				               / JAIGlobalParameter::seDolbyBehindDistanceMax)
				        + JAIGlobalParameter::seDolbyCenterValue;
			} else {
				fVar1 = 127.0f;
			}
		}
	}
	setSeInterDolby(4, (u8)fVar1 / 127.0f, param_1, 0);
}
#pragma dont_inline off

void JAISound::setStreamMode(u32) { }

void JAISound::setStreamInterVolume(u8 param_1, f32 param_2, u32 param_3)
{
	if ((unk8 & 0xC0000000) != 0xC0000000)
		return;
	if (!getStreamParameter())
		return;

	int iVar2 = initMoveParameter(&getStreamParameter()->unk14[param_1],
	                              param_2, param_3);
	if (iVar2 == 1) {
		getStreamParameter()->unk8 |= 1 << param_1;
	}

	if (getStreamParameter()->unk3D4 && iVar2 != 2) {
		getStreamParameter()->unk3D4->unk10 |= 0x40000;
	}
}

void JAISound::setStreamInterPitch(u8 param_1, f32 param_2, u32 param_3)
{
	if ((unk8 & 0xC0000000) != 0xC0000000)
		return;
	if (!getStreamParameter())
		return;

	int iVar2 = initMoveParameter(&getStreamParameter()->unk154[param_1],
	                              param_2, param_3);
	if (iVar2 == 1) {
		getStreamParameter()->unkC |= 1 << param_1;
	}

	if (getStreamParameter()->unk3D4 && iVar2 != 2) {
		getStreamParameter()->unk3D4->unk10 |= 0x100000;
	}
}

void JAISound::setStreamInterPan(u8 param_1, f32 param_2, u32 param_3)
{
	if ((unk8 & 0xC0000000) != 0xC0000000)
		return;
	if (!getStreamParameter())
		return;

	int iVar2 = initMoveParameter(&getStreamParameter()->unk294[param_1],
	                              param_2, param_3);
	if (iVar2 == 1) {
		getStreamParameter()->unk10 |= 1 << param_1;
	}

	if (getStreamParameter()->unk3D4 && iVar2 != 2) {
		getStreamParameter()->unk3D4->unk10 |= 0x80000;
	}
}

void JAISound::setStreamPrepareFlag(u8) { }

void JAISound::checkStreamReady() { }

void JAISound::setPauseMode(u8 param_1, u8 param_2)
{
	switch (unk8 & 0xC0000000) {
	case 0x80000000:
		if (getSeqParameter()) {
			if (param_1 == 3)
				param_1 = 4;
			if (param_1) {
				switch (param_1) {
				case 1:
					// TODO: should be volume, but that gets inlined =(
					setSeqInterDolby(11, param_2 / 127.0f, 1);
					break;
				case 2:
					JASystem::TrackMgr::handleToSeq(getSeqParameter()->unk0)
					    ->pauseTrackAll();
					break;
				}
			} else {
				getSeqParameter();
				switch (getSeqParameter()->unk1755) {
				case 1:
					setSeqInterVolume(11, 1.0f, 1);
					break;
				case 2:
					JASystem::TrackMgr::handleToSeq(getSeqParameter()->unk0)
					    ->unPauseTrackAll();
					break;
				}
			}

			getSeqParameter()->unk1755 = param_1;
		}
		break;
	case 0xC0000000:
		if (getStreamParameter()) {
			if (param_1) {
				switch (param_1) {
				case 1:
					setStreamInterVolume(11, param_2 / 127.0f, 1);
					break;
				case 2:
					JAInter::StreamLib::setPauseFlag(2);
					break;
				}
			} else {
				switch (getStreamParameter()->unk0) {
				case 1:
					setStreamInterVolume(11, 1.0f, 1);
					break;
				case 2:
					JAInter::StreamLib::clearPauseFlag(2);
					break;
				}
			}

			getStreamParameter()->unk0 = param_1;
		}
		break;
	}
}

void JAISound::setSeqPrepareFlag(u8) { }

void JAISound::checkSeqReady() { }

void JAISound::getVolume(u8) { }

void JAISound::getPan(u8) { }

void JAISound::getPitch(u8) { }

void JAISound::getFxmix(u8) { }

void JAISound::getDolby(u8) { }

void JAISound::getTempoProportion() { }

void JAISound::getPortData(u8) { }

f32 JAISound::getSeqInterVolume(u8 param_1)
{
	f32 result;
	if (unk1 == 4 || unk1 == 5) {
		result = getSeqParameter()->unk114[param_1].unk4;
	} else {
		result = -1.0f;
	}
	return result;
}

void JAISound::getSeqInterPan(u8) { }

void JAISound::getSeqInterPitch(u8) { }

void JAISound::getSeqInterFxmix(u8) { }

void JAISound::getSeqInterDolby(u8) { }

void JAISound::getSeqTempoProportion() { }

void JAISound::getSeqPortData(u8) { }

void JAISound::getTrackParameterU7(u8, s8) { }

void JAISound::getTrackParameterF32(u8, s8) { }

void JAISound::getPointer(u8, s8) { }

void JAISound::getTrackPortData(u8, u8) { }

void JAISound::getSeParametermeterF32(u8, u8) { }

f32 JAISound::getStreamInterVolume(u8 param_1)
{
	if ((unk8 & 0xC0000000) == 0xC0000000 && getStreamParameter()) {
		if (unk1 == 4 || unk1 == 5)
			return getStreamParameter()->unk14[param_1].unk4;

		return -1.0f;
	}

	return -1.0f;
}

void JAISound::getStreamInterPitch(u8) { }

void JAISound::getActorGroundNumber() { }

JAISeqParameter* JAISound::getSeqParameter() { return (JAISeqParameter*)unk38; }

JAISeParameter* JAISound::getSeParameter() { return (JAISeParameter*)unk38; }

JAIStreamParameter* JAISound::getStreamParameter()
{
	return (JAIStreamParameter*)unk38;
}

u32 JAISound::getTrackPortRoute(u8 param_1, u8 param_2)
{
	if (unk8 & 0x800)
		return (param_1 >> 4) + 0x20000000 + ((param_1 & 0xF) << 4)
		       + (param_2 << 16);
	return (param_1 & 0xf) + 0x10000000 + (param_2 << 16);
}

void JAISound::getSeInfoPointer() { }
