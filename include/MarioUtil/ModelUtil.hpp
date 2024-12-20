#ifndef MODEL_UTIL_HPP
#define MODEL_UTIL_HPP

#include <JSystem/J3DModel.hpp>
#include <JSystem/J3DAnmLoader.hpp>

struct J3DFrameCtrl;

struct TMultiBtk {
	int unk00;
	J3DAnmTextureSRTKey** unk04;
	J3DModelData* unk08;
	J3DFrameCtrl* unk0c;

	TMultiBtk(int, J3DModelData*);
	void update();
	void setNthData(int, J3DAnmTextureSRTKey*);
};

#endif
