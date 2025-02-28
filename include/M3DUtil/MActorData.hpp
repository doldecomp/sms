#ifndef M3DUTIL_M_ACTOR_DATA_HPP
#define M3DUTIL_M_ACTOR_DATA_HPP

#include <JSystem/JGadget/std-list.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DAnmLoader.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <printf.h>

class J3DModelData;
class J3DAnmTransformKey;
class J3DAnmColorKey;
class J3DAnmTexPattern;
class J3DAnmTextureSRTKey;
class J3DAnmTevRegKey;
class J3DAnmClusterKey;
class SampleCtrlModelData;

class MActorAnmDataBase {
public:
	MActorAnmDataBase(int param_1)
	{
		unk0 = param_1;
		unk8 = new const char*[unk0];
		unk4 = new u16[unk0];
		unkC = nullptr;
	}

	void sortByFileNameRaw(void**);
	void checkLower(const char*);

	// fabricated
	int getAnmNum() const { return unk0; }

public:
	/* 0x0 */ int unk0;
	/* 0x4 */ u16* unk4;
	/* 0x8 */ const char** unk8;
	/* 0xC */ J3DAnmBase** unkC;
};

template <class T> class MActorAnmDataEach : public MActorAnmDataBase {
public:
	MActorAnmDataEach(int param_1)
	    : MActorAnmDataBase(param_1)
	{
	}

	// TODO: fake, get rid of it
	void loadAnmPtrArray2(const char* param_1, const char* param_2)
	{
		loadAnmPtrArray(param_1, param_2);
	}

	void loadAnmPtrArray(const char* param_1, const char* param_2)
	{
		unkC = new J3DAnmBase*[unk0];
		for (int i = 0; i < unk0; ++i) {
			char buf[256];
			if (*unk8[i] != '/') {
				char tmp[256];
				snprintf(tmp, 0xff, "%s%s", param_1, unk8[i]);
				snprintf(buf, 0xff, "%s%s", tmp, param_2);
			} else {
				snprintf(buf, 0xff, "%s%s", unk8[i], param_2);
			}
			void* res = JKRGetResource(buf);
			if (res)
				unkC[i] = J3DAnmLoaderDataBase::load(res);
		}
		char trash[8];
		sortByFileNameRaw(unkC);
	}

	T* getAnmPtr(int idx) const
	{
		if (idx < unk0)
			return static_cast<T*>(unkC[idx]);
		return nullptr;
	}
};

struct MActorSubAnmInfo {
	/* 0x0 */ u32 unk0;
	/* 0x4 */ const char* unk4;
};

class MActorAnmData {
public:
	MActorAnmData();
	~MActorAnmData() { }

	void createSampleModelData(J3DModelData*);
	void addFileTable(const char*);
	void getSimpleName(const char*);
	void addFileNum(const char*);
	void init(const char*, const char**);
	void addIncidentalAnm(const char*, int);
	u32 partsNameToIdx(const char*);

public:
	/* 0x0 */ u32 unk0;
	/* 0x4 */ int unk4;
	/* 0x8 */ int unk8;
	/* 0xC */ int unkC;
	/* 0x10 */ int unk10;
	/* 0x14 */ int unk14;
	/* 0x18 */ int unk18;
	/* 0x1C */ JGadget::TList<MActorSubAnmInfo> unk1C;
	/* 0x2C */ MActorAnmDataEach<J3DAnmTransformKey>* unk2C;
	/* 0x30 */ MActorAnmDataEach<J3DAnmColorKey>* unk30;
	/* 0x34 */ MActorAnmDataEach<J3DAnmTexPattern>* unk34;
	/* 0x38 */ MActorAnmDataEach<J3DAnmTextureSRTKey>* unk38;
	/* 0x3C */ MActorAnmDataEach<J3DAnmTevRegKey>* unk3C;
	/* 0x40 */ MActorAnmDataEach<J3DAnmClusterKey>* unk40;
	/* 0x44 */ u32 unk44;
	/* 0x48 */ SampleCtrlModelData* unk48;
};

#endif
