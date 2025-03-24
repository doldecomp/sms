#include <JSystem/JAudio/JAInterface/JAIData.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>
#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>

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

void JAIData::initDummyVecLink()
{
	int i;

	unk228 = (JAIDummyVec*)unk230;
	unk22C = nullptr;

	unk230[0].unk0 = nullptr;
	unk230[0].unk4 = &unk230[1];
	for (i = 1; i < JAIGlobalParameter::dummyPositionMax - 1; ++i) {
		unk230[i].unk0 = &unk230[i - 1];
		unk230[i].unk4 = &unk230[i + 1];
	}
	unk230[i].unk0 = &unk230[i - 1];
	unk230[i].unk4 = nullptr;
}

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

void JAIData::initSeParaLinkBuffer()
{
	int i;

	unk1C8 = &unk1D0[0];
	unk1CC = nullptr;

	unk1D0[0].unk43C = nullptr;
	unk1D0[0].unk440 = &unk1D0[1];
	for (i = 1; i < JAIGlobalParameter::seRegistMax
	                        * JAIGlobalParameter::getParamSeCategoryMax()
	                    - 1;
	     ++i) {
		unk1D0[i].unk43C = &unk1D0[i - 1];
		unk1D0[i].unk440 = &unk1D0[i + 1];
	}
	unk1D0[i].unk43C = &unk1D0[i - 1];
	unk1D0[i].unk440 = nullptr;
}

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

void JAIData::initSeqTrackInfoParameter(u32 param)
{
	unk180[param].unkC  = 1.0f;
	unk180[param].unk18 = 0.5f;
	unk180[param].unk10 = 1.0f;
	unk180[param].unk14 = 0.0f;
	unk180[param].unk1C = 0.0f;
	unk180[param].unk20 = 1.0f;
	for (int i = 0; i < JAIGlobalParameter::seqTrackMax; ++i) {
		unk180[param].unk24[i] = 1.0f;
		unk180[param].unk30[i] = 64.0f;
		unk180[param].unk28[i] = 1.0f;
		unk180[param].unk2C[i] = 0.0f;
		unk180[param].unk34[i] = 0.0f;
		unk180[param].unk44[i] = 0;
	}
}

void JAIData::initStreamUpdateParameter()
{
	unk184->unk0  = 0;
	unk184->unk1  = 0;
	unk184->unk2  = 0;
	unk184->unk4  = 1.0f;
	unk184->unk8  = 1.0f;
	unk184->unkC  = 0.5f;
	unk184->unk10 = 0;
	unk184->unk14 = nullptr;
}

void JAIData::setSeMovePara(JAIMoveParaSet* moveParaSet)
{
	for (u8 i = 0; i < 8; ++i) {
		if (!moveParaSet[i].unkC)
			continue;

		--moveParaSet[i].unkC;
		if (moveParaSet[i].unkC == 0)
			moveParaSet[i].unk4 = moveParaSet[i].unk0;
		else
			moveParaSet[i].unk4 += moveParaSet[i].unk8;
	}
}

bool JAIData::moveParameter(JAIMoveParaSet* moveParaSet)
{
	if (moveParaSet->unkC == 0)
		return false;

	if (--moveParaSet->unkC) {
		moveParaSet->unk4 -= moveParaSet->unk8;
		return true;
	}

	moveParaSet->unk4 = moveParaSet->unk0;
	return false;
}

void* JAIData::checkOnMemory(u32 param1, u8* param2)
{
	for (u8 i = 0; i < JAIGlobalParameter::autoHeapMax; ++i) {
		if (param1 != unk1EC[i].unk8)
			continue;

		if (unk1EC[i].unk0 == 1)
			return (void*)0xffffffff;

		if (param2)
			*param2 = i;

		return unk1EC[i].unk4;
	}

	for (u8 i = 0; i < unk1B8; ++i) {
		if (param1 != unk1F0[i].unk8)
			continue;

		if (param2 != 0)
			*param2 = 0xff;

		return unk1F0[i].unk4;
	}
	return nullptr;
}

