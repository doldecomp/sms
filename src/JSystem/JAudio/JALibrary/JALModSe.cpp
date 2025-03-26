#include <JSystem/JAudio/JALibrary/JALModSe.hpp>
#include <JSystem/JAudio/JALibrary/JALSystem.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>

JALSystem::TFlagManager* JALSystem::spFManager;

void JALSystem::init() { spFManager = new TFlagManager; }

bool JALSystem::gateCheckFunc(u32 param_1, f32 param_2)
{
	if (TFlagManager::get()->isRegistered(param_1, ModType_JALSeModVolFunk)
	    && JALSeModVolFunk::gateCheck(param_1, param_2))
		return true;
	if (TFlagManager::get()->isRegistered(param_1, ModType_JALSeModPitFunk)
	    && JALSeModPitFunk::gateCheck(param_1, param_2))
		return true;
	if (TFlagManager::get()->isRegistered(param_1, ModType_JALSeModEffFunk)
	    && JALSeModEffFunk::gateCheck(param_1, param_2))
		return true;
	if (TFlagManager::get()->isRegistered(param_1, ModType_JALSeModVolFGrp)
	    && JALSeModVolFGrp::gateCheckGrp(param_1, param_2))
		return true;
	if (TFlagManager::get()->isRegistered(param_1, ModType_JALSeModPitFGrp)
	    && JALSeModPitFGrp::gateCheckGrp(param_1, param_2))
		return true;
	if (TFlagManager::get()->isRegistered(param_1, ModType_JALSeModEffFGrp)
	    && JALSeModEffFGrp::gateCheckGrp(param_1, param_2))
		return true;

	return false;
}

void JALSystem::processModFunc(JAISound* param_1, f32 param_2, u32 param_3,
                               u8 param_4)
{
	if (!param_1)
		return;

	f32 val;

	u32 thing = param_1->getUnk8();

	if (TFlagManager::get()->isRegistered(thing, ModType_JALSeModVolFunk)) {
		if (JALSeModVolFunk::calc(thing, param_2, &val))
			param_1->setVolume(val, param_3, param_4);
	} else if (TFlagManager::get()->isRegistered(thing,
	                                             ModType_JALSeModVolFGrp)) {
		if (JALSeModVolFGrp::calcGrp(thing, param_2, &val))
			param_1->setVolume(val, param_3, param_4);
	}

	if (TFlagManager::get()->isRegistered(thing, ModType_JALSeModPitFunk)) {
		if (JALSeModPitFunk::calc(thing, param_2, &val))
			param_1->setPitch(val, param_3, param_4);
	} else if (TFlagManager::get()->isRegistered(thing,
	                                             ModType_JALSeModPitFGrp)) {
		if (JALSeModPitFGrp::calcGrp(thing, param_2, &val))
			param_1->setPitch(val, param_3, param_4);
	}

	if (TFlagManager::get()->isRegistered(thing, ModType_JALSeModEffFunk)) {
		if (JALSeModEffFunk::calc(thing, param_2, &val))
			param_1->setFxmix(val, param_3, param_4);
	} else if (TFlagManager::get()->isRegistered(thing,
	                                             ModType_JALSeModEffFGrp)) {
		if (JALSeModEffFGrp::calcGrp(thing, param_2, &val))
			param_1->setFxmix(val, param_3, param_4);
	}
}

f32 JALSystem::processModDistVolume(u32 param_1, f32 param_2)
{
	f32 val;
	if (spFManager->isRegistered(param_1, ModType_JALSeModVolDist)) {
		if (JALSeModVolDist::calc(param_1, param_2, &val))
			return val;
	} else if (spFManager->isRegistered(param_1, ModType_JALSeModVolDGrp)) {
		if (JALSeModVolDGrp::calcGrp(param_1, param_2, &val))
			return val;
	}

	return 1.0f;
}

