#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/SampleCtrlModel.hpp>
#include <JSystem/JKernel/JKRFileFinder.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

static int to_upper_hack(int c)
{
	if (c >= 'a' && c <= 'z')
		return c + ('A' - 'a');

	return c;
}

static int strcmp_ignore_case(const char* fst, const char* snd)
{
	while (*fst != '\0' && *snd != '\0') {
		if (to_upper_hack(*fst) > to_upper_hack(*snd))
			return -1;

		if (to_upper_hack(*fst) < to_upper_hack(*snd))
			return 1;

		++fst;
		++snd;
	}

	if (to_upper_hack(*fst) > to_upper_hack(*snd))
		return -1;

	if (to_upper_hack(*fst) < to_upper_hack(*snd))
		return 1;

	return 0;
}

void MActorAnmDataBase::checkLower(const char* param_1)
{
	for (int i = 0; i < unk0; ++i) {
		if (strcmp_ignore_case(param_1, unk8[i])) {
			// assert?
		}
	}
}

// UNUSED (Size: 0x50 in MAP)
MActorAnmDataBase::MActorAnmDataBase(int param_1)
{
	unk0 = param_1;
	unk8 = new const char*[unk0];
	unk4 = new u16[unk0];
	unkC = nullptr;
}

void MActorAnmDataBase::sortByFileNameRaw(void** param_1)
{
	if (unk0 > 1) {
		for (int i = 1; i < unk0; ++i) {
			int j;

			const char* str = unk8[i];
			u16 key         = unk4[i];
			void* prm       = param_1[i];

			for (j = i - 1; j >= 0; --j) {

				if (strcmp_ignore_case(str, unk8[j]) < 0)
					break;

				unk8[j + 1]    = unk8[j];
				unk4[j + 1]    = unk4[j];
				param_1[j + 1] = param_1[j];
			}

			unk8[j + 1]    = str;
			unk4[j + 1]    = key;
			param_1[j + 1] = prm;
		}
	}
}

MActorAnmData::MActorAnmData()
    : unk0(0)
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

u16 MActorCalcKeyCode(const char* name)
{
	u32 result = 0;
	while (*name != '\0') {
		result = *name++ + result * 5;
	}
	return result;
}

u32 MActorAnmData::partsNameToIdx(const char* name)
{
	typedef JGadget::TList<MActorSubAnmInfo>::iterator I;
	u32 idx = 0;
	for (I it = unk1C.begin(), e = unk1C.end(); it != e; ++idx, ++it)
		if (strcmp(it->unk4, name) == 0)
			return idx;
	return -1;
}

// UNUSED (Size: 0x58 in MAP)
void MActorAnmData::addIncidentalAnm(const char*, int) { }

void MActorAnmData::init(const char* anm_folder, const char** additional_files)
{
	char fullAnmPath[256];
	int fullAnmPathLength;

	if (*anm_folder != '/')
		fullAnmPathLength
		    = snprintf(fullAnmPath, 0xff, "%s%s", "/", anm_folder);
	else
		fullAnmPathLength = snprintf(fullAnmPath, 0xff, "%s", anm_folder);

	if (fullAnmPathLength < 0 || fullAnmPathLength > 254)
		return;

	char anmFolder[256];
	snprintf(anmFolder, 0xff, "%s%s", fullAnmPath, "/");

	JKRFileFinder* fileFinder = JKRFileLoader::findFirstFile(fullAnmPath);

	JKRFileFinder* finder = fileFinder;
	do {
		addFileNum(finder->mBase.mFileName);
	} while (finder->findNextFile());

	if (additional_files != nullptr)
		for (int i = 0; i == 0 || additional_files[i] != nullptr; ++i)
			addFileNum(additional_files[i]);

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

	fileFinder = JKRFileLoader::findFirstFile(fullAnmPath);
	do {
		strstr(fileFinder->mBase.mFileName, "#");
		addFileTable(fileFinder->mBase.mFileName);
	} while (fileFinder->findNextFile());

	if (additional_files != nullptr && *additional_files != nullptr) {
		for (int i = 0; i == 0 || additional_files[i] != nullptr; ++i)
			addFileTable(additional_files[i]);
	}

	delete fileFinder;

	if (unk2C)
		unk2C->loadAnmPtrArray2(anmFolder, ".bck");
	if (unk30)
		unk30->loadAnmPtrArray2(anmFolder, ".bpk");
	if (unk34)
		unk34->loadAnmPtrArray2(anmFolder, ".btp");
	if (unk38)
		unk38->loadAnmPtrArray2(anmFolder, ".btk");
	if (unk3C)
		unk3C->loadAnmPtrArray2(anmFolder, ".brk");
	if (unk40)
		unk40->loadAnmPtrArray2(anmFolder, ".blk");
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

// UNUSED (Size: 0x80 in MAP). Inlined into every addFileTable() branch;
// the body below is what those branches compile to in the ROM.
char* MActorAnmData::getSimpleName(const char* file_name)
{
	u32 length = strlen(file_name) - (strlen(strrchr(file_name, '.')) - 1);
	char* simple_name = new char[length];
	snprintf(simple_name, length, "%s", file_name);
	return simple_name;
}

void MActorAnmData::addFileTable(const char* param_1)
{
	if (strstr(param_1, ".bck") != nullptr) {
		char* simple_name = getSimpleName(param_1);
		unk2C->unk8[unk4] = simple_name;
		u16 key           = MActorCalcKeyCode(simple_name);
		unk2C->unk4[unk4] = key;
		++unk4;
	}

	if (strstr(param_1, ".bpk") != nullptr) {
		char* simple_name = getSimpleName(param_1);
		unk30->unk8[unkC] = simple_name;
		u16 key           = MActorCalcKeyCode(simple_name);
		unk30->unk4[unkC] = key;
		++unkC;
	}

	if (strstr(param_1, ".btp") != nullptr) {
		char* simple_name  = getSimpleName(param_1);
		unk34->unk8[unk10] = simple_name;
		u16 key            = MActorCalcKeyCode(simple_name);
		unk34->unk4[unk10] = key;
		++unk10;
	}

	if (strstr(param_1, ".btk") != nullptr) {
		char* simple_name  = getSimpleName(param_1);
		unk38->unk8[unk14] = simple_name;
		u16 key            = MActorCalcKeyCode(simple_name);
		unk38->unk4[unk14] = key;
		++unk14;
	}

	if (strstr(param_1, ".brk") != nullptr) {
		char* simple_name  = getSimpleName(param_1);
		unk3C->unk8[unk18] = simple_name;
		u16 key            = MActorCalcKeyCode(simple_name);
		unk3C->unk4[unk18] = key;
		++unk18;
	}

	if (strstr(param_1, ".blk") != nullptr) {
		char* simple_name = getSimpleName(param_1);
		unk40->unk8[unk8] = simple_name;
		u16 key           = MActorCalcKeyCode(simple_name);
		unk40->unk4[unk8] = key;
		++unk8;
	}
}

void MActorAnmData::createSampleModelData(J3DModelData* data)
{
	unk48 = new SampleCtrlModelData(data);
}
