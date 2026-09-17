#ifndef DRAW_UTIL_HPP
#define DRAW_UTIL_HPP

#include <types.h>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry.hpp>

class JUTTexture;

class TSilhouette;

extern TSilhouette* gpSilhouetteManager;

class TSilhouette : public JDrama::TViewObj {
public:
	TSilhouette() { }

	void calcSilhouetteBorder();
	void setting(MtxPtr);

	virtual ~TSilhouette() { };
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	bool isUnk48Positive()
	{
		return gpSilhouetteManager->unk48 > 0.0f ? true : false;
	}

public:
	/* 0x10 */ u16 unk10;
	/* 0x12 */ GXColor unk12;
	/* 0x16 */ GXColor unk16;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32 unk24[3];
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
	void clash(f32);
	void tremble(f32, f32, f32, int);
	void init(J3DModel*);

	// Fabricated
	BOOL checkUnk8(u8 flag) { return unk8 & flag ? TRUE : FALSE; }

public:
	/* 0x0 */ J3DModel* unk0;
	/* 0x4 */ void* unk4;
	/* 0x8 */ u8 unk8;
	/* 0x9 */ u8 unk9;
	/* 0xA */ u8 unkA;
	/* 0xB */ char unkB;
	/* 0xC */ s16 unkC;
	/* 0x10 */ s32 unk10;
	/* 0x14 */ JGeometry::TVec3<s16>* unk14;
	/* 0x18 */ JGeometry::TVec3<s16>* unk18[2];
	/* 0x20 */ JGeometry::TVec3<s16>* unk20;
	/* 0x24 */ s16 unk24;
	/* 0x26 */ s16 unk26;
	/* 0x28 */ JGeometry::TVec3<f32>* unk28;
	/* 0x2C */ JGeometry::TVec3<f32>* unk2C[2];
	/* 0x34 */ JGeometry::TVec3<f32>* unk34;
	/* 0x38 */ f32 unk38;
	/* 0x3C */ f32 unk3C;
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
void SMS_DrawCube(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&);
int SMS_CountPolygonNumInShape(J3DShape*);
void SMS_ResetDamageFogEffect(J3DModelData*);
void SMS_AddDamageFogEffect(J3DModelData*, const JGeometry::TVec3<f32>&,
                            JDrama::TGraphics*);

/**
 * @brief Sets up the perspective for clip checks to be done via
 * calls to ViewFrustumClipCheck
 *
 * @param fovy vertical field of view
 * @param aspect aspect ratio
 * @param clip_near near plane
 * @param clip_far far plane
 */
void SetViewFrustumClipCheckPerspective(f32 fovy, f32 aspect, f32 clip_near,
                                        f32 clip_far);
/**
 * @brief Checks whether a sphere at a \p position with a \p radius could
 * possibly intersect the current frustum.
 *
 * @param gfx specifies camera transform
 * @param position center of a sphere
 * @param radius radius of a sphere
 * @return BOOL false whenever the sphere is definitely outside of the frustum,
 * true otherwise.
 */
BOOL ViewFrustumClipCheck(JDrama::TGraphics* gfx, Vec* position, f32 radius);

#endif
