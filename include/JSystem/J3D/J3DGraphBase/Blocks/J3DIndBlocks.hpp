#ifndef J3D_IND_BLOCKS_HPP
#define J3D_IND_BLOCKS_HPP

#include <types.h>
#include <JSystem/J3D/J3DGraphBase/Components/J3DIndTexOrder.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DIndTexMtx.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DIndTexCoordScale.hpp>

class J3DTevBlock;

class J3DIndBlock {
public:
	virtual void reset(J3DIndBlock*) { }
	virtual s32 countDLSize() = 0;
	virtual u32 getType()     = 0;

	virtual void setIndTexStageNum(u8)   = 0;
	virtual u8 getIndTexStageNum() const = 0;

	virtual void setIndTexOrder(u32, J3DIndTexOrder)        = 0;
	virtual void setIndTexOrder(u32, const J3DIndTexOrder*) = 0;
	virtual J3DIndTexOrder* getIndTexOrder(u32)             = 0;

	virtual void setIndTexMtx(u32, const J3DIndTexMtx*) = 0;
	virtual void setIndTexMtx(u32, J3DIndTexMtx)        = 0;
	virtual J3DIndTexMtx* getIndTexMtx(u32)             = 0;

	virtual void setIndTexCoordScale(u32, const J3DIndTexCoordScale*) = 0;
	virtual void setIndTexCoordScale(u32, J3DIndTexCoordScale)        = 0;
	virtual J3DIndTexCoordScale* getIndTexCoordScale(u32)             = 0;

	virtual ~J3DIndBlock() { }
	virtual void load(J3DTevBlock*) = 0;
};

class J3DIndBlockFull : public J3DIndBlock {
public:
	J3DIndBlockFull() { mIndTexStageNum = 0; }

	virtual void reset(J3DIndBlock*);
	virtual s32 countDLSize();
	virtual u32 getType() { return 'IBLF'; }

	virtual void setIndTexStageNum(u8 num) { mIndTexStageNum = num; }
	virtual u8 getIndTexStageNum() const { return mIndTexStageNum; }

	virtual void setIndTexOrder(u32 i, J3DIndTexOrder order)
	{
		mIndTexOrder[i] = order;
	}
	virtual void setIndTexOrder(u32 i, const J3DIndTexOrder* order)
	{
		mIndTexOrder[i] = *order;
	}
	virtual J3DIndTexOrder* getIndTexOrder(u32 i) { return &mIndTexOrder[i]; }

	virtual void setIndTexMtx(u32 i, const J3DIndTexMtx* mtx)
	{
		mIndTexMtx[i] = *mtx;
	}
	virtual void setIndTexMtx(u32 i, J3DIndTexMtx mtx) { mIndTexMtx[i] = mtx; }
	virtual J3DIndTexMtx* getIndTexMtx(u32 i) { return &mIndTexMtx[i]; }

	virtual void setIndTexCoordScale(u32 i, const J3DIndTexCoordScale* scale)
	{
		mIndTexCoordScale[i] = *scale;
	}
	virtual void setIndTexCoordScale(u32 i, J3DIndTexCoordScale scale)
	{
		mIndTexCoordScale[i] = scale;
	}
	virtual J3DIndTexCoordScale* getIndTexCoordScale(u32 i)
	{
		return &mIndTexCoordScale[i];
	}

	virtual ~J3DIndBlockFull() { }
	virtual void load(J3DTevBlock*);

public:
	/* 0x04 */ u8 mIndTexStageNum;
	/* 0x05 */ J3DIndTexOrder mIndTexOrder[4];
	/* 0x18 */ J3DIndTexMtx mIndTexMtx[3];
	/* 0x6C */ J3DIndTexCoordScale mIndTexCoordScale[4];
};

class J3DIndBlockNull : public J3DIndBlock {
public:
	virtual void reset(J3DIndBlock*) { }
	virtual s32 countDLSize() { return 0; }
	virtual u32 getType() { return 'IBLN'; }

	virtual void setIndTexStageNum(u8) { }
	virtual u8 getIndTexStageNum() const { return 0; }

	virtual void setIndTexOrder(u32, J3DIndTexOrder) { }
	virtual void setIndTexOrder(u32, const J3DIndTexOrder*) { }
	virtual J3DIndTexOrder* getIndTexOrder(u32) { return nullptr; }

	virtual void setIndTexMtx(u32, const J3DIndTexMtx*) { }
	virtual void setIndTexMtx(u32, J3DIndTexMtx) { }
	virtual J3DIndTexMtx* getIndTexMtx(u32) { return nullptr; }

	virtual void setIndTexCoordScale(u32, const J3DIndTexCoordScale*) { }
	virtual void setIndTexCoordScale(u32, J3DIndTexCoordScale) { }
	virtual J3DIndTexCoordScale* getIndTexCoordScale(u32) { return nullptr; }

	virtual ~J3DIndBlockNull() { }
	virtual void load(J3DTevBlock*) { }
};

#endif