u8 JAIData::checkUsefulAutoHeapPosition()
{
	int i            = 0;
	u32 smallestSize = -1;
	int smallest     = 0;

	for (; i < JAIGlobalParameter::autoHeapMax; ++i)
		if (unk1EC[i].unk8 == -1)
			break;

	if (i == JAIGlobalParameter::autoHeapMax) {
		for (i = 0; i < JAIGlobalParameter::autoHeapMax; ++i) {
			if (smallestSize > unk1EC[i].unkC && unk1EC[i].unk10 == -1) {
				smallest     = i;
				smallestSize = unk1EC[i].unkC;
			}
		}

		if (smallestSize != -1)
			i = smallest;
	}

	return i;
}

void* JAIData::getFreeAutoHeapPointer(u8 param1, u32 param2)
{
	unk1EC[param1].unk8  = param2;
	void* result         = unk1EC[param1].unk4;
	unk1EC[param1].unk10 = unk1B4;
	unk1EC[param1].unkC  = unk1B4;
	++unk1B4;
	return result;
}

void JAIData::releaseAutoHeapPointer(u8 param)
{
	if (param == 0xff)
		return;
	unk1EC[param].unk10 = -1;
}

u8* JAIData::getFreeStayHeapPointer(u32 param1, u32 param2)
{
	u8* result;

	if (unk1B8 >= JAIGlobalParameter::stayHeapMax)
		return nullptr;

	if ((u8*)unk1F0[unk1B8].unk4 + param1
	        < (u8*)unk1F0[0].unk4 + JAIGlobalParameter::stayHeapSize
	    && unk1B8 < JAIGlobalParameter::stayHeapMax) {
		result              = (u8*)unk1F0[unk1B8].unk4;
		unk1F0[unk1B8].unk8 = param2;
		u8* ptr             = (u8*)unk1F0[unk1B8].unk4 + (param1 & ~0x1F);
		if (param1 & 0x1F)
			ptr += 0x20;
		++unk1B8;
		if (unk1B8 < JAIGlobalParameter::stayHeapMax)
			unk1F0[unk1B8].unk4 = ptr;
	} else {
		result = nullptr;
	}
	return result;
}

void JAIData::clearAutoHeap(JAIHeapBlock* heapBlock) { }

void JAIData::clearAutoHeap(u32 param) { }

void JAIData::clearStayHeap(u32 param) { }

void JAIData::setAutoHeapLoadedFlag(u8 param1, u8 param2)
{
	unk1EC[param1].unk0 = param2;
}

void JAIData::changeAutoHeapPointerToPosition(u8* ptr) { }

void JAIData::stopPlayingSeq(u32 param) { }

u8* JAIData::getAutoHeapPointer(u32 param) { return nullptr; }

void JAIData::getInfoPointer(u32 param_1, void** param_2)
{
	u32 thing;
	JAISoundTable* table;

	*param_2 = &JAIConst::nullInfoData2;
	if (unk1B0 == 0) {
		table = &unk88;
		switch (param_1 & 0xC0000000) {
		case 0x00000000:
			thing = param_1 >> 12 & 0xff;
			JAIGlobalParameter::getParamSeCategoryMax();
			break;
		case 0x80000000:
			thing = 16;
			break;
		case 0xC0000000:
			thing = 17;
			break;
		}
	} else {
		switch (param_1 & 0xC0000000) {
		case 0x00000000:
			thing = param_1 >> 12 & 0xff;
			table = &unk88;
			JAIGlobalParameter::getParamSeCategoryMax();
			break;
		case 0x80000000:
			table = &unkC;
			thing = 0x10;
			break;
		case 0xC0000000:
			table = &unk104;
			thing = 0x11;
			break;
		}
	}

	// TODO: impossible to guess what they subtracted from `thing`
	// without more additional context/info =(
	// u32 tmp = param_1 & 0x3FF;
	// if (table->unk78 && table->unk12[thing - 0x10] < tmp)
	// 	*param_2 = &table->unk40[thing - 0x10][tmp];
	// else
	// 	*param_2 = nullptr;
}

