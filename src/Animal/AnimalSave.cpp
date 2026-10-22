#include "Animal/AnimalSave.hpp"

#include <System/DummyStrings.hpp>

TAnimalSaveIndividual::TAnimalSaveIndividual(const char* prm)
    : TParams(prm)
    , PARAM_INIT(mSLSharedAnmNum, 4)
    , PARAM_INIT(mSLViewClipNear, 100.0f)
    , PARAM_INIT(mSLViewClipFar, 10000.0f)
    , PARAM_INIT(mSLViewClipRadius, 75.0f)
    , PARAM_INIT(mSLMaxMarchSpeed, 6.0f)
    , PARAM_INIT(mSLMarchAccel, 0.04f)
    , PARAM_INIT(mSLMarchDecrease, 0.2f)
    , PARAM_INIT(mSLWalkTurnSpeed, 10.0f)
    , PARAM_INIT(mSLWaitTurnSpeed, 2.0f)
{
	load(mPrmPath);
}
