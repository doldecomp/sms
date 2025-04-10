#ifndef DRAW_UTIL_HPP
#define DRAW_UTIL_HPP

#include <types.h>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGeometry.hpp>

class JUTTexture;

class TSilhouette;

extern TSilhouette* gpSilhouetteManager;

class TSilhouette : public JDrama::TNameRef {
public:
	TSilhouette();

	void calcSilhouetteBorder();
	void setting(float (*)[4]);

	virtual ~TSilhouette() {};
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);

	bool isUnk48Positive()
	{
		return gpSilhouetteManager->unk48 > 0.0f ? true : false;
	}

public:
	/* 0xC */ char unkC[6];
	/* 0x12 */ GXColor unk12;
	/* 0x16 */ GXColor unk16;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 unk30;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ f32 unk38;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ JUTTexture* unk40;
	/* 0x44 */ JUTTexture* unk44;
	/* 0x48 */ f32 unk48;
	/* 0x4C */ f32 unk4C;
	/* 0x50 */ f32 unk50;
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
