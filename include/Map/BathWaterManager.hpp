#ifndef MAP_BATH_WATER_MANAGER_HPP
#define MAP_BATH_WATER_MANAGER_HPP

#include <Strategic/HitActor.hpp>
#include <JSystem/JMath.hpp>
#include <System/Params.hpp>
#include <System/ParamInst.hpp>

class TBathtub;
class JUTTexture;
class J3DModelData;
class J3DModel;

class TBathtubData {
public:
	JGeometry::TVec3<f32> getGravityDir(f32 amount) const
	{
		(void)0;
		if (!unk65) {
			JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
			(void)&up;
			JGeometry::TQuat4<f32> q;
			q.setRotate(unk0C, up, amount);
			JGeometry::TVec3<f32> result;
			q.rotate(up, result);
			return result;
		} else {
			return JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f);
		}
	}

	JGeometry::TVec3<f32> getPos(int i, int j, f32 h) const
	{
		f32 t     = (f32)i / (f32)j;
		f32 amp   = t * (unk3C - h);
		f32 angle = (f32)i * 0.31415927f;

		JGeometry::TVec3<f32> result;
		result = mPos;

		f32 s = amp * sinf(angle);
		result.x += unk18.at(0, 0) * s;
		result.y += unk18.at(0, 1) * s;
		result.z += unk18.at(0, 2) * s;

		f32 c = amp * cosf(angle);
		result.x += unk18.at(2, 0) * c;
		result.y += unk18.at(2, 1) * c;
		result.z += unk18.at(2, 2) * c;

		f32 yScale = (1.0f - t) * -(unk44 - h);
		result.x += unk18.at(1, 0) * yScale;
		result.y += unk18.at(1, 1) * yScale;
		result.z += unk18.at(1, 2) * yScale;

		return result;
	}

	// fabricated
	JGeometry::TVec3<f32> getThing() const
	{
		return JGeometry::TVec3<f32>(mPos.x, mPos.y - unk44, mPos.z);
	}

public:
	/* 0x00 */ JGeometry::TVec3<f32> mPos;
	/* 0x0C */ JGeometry::TVec3<f32> unk0C;
	/* 0x18 */ JGeometry::TRotation3<TMtx33f> unk18;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ f32 unk48;
	/* 0x4C */ f32 unk4C;
	/* 0x50 */ f32 unk50;
	/* 0x54 */ f32 unk54;
	/* 0x58 */ JGeometry::TVec3<f32> unk58;
	/* 0x64 */ u8 unk64;
	/* 0x65 */ u8 unk65;
};

class TBathWaterParams : public TParams {
public:
	TBathWaterParams(const char*);

	/* 0x08 */ TParamRT<u8> suppliesDrops;
	/* 0x1C */ TParamRT<u8> bathtubGravity;
	/* 0x30 */ TParamRT<u8> intersects;
	/* 0x44 */ TParamRT<u8> isVisible;
	/* 0x58 */ TParamRT<u8> checksMario;
	/* 0x6C */ TParamRT<s32> numDrops;
	/* 0x80 */ TParamRT<f32> dropRadius;
	/* 0x94 */ TParamRT<f32> texScale;
	/* 0xA8 */ TParamRT<f32> hitScale;
	/* 0xBC */ TParamRT<f32> modelScale;
	/* 0xD0 */ TParamRT<f32> modelScale2;
	/* 0xE4 */ TParamRT<f32> modelScaleY;
	/* 0xF8 */ TParamRT<f32> gravity;
	/* 0x10C */ TParamRT<f32> bounceY;
	/* 0x120 */ TParamRT<f32> bounceXZ;
	/* 0x134 */ TParamRT<f32> damp;
	/* 0x148 */ TParamRT<f32> jump;
	/* 0x15C */ TParamRT<f32> overGravity;
	/* 0x170 */ TParamRT<f32> emitVel;
	/* 0x184 */ TParamRT<s32> lifeTime;
};

class TBathWaterGlobalParams : public TParams {
public:
	TBathWaterGlobalParams();

	/* 0x08 */ TParamRT<u8> regR;
	/* 0x1C */ TParamRT<u8> regG;
	/* 0x30 */ TParamRT<u8> regB;
	/* 0x44 */ TParamRT<u8> regA;
	/* 0x58 */ TParamRT<u8> kRegR;
	/* 0x6C */ TParamRT<u8> kRegG;
	/* 0x80 */ TParamRT<u8> kRegB;
	/* 0x94 */ TParamRT<u8> kRegA;
	/* 0xA8 */ TParamRT<u8> polygonR;
	/* 0xBC */ TParamRT<u8> polygonG;
	/* 0xD0 */ TParamRT<u8> polygonB;
	/* 0xE4 */ TParamRT<f32> indTexScale;
	/* 0xF8 */ TParamRT<u8> showsCap;
	/* 0x10C */ TParamRT<u8> bendsNormal;
	/* 0x120 */ TParamRT<u8> showsMist;
	/* 0x134 */ TParamRT<u8> clearsAlpha;
	/* 0x148 */ TParamRT<u8> alpha;
	/* 0x15C */ TParamRT<u8> scrolls;
	/* 0x170 */ TParamRT<u8> displaysMesh;
	/* 0x184 */ TParamRT<u8> mode;
	/* 0x198 */ TParamRT<u8> mask;
	/* 0x1AC */ TParamRT<s32> indirectScale;
	/* 0x1C0 */ TParamRT<s32> scrollSpan;
	/* 0x1D4 */ TParamRT<s32> meshTexWidth;
	/* 0x1E8 */ TParamRT<f32> envMapScale;
	/* 0x1FC */ TParamRT<f32> capHeight;
	/* 0x210 */ TParamRT<f32> meshWidth;
};

class TBathWater;
class TBathWaterRenderer;
class TBathWaterManager;

class TBathWaterPreprocessor : public JDrama::TViewObj {
public:
	TBathWaterPreprocessor(TBathWaterManager*);
	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ TBathWaterManager* unk10;
};

class TBathWaterManager : public JDrama::TViewObj {
public:
	TBathWaterManager();
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);

	void wave(JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>&, f32, f32) const;
	void initializeIfYet_();
	void preprocess(JDrama::TGraphics*);
	f32 getWaterHeight(f32, f32) const;
	void throwMario(f32);

	// fabricated
	TBathWaterPreprocessor* getPreprocessor() { return &unk34; }

	static const char* fileNames[];

public:
	/* 0x10 */ JMath::TRandomFast unk10;
	/* 0x14 */ TBathWaterParams** unk14;
	/* 0x18 */ TBathWaterGlobalParams* unk18;
	/* 0x1C */ u8 unk1C;
	/* 0x20 */ TBathWater** unk20;
	/* 0x24 */ TBathtub* unk24;
	/* 0x28 */ TBathWaterRenderer* unk28[2];
	/* 0x30 */ TBathWaterRenderer* unk30;
	/* 0x34 */ TBathWaterPreprocessor unk34;
};

#endif
