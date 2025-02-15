#ifndef J3D_CLUSTER_HPP
#define J3D_CLUSTER_HPP

#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <dolphin/types.h>
#include <dolphin/mtx.h>

class J3DModelData;
class J3DModel;
class J3DAnmCluster;
class JUTNameTab;

struct J3DClusterKey {
	void operator=(const J3DClusterKey& other)
	{
		mPosNum  = other.mPosNum;
		mNrmNum  = other.mNrmNum;
		mPosFlag = other.mPosFlag;
		mNrmFlag = other.mNrmFlag;
	}

	/* 0x00 */ u16 mPosNum;
	/* 0x02 */ u16 mNrmNum;
	/* 0x04 */ u16* mPosFlag;
	/* 0x08 */ u16* mNrmFlag;
}; // Size: 0x0C

struct J3DClusterVertex {
	void operator=(const J3DClusterVertex& other)
	{
		mNum    = other.mNum;
		mSrcIdx = other.mSrcIdx;
		mDstIdx = other.mDstIdx;
	}

	/* 0x00 */ u16 mNum;
	/* 0x04 */ u16* mSrcIdx;
	/* 0x08 */ u16* mDstIdx;
}; // Size: 0x0C

class J3DDeformData;

class J3DDeformer {
public:
	void clear();
	void deform(J3DModel*, u16);
	void deform(J3DModel*, u16, float*);
	void base(J3DModel*);
	void normalize(float*);
	void normalizeWeight(int, float*);

public:
	/* 0x00 */ J3DDeformData* mDeformData;
	/* 0x04 */ J3DAnmCluster* mAnmCluster;
	/* 0x08 */ f32* mWeightList;
	/* 0x0C */ f32* field_0x0c;
	/* 0x10 */ u32 mFlags;
};

class J3DCluster {
public:
	J3DDeformer* getDeformer() { return mDeformer; }

public:
	/* 0x00 */ f32 mMaxAngle;
	/* 0x04 */ f32 mMinAngle;
	/* 0x08 */ J3DClusterKey* mClusterKey;
	/* 0x0C */ u8 mFlags;
	/* 0x0D */ u8 field_0xd[0x10 - 0x0D];
	/* 0x10 */ u16 mKeyNum;
	/* 0x12 */ u16 mPosNum;
	/* 0x14 */ u16 mNrmNum;
	/* 0x16 */ u16 mClusterVertexNum;
	/* 0x18 */ u16* mPosDstIdx;
	/* 0x1C */ J3DClusterVertex* mClusterVertex;
	/* 0x20 */ J3DDeformer* mDeformer;
};

class J3DDeformData {
public:
	J3DDeformData();

	void clear();
	void onAllFlag(u32);
	void offAllFlag(u32);
	void base(J3DModel*);

	void deform(J3DModel*);
	void setAnm(J3DAnmCluster*);

	J3DCluster* getClusterPointer(u16 i) { return &mClusters[i]; }
	J3DClusterKey* getClusterKeyPointer(u16 i) { return &mClusterKeys[i]; }

	f32* getVtxPos() { return mVtxPos; }
	f32* getVtxNrm() { return mVtxNrm; }

public:
	/* 0x00 */ u16 mClusterNum;
	/* 0x02 */ u16 mClusterKeyNum;
	/* 0x04 */ u16 mClusterVertexNum;
	/* 0x08 */ J3DCluster* mClusters;
	/* 0x0C */ J3DClusterKey* mClusterKeys;
	/* 0x10 */ J3DClusterVertex* mClusterVertices;
	/* 0x14 */ u16 mVtxPosNum;
	/* 0x16 */ u16 mVtxNrmNum;
	/* 0x18 */ f32* mVtxPos;
	/* 0x1C */ f32* mVtxNrm;
	/* 0x20 */ JUTNameTab* mClusterName;
	/* 0x24 */ JUTNameTab* mClusterKeyName;
};

class J3DSkinDeform {
public:
	J3DSkinDeform();

	void initMtxIndexArray(J3DModelData*);
	void calcNrmMtx(J3DModel*);

	virtual void deform(J3DModel*);
	virtual ~J3DSkinDeform() { }

	void setNrmMtx(int i, MtxPtr mtx) { J3DPSMtx33CopyFrom34(mtx, mNrmMtx[i]); }
	ROMtxPtr getNrmMtx(int i) { return mNrmMtx[i]; }

private:
	/* 0x04 */ u16* mPosUseMtx;
	/* 0x08 */ u16* mNrmUseMtx;
	/* 0x0C */ Mtx33* mNrmMtx;
	/* 0x10 */ u32 mFlags;
};

#endif
