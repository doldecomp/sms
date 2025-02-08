#ifndef J3D_PE_BLOCKS_HPP
#define J3D_PE_BLOCKS_HPP

#include <types.h>
#include <JSystem/J3D/J3DGraphBase/J3DComponents.hpp>

class J3DPEBlock {
public:
	virtual void reset(J3DPEBlock*) { }
	virtual s32 countDLSize() = 0;
	virtual u32 getType()     = 0;

	virtual void setFog(J3DFog*) { }
	virtual J3DFog* replaceFog(J3DFog*) { return nullptr; }
	virtual J3DFog* getFog() { return nullptr; }

	virtual void setAlphaComp(const J3DAlphaComp*) { }
	virtual void setAlphaComp(J3DAlphaComp) { }
	virtual J3DAlphaComp* getAlphaComp() { return nullptr; }

	virtual void setBlend(const J3DBlend*) { }
	virtual void setBlend(J3DBlend) { }
	virtual J3DBlend* getBlend() { return nullptr; }

	virtual void setZMode(const J3DZMode*) { }
	virtual void setZMode(J3DZMode) { }
	virtual J3DZMode* getZMode() { return nullptr; }

	virtual void setZCompLoc(const u8*) { }
	virtual void setZCompLoc(u8) { }
	virtual u8 getZCompLoc() const { return 0; }

	virtual void setDither(const u8*) { }
	virtual void setDither(u8) { }
	virtual u8 getDither() const { return 0; }

	virtual ~J3DPEBlock() { }
	virtual void load() = 0;
};

class J3DPEBlockOpa : public J3DPEBlock {
	virtual s32 countDLSize();
	virtual u32 getType() { return 'PEOP'; }

	virtual ~J3DPEBlockOpa() { }
	virtual void load();
};

class J3DPEBlockTexEdge : public J3DPEBlock {
	virtual s32 countDLSize();
	virtual u32 getType() { return 'PEED'; }

	virtual ~J3DPEBlockTexEdge() { }
	virtual void load();
};

class J3DPEBlockXlu : public J3DPEBlock {
	virtual s32 countDLSize();
	virtual u32 getType() { return 'PEXL'; }

	virtual ~J3DPEBlockXlu() { }
	virtual void load();
};

class J3DPEBlockFull : public J3DPEBlock {
public:
	J3DPEBlockFull() { initialize(); }

	void initialize();

	virtual void reset(J3DPEBlock*);
	virtual s32 countDLSize();
	virtual u32 getType() { return 'PEFL'; }

	virtual void setFog(J3DFog* fog) { mFog = fog; }
	virtual J3DFog* replaceFog(J3DFog* fog)
	{
		J3DFog* res = mFog;
		mFog        = fog;
		return res;
	}
	virtual J3DFog* getFog() { return mFog; }

	virtual void setAlphaComp(const J3DAlphaComp* alpha_comp)
	{
		mAlphaComp = *alpha_comp;
	}
	virtual void setAlphaComp(J3DAlphaComp alpha_comp)
	{
		mAlphaComp = alpha_comp;
	}
	virtual J3DAlphaComp* getAlphaComp() { return &mAlphaComp; }

	virtual void setBlend(const J3DBlend* blend) { mBlend = *blend; }
	virtual void setBlend(J3DBlend blend) { mBlend = blend; }
	virtual J3DBlend* getBlend() { return &mBlend; }

	virtual void setZMode(const J3DZMode* zmode) { mZMode = *zmode; }
	virtual void setZMode(J3DZMode zmode) { mZMode = zmode; }
	virtual J3DZMode* getZMode() { return &mZMode; }

	virtual void setZCompLoc(const u8* z_comp_loc) { mZCompLoc = *z_comp_loc; }
	virtual void setZCompLoc(u8 z_comp_loc) { mZCompLoc = z_comp_loc; }
	virtual u8 getZCompLoc() const { return mZCompLoc; }

	virtual void setDither(const u8* dither) { mDither = *dither; }
	virtual void setDither(u8 dither) { mDither = dither; }
	virtual u8 getDither() const { return mDither; }

	virtual ~J3DPEBlockFull() { }
	virtual void load();

private:
	/* 0x04 */ J3DFog* mFog;
	/* 0x08 */ J3DAlphaComp mAlphaComp;
	/* 0x0C */ J3DBlend mBlend;
	/* 0x10 */ J3DZMode mZMode;
	/* 0x12 */ u8 mZCompLoc;
	/* 0x13 */ u8 mDither;
};

#endif
