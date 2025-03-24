#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>

void JAIBasic::checkNextFrameSe() { }

void JAIBasic::sendPlayingSeCommand() { }

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

void JAIBasic::clearSeqMuteFromSeStop(JAISound* sound) { }

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

void JAIBasic::sendSeAllParameter(JAISound* sound) { }

void JAIBasic::releaseSeRegist(JAISound* sound) { }
