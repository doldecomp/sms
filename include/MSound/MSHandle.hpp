#ifndef MSHANDLE_HPP
#define MSHANDLE_HPP

#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <dolphin/types.h>
#include <dolphin/mtx.h>

struct SeCategory {
	u32 mType;
	f32 unk4;
	f32 unk8;
	f32 unkC;
};

class MSHandle : public JAISound {
public:
	static SeCategory smSeCategory[];
	static f32 smACosPrm[];
	static f32 cPan_MaxAmp;
	static f32 cPan_CAdjust;
	static f32 cPan_CShift;
	static f32 cPan_HiSence_Dist;
	static f32 cMS_DistanceMax_Sence;
	static f32 cDol_0Rad;
	static f32 cDol_HalfRad;
	static f32 cDol_FullRad;

	virtual void setSeDistanceParameters();
	virtual void setSeDistanceVolume(u8 param);
	virtual void setSeDistancePan(u8 param);
	virtual void setSeDistancePitch(u8 param);
	virtual void setSeDistanceDolby(u8 param);
	virtual f32 setDistanceVolumeCommon(f32 volume, u8 param);

	f32 calcVolume(f32 param1, f32 param2, f32 param3, u8 param4, u8 param5);
	f32 calcPan(const Vec& vec, f32 param1, f32 param2);
	f32 calcDolby(const Vec& vec, f32 param);
	f32 MSACos(f32 param);
};

#endif // MSHANDLE_HPP
