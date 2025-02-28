#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/SampleCtrlModel.hpp>
#include <JSystem/JKernel/JKRFileFinder.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

void MActorAnmData::createSampleModelData(J3DModelData* data)
{
	unk48 = new SampleCtrlModelData(data);
}

void MActorAnmData::addFileTable(const char* param_1)
{
	char* pcVar1;
	size_t sVar2;
	size_t sVar3;
	u16 uVar4;
	u32 uVar5;

	pcVar1 = strstr(param_1, ".bck");
	if (pcVar1 != (char*)0x0) {
		sVar2  = strlen(param_1);
		pcVar1 = strrchr(param_1, 0x2e);
		sVar3  = strlen(pcVar1);
		uVar5  = sVar2 - (sVar3 - 1);
		pcVar1 = new char[uVar5];
		snprintf(pcVar1, uVar5, "%s", param_1);
		uVar4             = 0;
		unk2C->unk8[unk4] = pcVar1;
		while (*pcVar1 != '\0') {
			uVar4 = *pcVar1++ + uVar4 * 5;
		}
		unk2C->unk4[unk4] = uVar4;
		++unk4;
	}

	pcVar1 = strstr(param_1, ".bpk");
	if (pcVar1 != (char*)0x0) {
		sVar2  = strlen(param_1);
		pcVar1 = strrchr(param_1, 0x2e);
		sVar3  = strlen(pcVar1);
		uVar5  = sVar2 - (sVar3 - 1);
		pcVar1 = new char[uVar5];
		snprintf(pcVar1, uVar5, "%s", param_1);
		uVar4             = 0;
		unk30->unk8[unkC] = pcVar1;
		while (*pcVar1 != '\0') {
			uVar4 = *pcVar1++ + uVar4 * 5;
		}
		unk30->unk4[unkC] = uVar4;
		++unkC;
	}

	pcVar1 = strstr(param_1, ".btp");
	if (pcVar1 != (char*)0x0) {
		sVar2  = strlen(param_1);
		pcVar1 = strrchr(param_1, 0x2e);
		sVar3  = strlen(pcVar1);
		uVar5  = sVar2 - (sVar3 - 1);
		pcVar1 = new char[uVar5];
		snprintf(pcVar1, uVar5, "%s", param_1);
		uVar4              = 0;
		unk34->unk8[unk10] = pcVar1;
		while (*pcVar1 != '\0') {
			uVar4 = *pcVar1++ + uVar4 * 5;
		}
		unk34->unk4[unk10] = uVar4;
		++unk10;
	}

	pcVar1 = strstr(param_1, ".btk");
	if (pcVar1 != (char*)0x0) {
		sVar2  = strlen(param_1);
		pcVar1 = strrchr(param_1, 0x2e);
		sVar3  = strlen(pcVar1);
		uVar5  = sVar2 - (sVar3 - 1);
		pcVar1 = new char[uVar5];
		snprintf(pcVar1, uVar5, "%s", param_1);
		uVar4              = 0;
		unk38->unk8[unk14] = pcVar1;
		while (*pcVar1 != '\0') {
			uVar4 = *pcVar1++ + uVar4 * 5;
		}
		unk38->unk4[unk14] = uVar4;
		++unk14;
	}

	pcVar1 = strstr(param_1, ".brk");
	if (pcVar1 != (char*)0x0) {
		sVar2  = strlen(param_1);
		pcVar1 = strrchr(param_1, 0x2e);
		sVar3  = strlen(pcVar1);
		uVar5  = sVar2 - (sVar3 - 1);
		pcVar1 = new char[uVar5];
		snprintf(pcVar1, uVar5, "%s", param_1);
		uVar4              = 0;
		unk3C->unk8[unk18] = pcVar1;
		while (*pcVar1 != '\0') {
			uVar4 = *pcVar1++ + uVar4 * 5;
		}
		unk3C->unk4[unk18] = uVar4;
		++unk18;
	}

	pcVar1 = strstr(param_1, ".blk");
	if (pcVar1 != (char*)0x0) {
		sVar2  = strlen(param_1);
		pcVar1 = strrchr(param_1, 0x2e);
		sVar3  = strlen(pcVar1);
		uVar5  = sVar2 - (sVar3 - 1);
		pcVar1 = new char[uVar5];
		snprintf(pcVar1, uVar5, "%s", param_1);
		uVar4             = 0;
		unk40->unk8[unk8] = pcVar1;
		while (*pcVar1 != '\0') {
			uVar4 = *pcVar1++ + uVar4 * 5;
		}
		unk40->unk4[unk8] = uVar4;
		++unk8;
	}
}

