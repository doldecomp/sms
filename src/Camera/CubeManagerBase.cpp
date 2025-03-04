#include <Camera/CubeManagerBase.hpp>
#include <Camera/cameralib.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

TCubeManagerArea* gpCubeArea;
TCubeManagerFast* gpCubeFastA;
TCubeManagerFast* gpCubeFastB;
TCubeManagerFast* gpCubeFastC;

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

u32 TCubeManagerBase::getDataNo(s32 i) const
{
	u32 result = -1;
	if (i >= 0 && i < unk10)
		result = (*unk14)[i].unk34;
	return result;
}

u32 TCubeManagerBase::getInCubeNo(const Vec& v) const
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

bool TCubeManagerArea::isInAreaCube(const Vec&) const { }

bool TCubeManagerFast::isInOtherCube(const Vec&) const { }

bool SMS_IsInOtherFastCube(const Vec&) { }

bool SMS_IsInSameCameraCube(const Vec&) { }
