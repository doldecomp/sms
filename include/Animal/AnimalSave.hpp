#ifndef ANIMAL_SAVE_HPP
#define ANIMAL_SAVE_HPP

#include <System/Params.hpp>
#include <System/ParamInst.hpp>

class TAnimalSaveIndividual : public TParams {
public:
	TAnimalSaveIndividual(const char* prm);

	/* 0x08 */ TParamRT<s32> mSLSharedAnmNum;
	/* 0x1C */ TParamRT<f32> mSLViewClipNear;
	/* 0x30 */ TParamRT<f32> mSLViewClipFar;
	/* 0x44 */ TParamRT<f32> mSLViewClipRadius;
	/* 0x58 */ TParamRT<f32> mSLMaxMarchSpeed;
	/* 0x6C */ TParamRT<f32> mSLMarchAccel;
	/* 0x80 */ TParamRT<f32> mSLMarchDecrease;
	/* 0x94 */ TParamRT<f32> mSLWalkTurnSpeed;
	/* 0xA8 */ TParamRT<f32> mSLWaitTurnSpeed;
};

#endif
