#include <JSystem/JAudio/JALibrary/JALModSe.hpp>
#include <JSystem/JAudio/JALibrary/JALSystem.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>

JALSystem::TFlagManager* JALSystem::spFManager;

void JALSystem::init() { spFManager = new TFlagManager; }

bool JALSystem::gateCheckFunc(u32 soundID, f32 value)
{
	if (TFlagManager::get()->isRegistered(soundID, ModType_JALSeModVolFunk)
	    && JALSeModVolFunk::gateCheck(soundID, value))
		return true;
	if (TFlagManager::get()->isRegistered(soundID, ModType_JALSeModPitFunk)
	    && JALSeModPitFunk::gateCheck(soundID, value))
		return true;
	if (TFlagManager::get()->isRegistered(soundID, ModType_JALSeModEffFunk)
	    && JALSeModEffFunk::gateCheck(soundID, value))
		return true;
	if (TFlagManager::get()->isRegistered(soundID, ModType_JALSeModVolFGrp)
	    && JALSeModVolFGrp::gateCheckGrp(soundID, value))
		return true;
	if (TFlagManager::get()->isRegistered(soundID, ModType_JALSeModPitFGrp)
	    && JALSeModPitFGrp::gateCheckGrp(soundID, value))
		return true;
	if (TFlagManager::get()->isRegistered(soundID, ModType_JALSeModEffFGrp)
	    && JALSeModEffFGrp::gateCheckGrp(soundID, value))
		return true;

	return false;
}

void JALSystem::processModFunc(JAISound* sound, f32 value, u32 fadeTime,
                               u8 slot)
{
	if (!sound)
		return;

	f32 val;

	u32 theSoundId = sound->getID();

	if (TFlagManager::get()->isRegistered(theSoundId,
	                                      ModType_JALSeModVolFunk)) {
		if (JALSeModVolFunk::calc(theSoundId, value, &val))
			sound->setVolume(val, fadeTime, slot);
	} else if (TFlagManager::get()->isRegistered(theSoundId,
	                                             ModType_JALSeModVolFGrp)) {
		if (JALSeModVolFGrp::calcGrp(theSoundId, value, &val))
			sound->setVolume(val, fadeTime, slot);
	}

	if (TFlagManager::get()->isRegistered(theSoundId,
	                                      ModType_JALSeModPitFunk)) {
		if (JALSeModPitFunk::calc(theSoundId, value, &val))
			sound->setPitch(val, fadeTime, slot);
	} else if (TFlagManager::get()->isRegistered(theSoundId,
	                                             ModType_JALSeModPitFGrp)) {
		if (JALSeModPitFGrp::calcGrp(theSoundId, value, &val))
			sound->setPitch(val, fadeTime, slot);
	}

	if (TFlagManager::get()->isRegistered(theSoundId,
	                                      ModType_JALSeModEffFunk)) {
		if (JALSeModEffFunk::calc(theSoundId, value, &val))
			sound->setFxmix(val, fadeTime, slot);
	} else if (TFlagManager::get()->isRegistered(theSoundId,
	                                             ModType_JALSeModEffFGrp)) {
		if (JALSeModEffFGrp::calcGrp(theSoundId, value, &val))
			sound->setFxmix(val, fadeTime, slot);
	}
}

f32 JALSystem::processModDistVolume(u32 soundID, f32 distance)
{
	f32 val;
	u32 id = soundID;
	if (TFlagManager::get()->isRegistered(id, ModType_JALSeModVolDist)) {
		if (JALSeModVolDist::calc(id, distance, &val))
			return val;
	} else if (TFlagManager::get()->isRegistered(id, ModType_JALSeModVolDGrp)) {
		if (JALSeModVolDGrp::calcGrp(id, distance, &val))
			return val;
	}

	return 1.0f;
}

