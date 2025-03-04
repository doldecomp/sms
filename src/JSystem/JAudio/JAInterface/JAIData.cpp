
#include "JSystem/JAudio/JAInterface/JAIData.hpp"

JAIData::JAIData() { }

void JAIData::init() { }

void JAIData::initData() { }

void JAIData::initInfoDataWork(JAISoundTable* soundTable, char* param) { }

void JAIData::initLinkBuffer(JAILinkBuffer* linkBuffer, u32 param) { }

void JAIData::initSeqParaLinkBuffer() { }

void JAIData::initSeParaLinkBuffer() { }

void JAIData::initStreamParaLinkBuffer() { }

void JAIData::initDummyVecLink() { }

void JAIData::initSeqParameter(JAISeqParameter* param) { }

void JAIData::initSePara(JAISeParameter* param) { }

void JAIData::initStreamParameter(JAIStreamParameter* param) { }

void JAIData::initSeqTrackInfoParameter(u32 param) { }

void JAIData::initStreamUpdateParameter() { }

void JAIData::setSeMovePara(JAIMoveParaSet* moveParaSet) { }

void JAIData::moveParameter(JAIMoveParaSet* moveParaSet) { }

bool JAIData::checkOnMemory(u32 param1, u8* param2) { return false; }

u8 JAIData::checkUsefulAutoHeapPosition() { return 0; }

u8* JAIData::getFreeAutoHeapPointer(u8 param1, u32 param2) { return nullptr; }

void JAIData::releaseAutoHeapPointer(u8 param) { }

u8* JAIData::getFreeStayHeapPointer(u32 param1, u32 param2) { return nullptr; }

void JAIData::clearAutoHeap(JAIHeapBlock* heapBlock) { }

void JAIData::clearAutoHeap(u32 param) { }

void JAIData::clearStayHeap(u32 param) { }

void JAIData::setAutoHeapLoadedFlag(u8 param1, u8 param2) { }

void JAIData::changeAutoHeapPointerToPosition(u8* ptr) { }

void JAIData::stopPlayingSeq(u32 param) { }

u8* JAIData::getAutoHeapPointer(u32 param) { return nullptr; }

bool JAIData::getInfoPointer(u32 param, void** ptr) { return false; }

void JAIData::reloadInfoDataWork(JAISoundTable* soundTable) { }

void JAIData::setInfoDataPointer(JAISoundTable* soundTable, u8* ptr) { }
