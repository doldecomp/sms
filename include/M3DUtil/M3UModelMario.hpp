#ifndef M3DUTIL_M3U_MODEL_MARIO_HPP
#define M3DUTIL_M3U_MODEL_MARIO_HPP

#include <M3DUtil/M3UModel.hpp>

class M3UModelCommonMario : public M3UModelCommon {
public:
	virtual void getMtxCalc(const M3UMtxCalcSetInfo&);

public:
};

class M3UModelMario : public M3UModel {
public:
	virtual void changeMtxCalcSIAnmBQAnmTransform(int, u8);
	virtual void updateIn();
	virtual void updateInMotion();
	virtual void updateOut();

public:
	// TODO:
};

#endif
