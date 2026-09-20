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
	MActorAnmDataBase(int anm_num);

	void sortByFileNameRaw(void** anms);
	void checkLower(const char*);

	// fabricated
	int getAnmNum() const { return mAnmNum; }
	u16 getKeyCode(int i) { return mAnmKeyCodes[i]; }
	const char* getName(int i) { return mAnmNames[i]; }

public:
	/* 0x0 */ int mAnmNum;
	/* 0x4 */ u16* mAnmKeyCodes;
	/* 0x8 */ const char** mAnmNames;
	/* 0xC */ J3DAnmBase** mAnimations;
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

	void loadAnmPtrArray(const char* directory, const char* extension)
	{
		mAnimations = new J3DAnmBase*[mAnmNum];
		for (int i = 0; i < mAnmNum; ++i) {
			char buf[256];
			if (*mAnmNames[i] != '/') {
				char tmp[256];
				snprintf(tmp, 0xff, "%s%s", directory, mAnmNames[i]);
				snprintf(buf, 0xff, "%s%s", tmp, extension);
			} else {
				snprintf(buf, 0xff, "%s%s", mAnmNames[i], extension);
			}
			void* res = JKRGetResource(buf);
			if (res)
				mAnimations[i] = J3DAnmLoaderDataBase::load(res);
		}

		sortByFileNameRaw(mAnimations);
	}

	T* getAnmPtr(int idx) const
	{
		if (idx < mAnmNum)
			return static_cast<T*>(mAnimations[idx]);
		return nullptr;
	}
};

struct MActorSubAnmInfo {
	/* 0x0 */ u16 unk0;
	/* 0x4 */ const char* unk4;
};

/**
 * @brief A library of animations shared by multiple MActors.
 */
class MActorAnmData {
public:
	MActorAnmData();
	~MActorAnmData() { }

	void createSampleModelData(J3DModelData*);
	void addFileTable(const char*);
	char* getSimpleName(const char*);
	void addFileNum(const char*);
	void init(const char*, const char**);
	void addIncidentalAnm(const char*, int);
	u32 partsNameToIdx(const char*);

	// fabricated
	s32 getUnk0() { return unk0; }
	SampleCtrlModelData* getUnk48() { return unk48; }
	MActorAnmDataEach<J3DAnmTransformKey>* getUnk2C() { return mBckAnms; }
	MActorAnmDataEach<J3DAnmColorKey>* getUnk30() { return mBpkAnms; }
	MActorAnmDataEach<J3DAnmTexPattern>* getUnk34() { return mBtpAnms; }
	MActorAnmDataEach<J3DAnmTextureSRTKey>* getUnk38() { return mBtkAnms; }
	MActorAnmDataEach<J3DAnmTevRegKey>* getUnk3C() { return mBrkAnms; }
	MActorAnmDataEach<J3DAnmClusterKey>* getUnk40() { return mBlkAnms; }

public:
	/* 0x0 */ int unk0;
	/* 0x4 */ int mBckNum;
	/* 0x8 */ int mBlkNum;
	/* 0xC */ int mBpkNum;
	/* 0x10 */ int mBtpNum;
	/* 0x14 */ int mBtkNum;
	/* 0x18 */ int mBrkNum;
	/* 0x1C */ JGadget::TList<MActorSubAnmInfo> unk1C;
	/* 0x2C */ MActorAnmDataEach<J3DAnmTransformKey>* mBckAnms;
	/* 0x30 */ MActorAnmDataEach<J3DAnmColorKey>* mBpkAnms;
	/* 0x34 */ MActorAnmDataEach<J3DAnmTexPattern>* mBtpAnms;
	/* 0x38 */ MActorAnmDataEach<J3DAnmTextureSRTKey>* mBtkAnms;
	/* 0x3C */ MActorAnmDataEach<J3DAnmTevRegKey>* mBrkAnms;
	/* 0x40 */ MActorAnmDataEach<J3DAnmClusterKey>* mBlkAnms;
	/* 0x44 */ u32 unk44;
	/* 0x48 */ SampleCtrlModelData* unk48;
};

u16 MActorCalcKeyCode(const char* name);

#endif
