#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>

JASystem::Kernel::TPortCmd JAISystemInterface::systemPortCmd;

void JAISystemInterface::startSeq(u32) { }

void JAISystemInterface::stopSeq(s32) { }

bool JAISystemInterface::checkSeqActiveFlag(u32) { return false; }

void JAISystemInterface::setSeqData(JASystem::TTrack*, u8*, s32,
                                    JASystem::Player::SEQ_PLAYMODE)
{
}

void JAISystemInterface::rootInit(JAISeqUpdateData*) { }

void JAISystemInterface::trackInit(JAISeqUpdateData*) { }

void JAISystemInterface::outerInit(JAISeqUpdateData*, void*, u32, u16, u8) { }

JASystem::TTrack* JAISystemInterface::trackToSeqp(JAISound*, u8)
{
	return nullptr;
}

void JAISystemInterface::setSeqPortargsF32(JAISeqUpdateData*, u32, u8, f32) { }

void JAISystemInterface::setSeqPortargsU32(JAISeqUpdateData*, u32, u8, u32) { }

void JAISystemInterface::setSeqPortargsPS16(JAISeqUpdateData*, u32, u8, s16*) {
}

void JAISystemInterface::setPortParameter(JASystem::Kernel::TPortArgs*,
                                          JASystem::TTrack*, u32, u32)
{
}

void JAISystemInterface::setSePortParameter(JASystem::Kernel::TPortArgs*) { }

void JAISystemInterface::setAudioFrameParameter(JASystem::Kernel::TPortArgs*) {
}

void JAISystemInterface::writePortApp(u32, u32, u16) { }

void JAISystemInterface::readPortApp(u32, u32, u16*) { }

bool JAISystemInterface::checkFileExsistence(char*) { return false; }

void* JAISystemInterface::JAIouterP(void*) { return nullptr; }

void* JAISystemInterface::JAIouterSW(void*) { return nullptr; }
