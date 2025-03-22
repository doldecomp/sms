#include <JSystem/JAudio/JAInterface/JAIData.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>

void JAIData::init()
{
	unk88.unk78  = 0;
	unkC.unk78   = 0;
	unk104.unk78 = 0;
	unk1B0       = 0;
	unk1F8       = 0;
	unk1B4       = 0;
	unk1B8       = 0;
}

void JAIData::initLinkBuffer(JAILinkBuffer* linkBuffer, u32 param)
{
	int i;
	u32 thing = JAIGlobalParameter::audioCameraMax * 0x1C;

	linkBuffer->unk0 = linkBuffer->unk8;
	linkBuffer->unk4 = nullptr;

	linkBuffer->unk8[0].unk2C = nullptr;
	linkBuffer->unk8[0].unk30 = &linkBuffer->unk8[1];
	JAISound::interPointer    = unk1F4;
	linkBuffer->unk8[0].unk1  = 0;
	linkBuffer->unk8[0].unk1C = unk1F4->allocHeap(thing);
	for (i = 1; i < param - 1; ++i) {
		linkBuffer->unk8[i].unk2C = &linkBuffer->unk8[i - 1];
		linkBuffer->unk8[i].unk30 = &linkBuffer->unk8[i + 1];
		JAISound::interPointer    = unk1F4;
		linkBuffer->unk8[i].unk1  = 0;
		linkBuffer->unk8[i].unk1C = unk1F4->allocHeap(thing);
	}
	linkBuffer->unk8[i].unk2C = &linkBuffer->unk8[i - 1];
	linkBuffer->unk8[i].unk30 = nullptr;
	JAISound::interPointer    = unk1F4;
	linkBuffer->unk8[i].unk1  = 0;
	linkBuffer->unk8[i].unk1C = unk1F4->allocHeap(thing);
}

void JAIData::initSeqParaLinkBuffer()
{
	int i;

	unk1BC = &unk1C4[0];
	unk1C0 = nullptr;

	unk1C4[0].unk1854 = nullptr;
	unk1C4[0].unk1858 = unk1C4 + 1;
	for (i = 1; i < JAIGlobalParameter::seqControlBufferMax - 1; ++i) {
		unk1C4[i].unk1854 = &unk1C4[i - 1];
		unk1C4[i].unk1858 = &unk1C4[i + 1];
	}
	unk1C4[i].unk1854 = &unk1C4[i - 1];
	unk1C4[i].unk1858 = nullptr;

	for (int i = 0; i < JAIGlobalParameter::seqControlBufferMax; ++i)
		initSeqParameter(&unk1C4[i]);
}

void JAIData::initDummyVecLink() { }

void JAIData::initSeqParameter(JAISeqParameter* param)
{
	u32 i;

	param->unk1758   = -1;
	param->unk4.unk4 = 1.0f;
	param->unk4.unkC = 0;
	param->unk1755   = 0;
	param->unk1756   = 0;

	// TODO: array? but why ops reordered tho?
	param->unk175C = 0;
	param->unk1760 = 0;
	param->unk1764 = 0;
	param->unk1768 = 0;
	param->unk1770 = 0;

	param->unk176C = 0;

	param->unk1774 = 0;
	param->unk1778 = 0;
	param->unk177C = 0;
	param->unk1780 = 0;
	param->unk1784 = 0;
	param->unk1788 = 0;
	param->unk178C = 0;

	for (i = 0; i < JAIGlobalParameter::seqTrackMax; ++i) {
		param->unk754[i].unk4  = 1.0f;
		param->unk954[i].unk4  = 0.5f;
		param->unkB54[i].unk4  = 1.0f;
		param->unkD54[i].unk4  = 0.0f;
		param->unkF54[i].unk4  = 0.0f;
		param->unk1154[i].unk4 = 0.0f;

		param->unk754[i].unk0  = 1.0f;
		param->unk954[i].unk0  = 0.5f;
		param->unkB54[i].unk0  = 1.0f;
		param->unkD54[i].unk0  = 0.0f;
		param->unkF54[i].unk0  = 0.0f;
		param->unk1154[i].unk0 = 0.0f;

		param->unk754[i].unkC  = 0;
		param->unk954[i].unkC  = 0;
		param->unkB54[i].unkC  = 0;
		param->unkD54[i].unkC  = 0;
		param->unkF54[i].unkC  = 0;
		param->unk1154[i].unkC = 0;

		param->unk1830[i].flag1 = 0;
		param->unk1830[i].flag3 = 0;
		param->unk1810[i]       = 0;

		param->unk1790[i] = 0;

		param->unk1354[i][0]  = 0;
		param->unk1354[i][1]  = 0;
		param->unk1354[i][2]  = 0;
		param->unk1354[i][3]  = 0;
		param->unk1354[i][4]  = 0;
		param->unk1354[i][5]  = 0;
		param->unk1354[i][6]  = 0;
		param->unk1354[i][7]  = 0;
		param->unk1354[i][8]  = 0;
		param->unk1354[i][9]  = 0;
		param->unk1354[i][10] = 0;
		param->unk1354[i][11] = 0;
		param->unk1354[i][12] = 0;
		param->unk1354[i][13] = 0;
		param->unk1354[i][14] = 0;
		param->unk1354[i][15] = 0;
	}

	for (i = 0; i < 16; ++i) {
		param->unk14[i].unk4 = 0.0f;
		param->unk14[i].unkC = 0;
	}

	for (i = 0; i < JAIGlobalParameter::seqPlayTrackMax + 0xC; ++i) {
		param->unk114[i].unk4 = 1.0f;
		param->unk254[i].unk4 = 0.5f;
		param->unk394[i].unk4 = 1.0f;
		param->unk4D4[i].unk4 = 0.0f;
		param->unk614[i].unk4 = 0.0f;

		param->unk114[i].unk0 = 1.0f;
		param->unk254[i].unk0 = 0.5f;
		param->unk394[i].unk0 = 1.0f;
		param->unk4D4[i].unk0 = 0.0f;
		param->unk614[i].unk0 = 0.0f;

		param->unk114[i].unkC = 0;
		param->unk254[i].unkC = 0;
		param->unk394[i].unkC = 0;
		param->unk4D4[i].unkC = 0;
		param->unk614[i].unkC = 0;
	}
}

