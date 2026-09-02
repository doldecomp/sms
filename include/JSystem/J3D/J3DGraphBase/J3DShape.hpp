#ifndef J3D_SHAPE_HPP
#define J3D_SHAPE_HPP

#include <JSystem/J3D/J3DAssert.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DVertex.hpp>
#include <dolphin/mtx.h>
#include <dolphin/gx.h>

class J3DMaterial;

/**
 * @brief Strategy for loading matrices into GP matrix memory to be used for
 * rendering. Basic form loads a single matrix from GX_POS_MTX_ARRAY by index or
 * optionally uses the view matrix instead for position, normals or both.
 */
class J3DShapeMtx {
public:
	typedef void (J3DShapeMtx::*LoadPipeline)(int, u16) const;

	J3DShapeMtx(u16 useMtxIndex)
	    : mUseMtxIndex(useMtxIndex)
	{
	}

	virtual ~J3DShapeMtx() { }
	virtual int getType() const { return 'SMTX'; }
	virtual u32 getUseMtxNum() const { return 1; }
	virtual u16 getUseMtxIndex(u16) const { return mUseMtxIndex; }
	virtual void load() const;
	virtual void calcNBTScale(const Vec&, float (*)[3][3], float (*)[3][3]);

	/// No CPU-driven transforms
	void loadMtxIndx_PNGP(int, u16) const;
	/// Position transforms are CPU-driven
	void loadMtxIndx_PCPU(int, u16) const;
	/// Normal transforms are CPU-driven
	void loadMtxIndx_NCPU(int, u16) const;
	/// Both position and normal transforms are CPU-driven
	void loadMtxIndx_PNCPU(int, u16) const;

	static LoadPipeline mtxLoadPipeline[4];
	static u32 currentPipeline;

private:
	u16 mUseMtxIndex;
};

/**
 * @brief Pre-recorded matrix loading strategy: a display list is executed
 * instead of doing GX commands.
 */
class J3DShapeMtxDL : public J3DShapeMtx {
public:
	J3DShapeMtxDL(u16);

	virtual ~J3DShapeMtxDL() { }
	virtual void load() const;
	virtual void calcNBTScale(const Vec&, float (*)[3][3], float (*)[3][3]) { }

private:
	void* mDisplayList;
};

/**
 * @brief Loads up to 10 matrices into GP matrix memory from GX_POS_MTX_ARRAY by
 * indices, optionally using the view matrix for all position or all normal
 * matrices.
 */
class J3DShapeMtxMulti : public J3DShapeMtx {
public:
	J3DShapeMtxMulti(u16 useMtxIndex, u16 useMtxNum, u16* useMtxIndexTable)
	    : J3DShapeMtx(useMtxIndex)
	    , mUseMtxNum(useMtxNum)
	    , mUseMtxIndexTable(useMtxIndexTable)
	{
	}

	virtual ~J3DShapeMtxMulti() { }
	virtual int getType() const { return 'SMML'; }
	virtual u32 getUseMtxNum() const { return mUseMtxNum; }
	virtual u16 getUseMtxIndex(u16 i) const { return mUseMtxIndexTable[i]; }
	virtual void load() const;
	virtual void calcNBTScale(const Vec&, float (*)[3][3], float (*)[3][3]);

private:
	u16 mUseMtxNum;
	u16* mUseMtxIndexTable;
};

/**
 * @brief A single piece of geometry in a J3D shape encoded as a GX display
 * list to be drawn with a single set of matrices.
 */
class J3DShapeDraw {
public:
	J3DShapeDraw(const u8*, u32);

	virtual ~J3DShapeDraw() { }

	void draw() const;

	u8* getDisplayList() const { return (u8*)mDisplayList; }
	u32 getDisplayListSize() const { return mDisplayListSize; }

public:
	u32 mDisplayListSize;
	const u8* mDisplayList;
};

enum J3DShpFlag {
	/// The shape is drawn. Cleared to hide it.
	J3DShpFlag_Visible = 0x1,
	/// Positions are already transformed, so load the view matrix instead.
	J3DShpFlag_SkinPosCpu = 0x4,
	/// Normals are already transformed, so load the view matrix instead.
	J3DShpFlag_SkinNrmCpu = 0x8,
	/// The shape is deformed by a J3DCluster.
	J3DShpFlag_EnableLod = 0x10,
};

/**
 * @brief A single 3D mesh.
 * @details A shape in J3D is cut up into pieces such that each piece contains
 * vertices which only need 10 or less matrices to be drawn. This comes from a
 * limitation of the GX hardware.
 */