void JALSystem::append(JALSystem::ModType param_1, const char* param_2,
                       u32 param_3, f32 param_4, f32 param_5, f32 param_6,
                       f32 param_7, f32 param_8, JALCalc::CurveSign param_9,
                       f32 param_10, f32 param_11, u8 param_12)
{
	JALPrmSet set1(param_4, param_5);
	JADPrmS<f32> prm(param_6, nullptr);
	JALPrmSet set2(param_7, param_8);
	JALPrmSet set3(param_7, param_8);

	switch (param_1) {
	case ModType_JALSeModVolFunk:
		new JALSeModVolFunk(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		TFlagManager::get()->addUseFlag(param_3, param_1);
		break;

	case ModType_JALSeModPitFunk:
		new JALSeModPitFunk(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		TFlagManager::get()->addUseFlag(param_3, param_1);
		break;

	case ModType_JALSeModEffFunk:
		new JALSeModEffFunk(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		TFlagManager::get()->addUseFlag(param_3, param_1);
		break;

	case ModType_JALSeModVolDist:
		new JALSeModVolDist(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		TFlagManager::get()->addUseFlag(param_3, param_1);
		break;

	case ModType_JALSeModEffDist:
		new JALSeModEffDist(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		TFlagManager::get()->addUseFlag(param_3, param_1);
		break;

	case ModType_JALSeModPitDist:
		new JALSeModPitDist(param_2, param_3, &set1, &set3, &prm, param_9,
		                    param_12);
		TFlagManager::get()->addUseFlag(param_3, param_1);
		break;

	case ModType_JALSeModVolFGrp:
		new JALSeModVolFGrp(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModPitFGrp:
		new JALSeModPitFGrp(param_2, param_3, &set1, &set3, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModEffFGrp:
		new JALSeModEffFGrp(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModVolDGrp:
		new JALSeModVolDGrp(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModEffDGrp:
		new JALSeModEffDGrp(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModPitDGrp:
		new JALSeModPitDGrp(param_2, param_3, &set1, &set3, &prm, param_9,
		                    param_12);
		break;
	}
}

void JALSystem::appendGrpMember(JALSystem::ModType param_1, u32 param_2,
                                u32 param_3)
{
	switch (param_1) {
	case ModType_JALSeModVolFGrp: {
		JALSeModDataGrp<JALSeModVolFGrp>* found
		    = JALListS<JALSeModVolFGrp, u32>::search(param_2);
		if (found)
			found->append(new JALSeModDataGrpMemb(param_1, nullptr));
		spFManager->addUseFlag(param_2, param_1);
	} break;
	case ModType_JALSeModPitFGrp: {
		JALSeModDataGrp<JALSeModPitFGrp>* found
		    = JALListS<JALSeModPitFGrp, u32>::search(param_2);
		if (found)
			found->append(new JALSeModDataGrpMemb(param_1, nullptr));
		spFManager->addUseFlag(param_2, param_1);
	} break;
	case ModType_JALSeModEffFGrp: {
		JALSeModDataGrp<JALSeModEffFGrp>* found
		    = JALListS<JALSeModEffFGrp, u32>::search(param_2);
		if (found)
			found->append(new JALSeModDataGrpMemb(param_1, nullptr));
		spFManager->addUseFlag(param_2, param_1);
	} break;
	case ModType_JALSeModVolDGrp: {
		JALSeModDataGrp<JALSeModVolDGrp>* found
		    = JALListS<JALSeModVolDGrp, u32>::search(param_2);
		if (found)
			found->append(new JALSeModDataGrpMemb(param_1, nullptr));
		spFManager->addUseFlag(param_2, param_1);
	} break;
	case ModType_JALSeModEffDGrp: {
		JALSeModDataGrp<JALSeModEffDGrp>* found
		    = JALListS<JALSeModEffDGrp, u32>::search(param_2);
		if (found)
			found->append(new JALSeModDataGrpMemb(param_1, nullptr));
		spFManager->addUseFlag(param_2, param_1);
	} break;
	case ModType_JALSeModPitDGrp: {
		JALSeModDataGrp<JALSeModPitDGrp>* found
		    = JALListS<JALSeModPitDGrp, u32>::search(param_2);
		if (found)
			found->append(new JALSeModDataGrpMemb(param_1, nullptr));
		spFManager->addUseFlag(param_2, param_1);
	} break;
	}
}

JALSystem::TFlagManager::TFlagManager()
{
	u16 size;
	u8 i;
	u16 j;
	for (i = 0; i < 16; ++i) {
		size    = JAIBasic::basic->unk0->unk88.unk2[i];
		unk0[i] = new u16[size];
		for (j = 0; j < size; ++j)
			unk0[i][j] = 0;
	}
}

void JALSystem::TFlagManager::addUseFlag(u32 param_1, u16 param_2)
{
	unk0[(u16)param_1 >> 12][param_1 & 0x3FF] += param_2;
}

bool JALSystem::TFlagManager::isRegistered(u32 param_1, u16 param_2)
{
	if (param_2 & unk0[(u16)param_1 >> 12][param_1 & 0x3FF])
		return true;
	return false;
}
