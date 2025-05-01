#ifndef CAMERA_CAMERA_SHAKE_HPP
#define CAMERA_CAMERA_SHAKE_HPP

#include <JSystem/JGeometry.hpp>

class TCamSaveShake;

enum EnumCamShakeMode {
	CAM_SHAKE_MODE_UNK5 = 5,
};

class TCameraShake;

extern TCameraShake* gpCameraShake;

class TCameraShake {
public:
	struct TCamShakeAngle { };

	struct TCamShakeInfo { };

	TCameraShake();
	void getUseShakeData_();
	void setShakeAngleOne_(TCameraShake::TCamShakeAngle*, f32, s16, u16, f32);
	void setShakeAngleAll_(TCameraShake::TCamShakeInfo*, const TCamSaveShake*,
	                       u16, f32);
	void startShake(EnumCamShakeMode, f32);
	void keepShake(EnumCamShakeMode, f32);
	void execShake(const JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>*,
	               JGeometry::TVec3<f32>*);
};

#endif