void MActorAnmData::addFileNum(const char* name)
{
	if (strstr(name, ".bck"))
		++unk4;
	if (strstr(name, ".bpk"))
		++unkC;
	if (strstr(name, ".btp"))
		++unk10;
	if (strstr(name, ".btk"))
		++unk14;
	if (strstr(name, ".brk"))
		++unk18;
	if (strstr(name, ".blk"))
		++unk8;
}

void MActorAnmData::init(const char* param_1, const char** param_2)
{
	char thing[256];
	int uMVar1;

	if (*param_1 != '/')
		uMVar1 = snprintf(thing, 0xff, "%s%s", "/", param_1);
	else
		uMVar1 = snprintf(thing, 0xff, "%s", param_1);

	if (uMVar1 < 0 || uMVar1 > 0xfe)
		return;

	char thing2[256];
	snprintf(thing2, 0xff, "%s%s", thing, "/");

	JKRFileFinder* fileFinder = JKRFileLoader::findFirstFile(thing2);

	JKRFileFinder* finder = fileFinder;
	do {
		addFileNum(finder->mBase.mFileName);
	} while (finder->findNextFile());

	if (param_2 != nullptr)
		for (int i = 0; i == 0 || param_2[i] != nullptr; ++i)
			addFileNum(param_2[i]);

	delete fileFinder;

	if (unk4 > 0)
		unk2C = new MActorAnmDataEach<J3DAnmTransformKey>(unk4);
	if (unkC > 0)
		unk30 = new MActorAnmDataEach<J3DAnmColorKey>(unkC);
	if (unk10 > 0)
		unk34 = new MActorAnmDataEach<J3DAnmTexPattern>(unk10);
	if (unk14 > 0)
		unk38 = new MActorAnmDataEach<J3DAnmTextureSRTKey>(unk14);
	if (unk18 > 0)
		unk3C = new MActorAnmDataEach<J3DAnmTevRegKey>(unk18);
	if (unk8 > 0)
		unk40 = new MActorAnmDataEach<J3DAnmClusterKey>(unk8);

	unk4  = 0;
	unk8  = 0;
	unkC  = 0;
	unk10 = 0;
	unk14 = 0;
	unk18 = 0;

	fileFinder = JKRFileLoader::findFirstFile(thing2);
	do {
		strstr(fileFinder->mBase.mFileName, "#");
		addFileTable(fileFinder->mBase.mFileName);
	} while (fileFinder->findNextFile());

	if (param_2 != nullptr && *param_2 != nullptr) {
		for (int i = 0; i == 0 || param_2[i] != nullptr; ++i)
			addFileTable(param_2[i]);
	}

	delete fileFinder;

	if (unk2C)
		unk2C->loadAnmPtrArray2(thing, ".bck");
	if (unk30)
		unk30->loadAnmPtrArray2(thing, ".bpk");
	if (unk34)
		unk34->loadAnmPtrArray2(thing, ".btp");
	if (unk38)
		unk38->loadAnmPtrArray2(thing, ".btk");
	if (unk3C)
		unk3C->loadAnmPtrArray2(thing, ".brk");
	if (unk40)
		unk40->loadAnmPtrArray2(thing, ".blk");
}

u32 MActorAnmData::partsNameToIdx(const char* name)
{
	JGadget::TList<MActorSubAnmInfo>::iterator e;
	JGadget::TList<MActorSubAnmInfo>::iterator it;
	it      = unk1C.begin();
	e       = unk1C.end();
	u32 idx = 0;
	for (; it != e; ++idx, ++it)
		if (strcmp(it->unk4, name) == 0)
			return idx;
	return -1;
}

u16 MActorCalcKeyCode(const char* name)
{
	u32 result = 0;
	while (*name != '\0') {
		result = *name++ + result * 5;
	}
	return result;
}

MActorAnmData::MActorAnmData()
{
	unk2C = nullptr;
	unk30 = nullptr;
	unk34 = nullptr;
	unk38 = nullptr;
	unk3C = nullptr;
	unk40 = nullptr;

	unk44 = 0;
	unk48 = nullptr;

	unk4  = 0;
	unk8  = 0;
	unkC  = 0;
	unk10 = 0;
	unk14 = 0;
	unk18 = 0;
}

inline int to_upper(int c)
{
	if (c >= 'a' && c <= 'z')
		return c + -0x20;
	else
		return c;
}

void MActorAnmDataBase::sortByFileNameRaw(void** param_1) { }

void MActorAnmDataBase::checkLower(const char* param_1)
{
	for (int iVar1 = 0; iVar1 < unk0; ++iVar1) {
		const char* pcVar4 = unk8[iVar1];
		const char* pcVar5 = param_1;
		while (*pcVar5 != '\0' && *pcVar4 != '\0') {

			if (to_upper(*pcVar5) > to_upper(*pcVar4)
			    || to_upper(*pcVar5) < to_upper(*pcVar4))
				break;

			++pcVar5;
			++pcVar4;
		}
	}
}
