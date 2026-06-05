#include <Camera/CubeManagerBase.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <System/MarDirector.hpp>
#include <Camera/cameralib.hpp>
#include <Player/MarioAccess.hpp>
#include <string.h>

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

TCubeManagerBase* gpCubeCamera;
TCubeManagerBase* gpCubeMirror;
TCubeManagerBase* gpCubeWire;
TCubeManagerBase* gpCubeStream;
TCubeManagerBase* gpCubeShadow;
TCubeManagerArea* gpCubeArea;
TCubeManagerFast* gpCubeFastA;
TCubeManagerFast* gpCubeFastB;
TCubeManagerFast* gpCubeFastC;
TCubeManagerBase* gpCubeSoundChange;
TCubeManagerBase* gpCubeSoundEffect;

TCubeManagerBase::TCubeManagerBase(const char* name, const char* param_2)
    : JDrama::TViewObj(name)
    , unk10(0)
    , unk14(nullptr)
    , unk18(param_2)
{
}

TCubeManagerBase::TCubeManagerBase(const char* name, u8 param_2)
    : JDrama::TViewObj(name)
    , unk10(param_2)
    , unk14(nullptr)
    , unk18(nullptr)
{
	initializer();
}

void TCubeManagerBase::initializer()
{
	unk14 = new TNameRefPtrAryT<TCubeGeneralInfo>;
	unk14->reserve(unk10);
	for (int i = 0; i < unk10; ++i)
		unk14->push_back(new TCubeGeneralInfo);
}

void TCubeManagerBase::load(JSUMemoryInputStream& stream)
{
	JDrama::TNameRef::load(stream);
	JDrama::TNameRef* root
	    = JDrama::TNameRefGen::getInstance()->getRootNameRef();
	TNameRefPtrAryT<TCubeGeneralInfo>* ary
	    = (TNameRefPtrAryT<TCubeGeneralInfo>*)root->search(unk18);

	if (ary == nullptr)
		return;

	unk14 = ary;
	unk10 = unk14->size();
}

void TCubeManagerBase::perform(u32, JDrama::TGraphics*) { }

s32 TCubeManagerBase::getDataNo(s32 i) const
{
	u32 result = -1;
	if (i >= 0 && i < unk10)
		result = (*unk14)[i].unk34;
	return result;
}

int TCubeManagerBase::getInCubeNo(const Vec& v) const
{
	for (u32 i = 0; i < unk10; ++i) {
		TCubeGeneralInfo& info = (*unk14)[i];
		if (CLBIsPointInCube(v, info.getUnkC(), info.getUnk18(),
		                     info.getUnk24()))
			return i;
	}

	return -1;
}

bool TCubeManagerBase::isInCube(const Vec& v, s32 i) const
{
	bool result = false;
	if (i >= 0 && i < unk10) {
		TCubeGeneralInfo& info = (*unk14)[i];
		if (CLBIsPointInCube(v, info.getUnkC(), info.getUnk18(),
		                     info.getUnk24()))
			result = true;
	}
	return result;
}

void TCubeManagerBase::calcPointInCubeRatio(const Vec& param_1, s32 param_2,
                                            float* param_3, float* param_4,
                                            float* param_5) const
{
	TCubeGeneralInfo& info = (*unk14)[param_2];
	CLBCalcPointInCubeRatio(param_1, info.getUnkC(), info.getUnk18(),
	                        info.getUnk24(), param_3, param_4, param_5);
}

bool TCubeManagerArea::isInAreaCube(const Vec& pos) const
{
	int found = getInCubeNo(pos);

	if (unk1C == found)
		return true;

	// Presumably hotel delphino floor transitions?
	if (gpMarDirector->getCurrentMap() == 7 && unk1C != -1 && found != -1) {
		const char* curName = (*unk14)[unk1C].getName();
		const char* newName = (*unk14)[found].getName();

		if (strcmp(curName, "３階") == 0) {
			if (strcmp(newName, "２階") == 0 || strcmp(newName, "１階") == 0)
				return true;
		} else if (strcmp(curName, "２階") == 0) {
			if (strcmp(newName, "１階") == 0)
				return true;
		}
	}

	return false;
}

inline bool TCubeManagerFast::isInOtherCube(const Vec& pos) const
{
	bool result = false;
	int in      = getInCubeNo(pos);
	if (unk1C != -1 && in != -1 && unk1C != in)
		result = true;

	return result;
}

bool SMS_IsInOtherFastCube(const Vec& pos)
{
	bool result = false;
	if (!gpMarDirector->isDemoModeNow()
	    && (gpCubeFastA->isInOtherCube(pos) || gpCubeFastB->isInOtherCube(pos)
	        || gpCubeFastC->isInOtherCube(pos)))
		result = true;

	return result;
}

bool SMS_IsInSameCameraCube(const Vec& pos)
{
	bool result  = false;
	Vec marioPos = SMS_GetMarioPos();
	marioPos.y += 75.0f;
	int uVar7 = gpCubeCamera->getInCubeNo(marioPos);
	int uVar4 = gpCubeCamera->getInCubeNo(pos);
	if (uVar7 == uVar4 && uVar7 != -1)
		result = true;
	return result;
}
