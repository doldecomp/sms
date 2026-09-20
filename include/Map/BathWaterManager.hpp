#ifndef MAP_BATH_WATER_MANAGER_HPP
#define MAP_BATH_WATER_MANAGER_HPP

#include <Strategic/HitActor.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JGeometry/JGPosition3.hpp>
#include <System/Params.hpp>
#include <System/ParamInst.hpp>

class TBathtub;
class JUTTexture;
class J3DModelData;
class J3DModel;

class TBathtubData {
public:
	// TODO: 82.8%. The instruction stream is right; what is left is the
	// callee-saved float allocation around the inlined
	// TQuat4::setRotate(from, to, amount) -- retail holds the cross
	// product's components and the two constant `up` components in
	// f25-f31 and re-reads only up.x, while we spill up.y/up.z back to the
	// stack across atan2f/sinf/cosf. JGQuat4.hpp's setRotate already carries
	// the same note (the order is decided inside cross()'s batched set()),
	// so no spelling here reaches it.
	JGeometry::TVec3<f32> getGravityDir(f32 amount) const
	{
		(void)0;
		if (!unk65) {
			JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
			JGeometry::TQuat4<f32> q;
			q.setRotate(unk0C, up, amount);
			q.rotate(up, up);
			return up;
		} else {
			return JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f);
		}
	}

	JGeometry::TVec3<f32> getPos(int i, int j, f32 h) const
	{
		// angle before amp: the ROM gives angle the higher float register, and
		// register order follows declaration order while the int-to-float
		// conversions follow assignment order.
		// TODO: the three conversion temporaries still land in a different
		// order (0x30/0x38/0x40 hold j, i, i in the ROM and i, j, i here);
		// naming j first, `t = j; t = i / t`, and commuting the product do
		// not move them.
		f32 t     = (f32)i / (f32)j;
		f32 angle = (f32)i * 0.31415927f;
		f32 amp   = t * (unk3C - h);

		JGeometry::TVec3<f32> result;
		result = mPos;

		// The rows of mMtx are the bathtub's axes: the sin term walks
		// 0x18/0x1c/0x20 (row 0, X), the cos term 0x30/0x34/0x38 (row 2, Z)
		// and the height term 0x24/0x28/0x2c (row 1, Y). Read through the
		// array and not at(): an inlined accessor's result always lands as
		// the *second* operand of the multiply, and the ROM has the matrix
		// element first in all nine fmadds.
		f32 s = amp * sinf(angle);
		result.x += unk18.mMtx[0][0] * s;
		result.y += unk18.mMtx[0][1] * s;
		result.z += unk18.mMtx[0][2] * s;

		f32 c = amp * cosf(angle);
		result.x += unk18.mMtx[2][0] * c;
		result.y += unk18.mMtx[2][1] * c;
		result.z += unk18.mMtx[2][2] * c;

		f32 yScale = (1.0f - t) * -(unk44 - h);
		result.x += unk18.mMtx[1][0] * yScale;
		result.y += unk18.mMtx[1][1] * yScale;
		result.z += unk18.mMtx[1][2] * yScale;

		return result;
	}

	// fabricated
	JGeometry::TVec3<f32> getThing() const
	{
		return JGeometry::TVec3<f32>(mPos.x, mPos.y - unk44, mPos.z);
	}

	// fabricated, but it has to be a function and it has to return by value:
	// TBathWaterManager::throwMario builds its local position through a
	// by-value temporary that it then copies word-wise into a named vector,
	// and only at this depth does the member template TVec3<f>::set<f> stay
	// the `bl` the ROM has (constructor at depth 2, set<f> at depth 3).
	// The projection is the transpose of TRotation3::mult33 because the
	// bathtub's axes are this matrix's rows.
	JGeometry::TVec3<f32> getLocalPos(const JGeometry::TVec3<f32>& pos) const
	{
		JGeometry::TVec3<f32> diff;
		diff.sub(pos, mPos);
		return JGeometry::TVec3<f32>(
		    unk18.mMtx[0][0] * diff.x + unk18.mMtx[0][1] * diff.y
		        + unk18.mMtx[0][2] * diff.z,
		    unk18.mMtx[1][0] * diff.x + unk18.mMtx[1][1] * diff.y
		        + unk18.mMtx[1][2] * diff.z,
		    unk18.mMtx[2][0] * diff.x + unk18.mMtx[2][1] * diff.y
		        + unk18.mMtx[2][2] * diff.z);
	}

public:
	/* 0x00 */ JGeometry::TVec3<f32> mPos;
	/* 0x0C */ JGeometry::TVec3<f32> unk0C;
	// TPosition3, not TRotation3: the extra (empty) inheritance level is what
	// puts the 4-byte SMatrix33R<f32> constructor out of line, as the map's
	// weak __ct__Q29JGeometry13SMatrix33R<f>Fv in MapObjCorona.cpp shows.
	/* 0x18 */ JGeometry::TPosition3<JGeometry::TMatrix33<
	    JGeometry::SMatrix33R<f32> > > unk18;
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
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

public:
	/* 0x10 */ TBathWaterManager* unk10;
};

class TBathWaterManager : public JDrama::TViewObj {
public:
	TBathWaterManager();
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

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
