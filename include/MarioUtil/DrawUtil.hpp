#ifndef DRAW_UTIL_HPP
#define DRAW_UTIL_HPP

#include <types.h>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGeometry.hpp>

class TSilhouette : public JDrama::TNameRef {
public:
	TSilhouette();

	void calcSilhouetteBorder();
	void setting(float (*)[4]);

	virtual ~TSilhouette() {};
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
};

class J3DModel;

class TTrembleModelEffect {
public:
	void reset();
	void movement();
	void clash(float);
	void tremble(float, float, float, int);
	void init(J3DModel*);
};

class J3DMaterial;
class J3DModelData;
class J3DShape;

void SMS_UnifyMaterial(J3DModel*);
void SMS_CalcMatAnmAndMakeDL(J3DModel*, u16);
void SMS_ShowJoint(J3DMaterial*, bool);
void SMS_DrawInit();
void SMS_MakeDLAndLock(J3DModel*);
void SMS_DrawShape(J3DModelData*, u16);
void SMS_SettingDrawShape(J3DModelData*, u16);
void SMS_DrawCube(const JGeometry::TVec3<float>&,
                  const JGeometry::TVec3<float>&);
void SMS_CountPolygonNumInShape(J3DShape*);
void SMS_ResetDamageFogEffect(J3DModelData*);
void SMS_AddDamageFogEffect(J3DModelData*, const JGeometry::TVec3<float>&,
                            JDrama::TGraphics*);

#endif