class J3DShape {
	friend class J3DShapeFactory;

public:
	J3DShape()
	{
		mCurrentTexMtx[0] = GX_IDENTITY;
		mCurrentTexMtx[1] = GX_IDENTITY;
		mCurrentTexMtx[2] = GX_IDENTITY;
		mCurrentTexMtx[3] = GX_IDENTITY;
		mCurrentTexMtx[4] = GX_IDENTITY;
		mCurrentTexMtx[5] = GX_IDENTITY;
		mCurrentTexMtx[6] = GX_IDENTITY;
		mCurrentTexMtx[7] = GX_IDENTITY;
		initialize();
	}
	~J3DShape();
	void initialize();
	void calcNBTScale(const Vec&, float (*)[3][3], float (*)[3][3]);
	int countBumpMtxNum() const;
	void makeVtxArrayCmd();
	void makeVcdVatCmd();
	void loadVtxArray() const;
	void draw() const;

	void setCurrentTexMtx(u8 t0, u8 t1, u8 t2, u8 t3, u8 t4, u8 t5, u8 t6,
	                      u8 t7)
	{
		mCurrentTexMtx[0] = t0;
		mCurrentTexMtx[1] = t1;
		mCurrentTexMtx[2] = t2;
		mCurrentTexMtx[3] = t3;
		mCurrentTexMtx[4] = t4;
		mCurrentTexMtx[5] = t5;
		mCurrentTexMtx[6] = t6;
		mCurrentTexMtx[7] = t7;
	}

	bool checkFlag(u32 flag) const { return (mFlags & flag) ? TRUE : FALSE; }
	void onFlag(u32 flag) { mFlags |= flag; }
	void offFlag(u32 flag) { mFlags &= ~flag; }

	u32 getIndex() const { return mIndex; }
	GXVtxDescList* getVtxDesc() const { return mVtxDesc; }
	u32 getMtxGroupNum() const { return mMtxGroupNum; }
	J3DShapeMtx* getShapeMtx(u16 idx) const { return mMatrices[idx]; }
	J3DShapeDraw* getShapeDraw(u16 idx) const { return mDraws[idx]; }
	u32 getBumpMtxOffset() const { return mBumpMtxOffset; }
	void setBumpMtxOffset(u32 offset) { mBumpMtxOffset = offset; }

	void setDrawMtx(Mtx** pDrawMtx) { mDrawMatrices = pDrawMtx; }
	void setNrmMtx(Mtx33** pNrmMtx) { mNormMatrices = pNrmMtx; }
	void setCurrentViewNoPtr(u32* pViewNoPtr)
	{
		J3D_ASSERT_NULLPTR(pViewNoPtr != nullptr);
		mCurrentViewNo = pViewNoPtr;
	}

	void setScaleFlagArray(u8* pScaleFlagArray)
	{
		J3D_ASSERT_NULLPTR(pScaleFlagArray != nullptr);
		mScaleFlagArray = pScaleFlagArray;
	}

	void setDrawMtxDataPointer(J3DDrawMtxData* pMtxData)
	{
		J3D_ASSERT_NULLPTR(pMtxData != nullptr);
		mDrawMtxData = pMtxData;
	}

	void setVertexDataPointer(J3DVertexData* pVtxData)
	{
		J3D_ASSERT_NULLPTR(pVtxData != nullptr);
		mVertexData = pVtxData;
	}

	void* getVcdVatCmd() const { return mVcdVatCmd; }
	void setVcdVatCmd(void* pVatCmd) { mVcdVatCmd = pVatCmd; }

	enum {
		kVcdVatDLSize = 0xC0,
	};

private:
	/* 0x0 */ J3DMaterial* mMaterial;
	/* 0x4 */ u16 mIndex;
	/* 0x6 */ u16 mMtxGroupNum;
	/* 0x8 */ u32 mFlags;
	/* 0xC */ f32 mRadius;
	/* 0x10 */ Vec mMin;
	/* 0x1C */ Vec mMax;
	/* 0x28 */ void* mVcdVatCmd;
	/* 0x2C */ GXVtxDescList* mVtxDesc;
	/* 0x30 */ bool mHasNBT;
	/* 0x34 */ J3DShapeMtx** mMatrices; // mMtxGroupNum entries
	/* 0x38 */ J3DShapeDraw** mDraws;   // mMtxGroupNum entries
	/* 0x3C */ u8 mCurrentTexMtx[8];
	/* 0x44 */ J3DVertexData* mVertexData;
	/* 0x48 */ J3DDrawMtxData* mDrawMtxData;
	/* 0x4C */ u8* mScaleFlagArray;
	/* 0x50 */ Mtx** mDrawMatrices;
	/* 0x54 */ Mtx33** mNormMatrices;
	/* 0x58 */ u32* mCurrentViewNo;
	/* 0x5C */ u32 mBumpMtxOffset;
};

#endif
