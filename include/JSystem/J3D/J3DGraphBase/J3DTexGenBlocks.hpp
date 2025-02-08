#ifndef J3D_TEX_GEN_BLOCKS_HPP
#define J3D_TEX_GEN_BLOCKS_HPP

#include <JSystem/J3D/J3DGraphBase/J3DComponents.hpp>

class J3DTexGenBlock {
public:
	virtual void reset(J3DTexGenBlock*) { }
	virtual void calc(MtxPtr) = 0;
	virtual s32 countDLSize() = 0;
	virtual u32 getType()     = 0;

	virtual void setTexGenNum(const u32*) = 0;
	virtual void setTexGenNum(u32)        = 0;
	virtual u32 getTexGenNum() const      = 0;

	virtual void setTexCoord(u32, const J3DTexCoord*) = 0;
	virtual J3DTexCoord* getTexCoord(u32)             = 0;

	virtual void setTexMtx(u32, J3DTexMtx*)           = 0;
	virtual J3DTexMtx* replaceTexMtx(u32, J3DTexMtx*) = 0;
	virtual J3DTexMtx* getTexMtx(u32)                 = 0;

	virtual void setNBTScale(const J3DNBTScale*) = 0;
	virtual void setNBTScale(J3DNBTScale)        = 0;
	virtual J3DNBTScale* getNBTScale()           = 0;

	virtual ~J3DTexGenBlock() { }
	virtual void load()  = 0;
	virtual void patch() = 0;
};

class J3DTexGenBlockBasic : public J3DTexGenBlock {
public:
	J3DTexGenBlockBasic()
	    : mNBTScale(j3dDefaultNBTScaleInfo)
	{
		initialize();
	}

	void initialize();

	virtual void reset(J3DTexGenBlock*);
	virtual void calc(MtxPtr);
	virtual s32 countDLSize();
	virtual u32 getType() { return 'TGBC'; }

	virtual void setTexGenNum(const u32* num) { mTexGenNum = *num; }
	virtual void setTexGenNum(u32 num) { mTexGenNum = num; }
	virtual u32 getTexGenNum() const { return mTexGenNum; }

	virtual void setTexCoord(u32 i, const J3DTexCoord* coord)
	{
		mTexCoord[i] = *coord;
	}
	virtual J3DTexCoord* getTexCoord(u32 i) { return &mTexCoord[i]; }

	virtual void setTexMtx(u32 i, J3DTexMtx* mtx) { mTexMtx[i] = mtx; }
	virtual J3DTexMtx* replaceTexMtx(u32 i, J3DTexMtx* mtx)
	{
		J3DTexMtx* ret = mTexMtx[i];
		mTexMtx[i]     = mtx;
		return ret;
	}
	virtual J3DTexMtx* getTexMtx(u32 i) { return mTexMtx[i]; }

	virtual void setNBTScale(const J3DNBTScale* scale) { mNBTScale = *scale; }
	virtual void setNBTScale(J3DNBTScale scale) { mNBTScale = scale; }
	virtual J3DNBTScale* getNBTScale() { return &mNBTScale; }

	virtual ~J3DTexGenBlockBasic() { }
	virtual void load();
	virtual void patch();

public:
	/* 0x4 */ u32 mTexGenNum;
	/* 0x8 */ J3DTexCoord mTexCoord[8];
	/* 0x28 */ J3DTexMtx* mTexMtx[8];
	/* 0x48 */ J3DNBTScale mNBTScale;

	/* 0x58 */ u32 mTexMtxOffset;
};

#endif