void JAIData::initData()
{
	initInfoDataWork(&unk88, JAIGlobalParameter::seInfoFileName);
	if (unk1B0 == 1) {
		initInfoDataWork(&unkC, JAIGlobalParameter::seqInfoFileName);
		initInfoDataWork(&unk104, JAIGlobalParameter::streamInfoFileName);
	}
	if (unk1F4->unk68) {
		JAIGlobalParameter::seTrackMax = 0;
		for (int i = 0; i < JAIGlobalParameter::soundSceneMax; ++i) {
			u32 sum = 0;
			for (int j = 0; j < JAIGlobalParameter::getParamSeCategoryMax();
			     ++j) {
				sum += unk1F4->unk68[i][2 * j];
			}
			if (JAIGlobalParameter::seTrackMax < sum)
				JAIGlobalParameter::seTrackMax = sum;
		}
	}
	unk230 = (JAIDummyVec*)unk1F4->allocHeap(
	    JAIGlobalParameter::dummyPositionMax * sizeof(JAIDummyVec));
	initDummyVecLink();

	unk1E8 = (JAILinkBuffer*)unk1F4->allocHeap(
	    JAIGlobalParameter::getParamSeCategoryMax() * sizeof(JAILinkBuffer));
	unk8 = (void**)unk1F4->allocHeap(JAIGlobalParameter::getParamSeCategoryMax()
	                                 * sizeof(void*));
	unk1E4 = (JAISound**)unk1F4->allocHeap(
	    JAIGlobalParameter::getParamSeCategoryMax() * sizeof(JAISound*));

	for (int i = 0; i < JAIGlobalParameter::getParamSeCategoryMax(); ++i) {
		unk1E4[i]      = unk1F4->makeSound(JAIGlobalParameter::seRegistMax);
		unk1E8[i].unk8 = unk1E4[i];
		initLinkBuffer(&unk1E8[i], JAIGlobalParameter::seRegistMax);
		unk8[i] = unk1F4->allocHeap(JAIGlobalParameter::seRegistMax * 0xC);
		for (int j = 0; j < JAIGlobalParameter::seRegistMax; ++j) {
			unk8[j] = 0; // TODO:
		}
	}
	unk0 = (FabricatedUnk0Struct*)unk1F4->allocHeap(
	    JAIGlobalParameter::seTrackMax * 0x18);
	for (u32 i = 0; i < JAIGlobalParameter::seTrackMax; ++i) {
		FabricatedUnk0Struct& s = unk0[i];

		s.unk4  = 1.0f;
		s.unk8  = 1.0f;
		s.unkC  = 0.0f;
		s.unk10 = 0.5f;
		s.unk0  = 0xff;
		s.unk14 = 0.0f;
	}

	unk208      = unk1F4->makeSound(JAIGlobalParameter::seqControlBufferMax);
	unk20C      = unk1F4->makeSound(JAIGlobalParameter::streamControlBufferMax);
	unk210.unk8 = unk208;
	initLinkBuffer(&unk210, JAIGlobalParameter::seqControlBufferMax);
	unk21C.unk8 = unk20C;
	initLinkBuffer(&unk21C, JAIGlobalParameter::streamControlBufferMax);
	unk1C4 = (JAISeqParameter*)unk1F4->allocHeap(
	    JAIGlobalParameter::seqControlBufferMax * sizeof(JAISeqParameter));
	initSeqParaLinkBuffer();
	unk1D0 = (JAISeParameter*)unk1F4->allocHeap(
	    JAIGlobalParameter::seRegistMax
	    * JAIGlobalParameter::getParamSeCategoryMax() * sizeof(JAISeParameter));
	initSeParaLinkBuffer();
	unk1DC = (JAIStreamParameter*)unk1F4->allocHeap(
	    JAIGlobalParameter::streamParameterBufferMax
	    * sizeof(JAIStreamParameter));
	initStreamParaLinkBuffer();
	for (int i = 0; i < JAIGlobalParameter::seRegistMax
	                        * JAIGlobalParameter::getParamSeCategoryMax();
	     ++i) {
		initSePara(&unk1D0[i]);
	}

	unk1EC = (JAIHeapBlock*)unk1F4->allocHeap(JAIGlobalParameter::autoHeapMax
	                                          * sizeof(JAIHeapBlock));
	for (int i = 0; i < JAIGlobalParameter::autoHeapMax; ++i) {
		unk1EC[i].unk0  = 0;
		unk1EC[i].unkC  = 0;
		unk1EC[i].unk8  = -1;
		unk1EC[i].unk10 = -1;
	}

	unk1F0 = (JAIHeapBlock*)unk1F4->allocHeap(JAIGlobalParameter::stayHeapMax
	                                          * sizeof(JAIHeapBlock));
	for (int i = 0; i < JAIGlobalParameter::stayHeapMax; ++i) {
		unk1F0[i].unk0  = 0;
		unk1F0[i].unkC  = 0;
		unk1F0[i].unk8  = -1;
		unk1F0[i].unk10 = -1;
	}

	unk1E0 = (u32*)unk1F4->allocHeap(JAIGlobalParameter::seqPlayTrackMax * 4);
	unk180 = (JAISeqUpdateData*)unk1F4->allocHeap(
	    JAIGlobalParameter::seqPlayTrackMax * sizeof(JAISeqUpdateData));
	for (int i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
		unk180[i].unk4C
		    = (JAISeqUpdateData::FabricatedUnk4CStruct*)unk1F4->allocHeap(
		        0x7BC);
		unk1E0[i]       = 0;
		unk180[i].unk0  = 0;
		unk180[i].unk1  = 0;
		unk180[i].unk2  = 0;
		unk180[i].unk3  = 0;
		unk180[i].unk8  = 0;
		unk180[i].unk48 = 0;

		unk180[i].unk24 = (f32*)unk1F4->allocHeap(
		    JAIGlobalParameter::seqTrackMax * sizeof(f32));
		unk180[i].unk30 = (f32*)unk1F4->allocHeap(
		    JAIGlobalParameter::seqTrackMax * sizeof(f32));
		unk180[i].unk28 = (f32*)unk1F4->allocHeap(
		    JAIGlobalParameter::seqTrackMax * sizeof(f32));
		unk180[i].unk2C = (f32*)unk1F4->allocHeap(
		    JAIGlobalParameter::seqTrackMax * sizeof(f32));
		unk180[i].unk34 = (f32*)unk1F4->allocHeap(
		    JAIGlobalParameter::seqTrackMax * sizeof(f32));
		unk180[i].unk44 = (u32*)unk1F4->allocHeap(
		    (JAIGlobalParameter::seqTrackMax + 1) * sizeof(u32));
		initSeqTrackInfoParameter(i);
	}
	unk184 = (JAIStreamUpdateParameter*)unk1F4->allocHeap(
	    sizeof(JAIStreamUpdateParameter));
	unk184->unk0  = 0;
	unk184->unk1  = 0;
	unk184->unk2  = 0;
	unk184->unk4  = 1.0f;
	unk184->unk8  = 1.0f;
	unk184->unkC  = 0.5f;
	unk184->unk10 = 0;
	unk184->unk14 = nullptr;

	if (unk1F4->unk68) {
		unk4 = unk1F4->unk68;
	} else {
		unk4 = (u8**)unk1F4->allocHeap(JAIGlobalParameter::soundSceneMax * 4);
		for (int i = 0; i < JAIGlobalParameter::soundSceneMax; ++i)
			unk4[i] = JAIConst::sCInfos_0;
	}

	if (unk1F4->unk6C) {
		unk188 = *unk1F4->unk6C;

		unk1AC = (JASystem::DSPInterface::FxlineConfig_**)unk1F4->allocHeap(
		    unk188 * sizeof(JASystem::DSPInterface::FxlineConfig_*));
		for (int i = 0; i < unk188; ++i) {
			// TODO:
		}
		for (int i = 0; i < 4; ++i) {
			if (!unk18C[i])
				continue;
			unk19C[i] = (s16*)unk1F4->allocHeap(unk18C[i] * 0xA0);
			JASystem::DSPInterface::getFXHandle(i)->setFXLine(unk19C[i],
			                                                  &unk1AC[0][i]);
		}
	}
}

void JAIData::initInfoDataWork(JAISoundTable* soundTable, char* path)
{
	u32 size;
	if (!soundTable->unk78)
		size = JASystem::Dvd::checkFile(path);
	else
		size = soundTable->unk28;

	if (size == 0)
		return;

	if (!soundTable->unk78) {
		soundTable->unk78 = unk1F4->allocHeap(size);
		JASystem::Dvd::loadFile(path, soundTable->unk78);
		soundTable->unk28 = size;
	}
	soundTable->unk0 = *((u8*)soundTable->unk78 + 3);
	// TODO: WTF???
	soundTable->unk2C = &path;
	for (u8 i = 0; i < 18; ++i) {
		// TODO: initializing soundTable from the disc data,
		// but it's really weird
	}
}

void JAIData::reloadInfoDataWork(JAISoundTable* soundTable) { }

void JAIData::setInfoDataPointer(JAISoundTable* soundTable, u8* ptr) { }