f32 JALSystem::processModDistPitch(u32 soundID, f32 distance)
{
	f32 val;
	u32 id = soundID;
	if (TFlagManager::get()->isRegistered(id, ModType_JALSeModPitDist)) {
		if (JALSeModPitDist::calc(id, distance, &val))
			return val;
	} else if (TFlagManager::get()->isRegistered(id, ModType_JALSeModPitDGrp)) {
		if (JALSeModPitDGrp::calcGrp(id, distance, &val))
			return val;
	}

	return 1.0f;
}

f32 JALSystem::processModDistFx(u32 soundID, f32 distance)
{
	f32 val;
	u32 id = soundID;
	if (TFlagManager::get()->isRegistered(id, ModType_JALSeModEffDist)) {
		if (JALSeModEffDist::calc(id, distance, &val))
			return val;
	} else if (TFlagManager::get()->isRegistered(id, ModType_JALSeModEffDGrp)) {
		if (JALSeModEffDGrp::calcGrp(id, distance, &val))
			return val;
	}

	return 1.0f;
}

void JALSystem::append(JALSystem::ModType type, const char* name, u32 soundID,
                       f32 inMin, f32 inMax, f32 defaultValue, f32 outMin,
                       f32 outMax, JALCalc::CurveSign curveSign, f32 unk1,
                       f32 unk2, u8 flags)
{
	JALPrmSet inputRange(inMin, inMax);
	JADPrmS<f32> defaultParam(defaultValue, nullptr);
	JALPrmSet outputRange(outMin, outMax);
	JALPrmSet outputPitchRange(outMin, outMax);

	switch (type) {
	case ModType_JALSeModVolFunk:
		new JALSeModVolFunk(name, soundID, &inputRange, &outputRange,
		                    &defaultParam, curveSign, flags);
		TFlagManager::get()->addUseFlag(soundID, type);
		break;

	case ModType_JALSeModPitFunk:
		new JALSeModPitFunk(name, soundID, &inputRange, &outputPitchRange,
		                    &defaultParam, curveSign, flags);
		TFlagManager::get()->addUseFlag(soundID, type);
		break;

	case ModType_JALSeModEffFunk:
		new JALSeModEffFunk(name, soundID, &inputRange, &outputRange,
		                    &defaultParam, curveSign, flags);
		TFlagManager::get()->addUseFlag(soundID, type);
		break;

	case ModType_JALSeModVolDist:
		new JALSeModVolDist(name, soundID, &inputRange, &outputRange,
		                    &defaultParam, curveSign, flags);
		TFlagManager::get()->addUseFlag(soundID, type);
		break;

	case ModType_JALSeModEffDist:
		new JALSeModEffDist(name, soundID, &inputRange, &outputRange,
		                    &defaultParam, curveSign, flags);
		TFlagManager::get()->addUseFlag(soundID, type);
		break;

	case ModType_JALSeModPitDist:
		new JALSeModPitDist(name, soundID, &inputRange, &outputPitchRange,
		                    &defaultParam, curveSign, flags);
		TFlagManager::get()->addUseFlag(soundID, type);
		break;

	case ModType_JALSeModVolFGrp:
		new JALSeModVolFGrp(name, soundID, &inputRange, &outputRange,
		                    &defaultParam, curveSign, flags);
		break;

	case ModType_JALSeModPitFGrp:
		new JALSeModPitFGrp(name, soundID, &inputRange, &outputPitchRange,
		                    &defaultParam, curveSign, flags);
		break;

	case ModType_JALSeModEffFGrp:
		new JALSeModEffFGrp(name, soundID, &inputRange, &outputRange,
		                    &defaultParam, curveSign, flags);
		break;

	case ModType_JALSeModVolDGrp:
		new JALSeModVolDGrp(name, soundID, &inputRange, &outputRange,
		                    &defaultParam, curveSign, flags);
		break;

	case ModType_JALSeModEffDGrp:
		new JALSeModEffDGrp(name, soundID, &inputRange, &outputRange,
		                    &defaultParam, curveSign, flags);
		break;

	case ModType_JALSeModPitDGrp:
		new JALSeModPitDGrp(name, soundID, &inputRange, &outputPitchRange,
		                    &defaultParam, curveSign, flags);
		break;
	}
}