void JAIData::initSeParaLinkBuffer() { }

void JAIData::initSePara(JAISeParameter* param)
{
	f32 dolby = JAIGlobalParameter::seDolbyCenterValue / 127.0f;
	for (int i = 0; i < 8; ++i) {
		param->unk124[i].unk4 = 1.0f;
		param->unk1A4[i].unk4 = 0.5f;
		param->unk224[i].unk4 = 1.0f;
		param->unk2A4[i].unk4 = 0.0f;
		param->unk324[i].unk4 = 0.0f;
		param->unk3A4[i].unk4 = dolby;

		param->unk124[i].unkC = 0;
		param->unk1A4[i].unkC = 0;
		param->unk224[i].unkC = 0;
		param->unk2A4[i].unkC = 0;
		param->unk324[i].unkC = 0;
		param->unk3A4[i].unkC = 0;
	}
	param->unk124[7].unk4 = -1.0f;
	param->unk1A4[7].unk4 = -1.0f;
	param->unk224[7].unk4 = -1.0f;
	param->unk2A4[7].unk4 = -1.0f;
	param->unk324[7].unk4 = -1.0f;
	param->unk3A4[7].unk4 = -1.0f;

	param->unk424 = 0;
	param->unk428 = 0;
	param->unk42C = 0;
	param->unk430 = 0;
	param->unk434 = 0;
	param->unk438 = 0;

	param->unk20 = 0;
}

void JAIData::initStreamParaLinkBuffer()
{
	int i;

	unk1D4 = &unk1DC[0];
	unk1D8 = nullptr;

	unk1DC[0].unk3D8 = nullptr;
	unk1DC[0].unk3DC = &unk1DC[1];
	for (i = 1; i < JAIGlobalParameter::streamParameterBufferMax - 1; ++i) {
		unk1DC[i].unk3D8 = &unk1DC[i - 1];
		unk1DC[i].unk3DC = &unk1DC[i + 1];
	}
	unk1DC[i].unk3D8 = &unk1DC[i - 1];
	unk1DC[i].unk3DC = nullptr;

	for (int i = 0; i < JAIGlobalParameter::streamParameterBufferMax; ++i)
		initStreamParameter(&unk1DC[i]);
}

void JAIData::initStreamParameter(JAIStreamParameter* param)
{
	param->unk0 = 0;
	param->unk4 = 0;
	param->unk8 = 0;
	param->unkC = 0;

	for (int i = 0; i < 13; ++i) {
		param->unk14[i].unk4  = 1.0f;
		param->unk14[i].unk0  = 1.0f;
		param->unk14[i].unkC  = 0;
		param->unk154[i].unk4 = 1.0f;
		param->unk154[i].unk0 = 1.0f;
		param->unk154[i].unkC = 0;
		param->unk294[i].unk4 = 0.5f;
		param->unk294[i].unk0 = 0.5f;
		param->unk294[i].unkC = 0;
	}
}

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

void JAIData::initData() { }

void JAIData::initInfoDataWork(JAISoundTable* soundTable, char* param) { }

void JAIData::reloadInfoDataWork(JAISoundTable* soundTable) { }

void JAIData::setInfoDataPointer(JAISoundTable* soundTable, u8* ptr) { }
