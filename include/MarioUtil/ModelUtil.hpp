#ifndef MODEL_UTIL_HPP
#define MODEL_UTIL_HPP

#include <Player/MarioMain.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DAnmLoader.hpp>

class J3DFrameCtrl;
class J3DAnmTextureSRTKey;
class J3DModelData;
class J3DModel;
class SDLModel;

struct TMultiBtk {
	int unk00;
	J3DAnmTextureSRTKey** unk04;
	J3DModelData* unk08;
	J3DFrameCtrl* unk0c;

	TMultiBtk(int, J3DModelData*);
	void update();
	void setNthData(int, J3DAnmTextureSRTKey*);
};

SDLModel* SMS_CreateMinimumSDLModel(const char*);
J3DModel* SMS_CreatePartsModel(char*, unsigned long);
void SMS_RideMoveCalcLocalPos(TRidingInfo*, const JGeometry::TVec3<float>&);
void SMS_RideMoveByGroundActor(TRidingInfo*, JGeometry::TVec3<float>*, float*);
void SMS_DumpJ3DModel(J3DModel*);

#endif