void JALSystem::appendGrpMember(JALSystem::ModType groupType, u32 groupID,
                                u32 memberSoundID)
{
	switch (groupType) {
	case ModType_JALSeModVolFGrp: {
		JALSeModDataGrp<JALSeModVolFGrp>* found
		    = JALListS<JALSeModVolFGrp, u32>::search(groupID);
		if (found) {
			JALSeModDataGrpMemb* memb
			    = new JALSeModDataGrpMemb(memberSoundID, nullptr);
			found->append(memb);
		}
		spFManager->addUseFlag(memberSoundID, groupType);
	} break;
	case ModType_JALSeModPitFGrp: {
		JALSeModDataGrp<JALSeModPitFGrp>* found
		    = JALListS<JALSeModPitFGrp, u32>::search(groupID);
		if (found) {
			JALSeModDataGrpMemb* memb
			    = new JALSeModDataGrpMemb(memberSoundID, nullptr);
			found->append(memb);
		}
		spFManager->addUseFlag(memberSoundID, groupType);
	} break;
	case ModType_JALSeModEffFGrp: {
		JALSeModDataGrp<JALSeModEffFGrp>* found
		    = JALListS<JALSeModEffFGrp, u32>::search(groupID);
		if (found) {
			JALSeModDataGrpMemb* memb
			    = new JALSeModDataGrpMemb(memberSoundID, nullptr);
			if (found)
				found->append(memb);
		}
		spFManager->addUseFlag(memberSoundID, groupType);
	} break;
	case ModType_JALSeModVolDGrp: {
		JALSeModDataGrp<JALSeModVolDGrp>* found
		    = JALListS<JALSeModVolDGrp, u32>::search(groupID);
		if (found) {
			JALSeModDataGrpMemb* memb
			    = new JALSeModDataGrpMemb(memberSoundID, nullptr);
			if (found)
				found->append(memb);
		}
		spFManager->addUseFlag(memberSoundID, groupType);
	} break;
	case ModType_JALSeModEffDGrp: {
		JALSeModDataGrp<JALSeModEffDGrp>* found
		    = JALListS<JALSeModEffDGrp, u32>::search(groupID);
		if (found) {
			JALSeModDataGrpMemb* memb
			    = new JALSeModDataGrpMemb(memberSoundID, nullptr);
			if (found)
				found->append(memb);
		}
		spFManager->addUseFlag(memberSoundID, groupType);
	} break;
	case ModType_JALSeModPitDGrp: {
		JALSeModDataGrp<JALSeModPitDGrp>* found
		    = JALListS<JALSeModPitDGrp, u32>::search(groupID);
		if (found) {
			JALSeModDataGrpMemb* memb
			    = new JALSeModDataGrpMemb(memberSoundID, nullptr);
			if (found)
				found->append(memb);
		}
		spFManager->addUseFlag(memberSoundID, groupType);
	} break;
	}
}

JALSystem::TFlagManager::TFlagManager()
{
	u16 size;
	for (u8 i = 0; i < 16; ++i) {
		size    = JAIBasic::getInterface()->mData->mSeTable.mSoundMax[i];
		mFlags[i] = new u16[size];
		for (u16 j = 0; j < size; ++j)
			mFlags[i][j] = 0;
	}
}

void JALSystem::TFlagManager::addUseFlag(u32 soundID, u16 modType)
{
	mFlags[(u16)soundID >> 12][soundID & JAISoundID_IndexMask] += modType;
}

u16 JALSystem::TFlagManager::getUseFlag(u32 soundID)
{
	return mFlags[(u16)soundID >> 12][soundID & JAISoundID_IndexMask];
}

bool JALSystem::TFlagManager::isRegistered(u32 soundID, u16 modType)
{
	if (modType & mFlags[(u16)soundID >> 12][soundID & JAISoundID_IndexMask])
		return true;
	return false;
}
