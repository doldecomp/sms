#include <JSystem/JAudio/JALibrary/JALModSe.hpp>
#include <JSystem/JAudio/JALibrary/JALSystem.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>

JALSystem::TFlagManager* JALSystem::spFManager;

void JALSystem::init() { spFManager = new TFlagManager; }

#pragma dont_inline on
bool JALSystem::gateCheckFunc(u32 param_1, f32 param_2)
{

	if (spFManager->isRegistered(param_1, ModType_JALSeModVolFunk)
	    && JALSeModVolFunk::gateCheck(param_1, param_2))
		return true;
	if (spFManager->isRegistered(param_1, ModType_JALSeModPitFunk)
	    && JALSeModPitFunk::gateCheck(param_1, param_2))
		return true;
	if (spFManager->isRegistered(param_1, ModType_JALSeModEffFunk)
	    && JALSeModEffFunk::gateCheck(param_1, param_2))
		return true;
	if (spFManager->isRegistered(param_1, ModType_JALSeModVolFGrp)
	    && JALSeModVolFGrp::gateCheckGrp(param_1, param_2))
		return true;
	if (spFManager->isRegistered(param_1, ModType_JALSeModPitFGrp)
	    && JALSeModPitFGrp::gateCheckGrp(param_1, param_2))
		return true;
	if (spFManager->isRegistered(param_1, ModType_JALSeModEffFGrp)
	    && JALSeModEffFGrp::gateCheckGrp(param_1, param_2))
		return true;

	return false;
}

void JALSystem::processModFunc(JAISound* param_1, f32 param_2, u32 param_3,
                               u8 param_4)
{
	if (!param_1)
		return;

	u32 thing = param_1->unk8;

	if (spFManager->isRegistered(thing, ModType_JALSeModVolFunk)) {
		f32 val;
		if (JALSeModVolFunk::calc(thing, param_2, &val))
			param_1->setVolume(val, param_3, param_4);
	} else if (spFManager->isRegistered(thing, ModType_JALSeModVolFGrp)) {
		f32 val;
		if (JALSeModVolFGrp::calcGrp(thing, param_2, &val))
			param_1->setVolume(val, param_3, param_4);
	}

	if (spFManager->isRegistered(thing, ModType_JALSeModPitFunk)) {
		f32 val;
		if (JALSeModPitFunk::calc(thing, param_2, &val))
			param_1->setPitch(val, param_3, param_4);
	} else if (spFManager->isRegistered(thing, ModType_JALSeModPitFGrp)) {
		f32 val;
		if (JALSeModPitFGrp::calcGrp(thing, param_2, &val))
			param_1->setPitch(val, param_3, param_4);
	}

	if (spFManager->isRegistered(thing, ModType_JALSeModEffFunk)) {
		f32 val;
		if (JALSeModEffFunk::calc(thing, param_2, &val))
			param_1->setFxmix(val, param_3, param_4);
	} else if (spFManager->isRegistered(thing, ModType_JALSeModEffFGrp)) {
		f32 val;
		if (JALSeModEffFGrp::calcGrp(thing, param_2, &val))
			param_1->setFxmix(val, param_3, param_4);
	}
}

f32 JALSystem::processModDistVolume(u32 param_1, f32 param_2)
{
	if (spFManager->isRegistered(param_1, ModType_JALSeModVolDist)) {
		f32 val;
		if (JALSeModVolDist::calc(param_1, param_2, &val))
			return val;
	} else if (spFManager->isRegistered(param_1, ModType_JALSeModVolDGrp)) {
		f32 val;
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
	JALPrmSet set1;
	JALPrmSet set2;
	JADPrmS<f32> prm;

	switch (param_1) {
	case ModType_JALSeModVolFunk:
		new JALSeModVolFunk(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModPitFunk:
		new JALSeModPitFunk(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModEffFunk:
		new JALSeModEffFunk(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModVolDist:
		new JALSeModVolDist(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModPitDist:
		new JALSeModPitDist(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModEffDist:
		new JALSeModEffDist(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModVolFGrp:
		new JALSeModVolFGrp(param_2, param_3, &set1, &set2, &prm, param_9,
		                    param_12);
		break;

	case ModType_JALSeModPitFGrp:
		new JALSeModPitFGrp(param_2, param_3, &set1, &set2, &prm, param_9,
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
		new JALSeModPitDGrp(param_2, param_3, &set1, &set2, &prm, param_9,
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
		if (found) {
			JALSeModDataGrpMemb* memb = new JALSeModDataGrpMemb(param_3, found);
			found->append(&memb->unk4);
			memb->unk14 = found;
		}
		spFManager->addUseFlag(param_2, ModType_JALSeModVolFGrp);
	} break;
	case ModType_JALSeModPitFGrp: {
		JALSeModDataGrp<JALSeModPitFGrp>* found
		    = JALListS<JALSeModPitFGrp, u32>::search(param_2);
		if (found) {
			JALSeModDataGrpMemb* memb = new JALSeModDataGrpMemb(param_3, found);
			found->append(&memb->unk4);
			memb->unk14 = found;
		}
		spFManager->addUseFlag(param_2, ModType_JALSeModPitFGrp);
	} break;
	case ModType_JALSeModEffFGrp: {
		JALSeModDataGrp<JALSeModEffFGrp>* found
		    = JALListS<JALSeModEffFGrp, u32>::search(param_2);
		if (found) {
			JALSeModDataGrpMemb* memb = new JALSeModDataGrpMemb(param_3, found);
			if (found) {
				found->append(&memb->unk4);
				memb->unk14 = found;
			}
		}
		spFManager->addUseFlag(param_2, ModType_JALSeModEffFGrp);
	} break;
	case ModType_JALSeModVolDGrp: {
		JALSeModDataGrp<JALSeModVolDGrp>* found
		    = JALListS<JALSeModVolDGrp, u32>::search(param_2);
		if (found) {
			JALSeModDataGrpMemb* memb = new JALSeModDataGrpMemb(param_3, found);
			if (found) {
				found->append(&memb->unk4);
				memb->unk14 = found;
			}
		}
		spFManager->addUseFlag(param_2, ModType_JALSeModVolDGrp);
	} break;
	case ModType_JALSeModEffDGrp: {
		JALSeModDataGrp<JALSeModEffDGrp>* found
		    = JALListS<JALSeModEffDGrp, u32>::search(param_2);
		if (found) {
			JALSeModDataGrpMemb* memb = new JALSeModDataGrpMemb(param_3, found);
			if (found) {
				found->append(&memb->unk4);
				memb->unk14 = found;
			}
		}
		spFManager->addUseFlag(param_2, ModType_JALSeModEffDGrp);
	} break;
	case ModType_JALSeModPitDGrp: {
		JALSeModDataGrp<JALSeModPitDGrp>* found
		    = JALListS<JALSeModPitDGrp, u32>::search(param_2);
		if (found) {
			JALSeModDataGrpMemb* memb = new JALSeModDataGrpMemb(param_3, found);
			if (found) {
				found->append(&memb->unk4);
				memb->unk14 = found;
			}
		}
		spFManager->addUseFlag(param_2, ModType_JALSeModPitDGrp);
	} break;
	}
}

JALSystem::TFlagManager::TFlagManager() { }

void JALSystem::TFlagManager::addUseFlag(u32, u16) { }

bool JALSystem::TFlagManager::isRegistered(u32, u16) { }
