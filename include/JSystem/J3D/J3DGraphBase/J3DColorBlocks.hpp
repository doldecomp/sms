#ifndef J3D_COLOR_BLOCKS_HPP
#define J3D_COLOR_BLOCKS_HPP

#include <types.h>
#include <JSystem/J3D/J3DGraphBase/J3DComponents.hpp>

class J3DColorBlock {
public:
	virtual void reset(J3DColorBlock*) { }
	virtual s32 countDLSize() = 0;
	virtual u32 getType()     = 0;

	virtual void setMatColor(u32, const J3DGXColor*) = 0;
	virtual void setMatColor(u32, J3DGXColor)        = 0;
	virtual J3DGXColor* getMatColor(u32)             = 0;

	virtual void setAmbColor(u32, const J3DGXColor*) = 0;
	virtual void setAmbColor(u32, J3DGXColor)        = 0;
	virtual J3DGXColor* getAmbColor(u32)             = 0;

	virtual void setColorChanNum(u8)        = 0;
	virtual void setColorChanNum(const u8*) = 0;
	virtual u8 getColorChanNum() const      = 0;

	virtual void setColorChan(u32, const J3DColorChan&) = 0;
	virtual void setColorChan(u32, const J3DColorChan*) = 0;
	virtual J3DColorChan* getColorChan(u32)             = 0;

	virtual void setLight(u32, J3DLightObj*)             = 0;
	virtual J3DLightObj* replaceLight(u32, J3DLightObj*) = 0;
	virtual J3DLightObj* getLight(u32)                   = 0;

	virtual void setCullMode(const u8*) = 0;
	virtual void setCullMode(u8)        = 0;
	virtual u8 getCullMode() const      = 0;

	virtual ~J3DColorBlock() { }
	virtual void load() = 0;
};

class J3DColorBlockLightOff : public J3DColorBlock {
public:
	J3DColorBlockLightOff() { initialize(); }

	void initialize();

	virtual void reset(J3DColorBlock*);
	virtual s32 countDLSize();
	virtual u32 getType() { return 'CLOF'; }

	virtual void setMatColor(u32 i, const J3DGXColor* color)
	{
		mMatColor[i] = *color;
	}
	virtual void setMatColor(u32 i, J3DGXColor color) { mMatColor[i] = color; }
	virtual J3DGXColor* getMatColor(u32 i) { return &mMatColor[i]; }

	virtual void setAmbColor(u32, const J3DGXColor*) { }
	virtual void setAmbColor(u32, J3DGXColor) { }
	virtual J3DGXColor* getAmbColor(u32) { return nullptr; }

	virtual void setColorChanNum(u8 num) { mColorChanNum = num; }
	virtual void setColorChanNum(const u8* num) { mColorChanNum = *num; }
	virtual u8 getColorChanNum() const { return mColorChanNum; }

	virtual void setColorChan(u32 i, const J3DColorChan& chan)
	{
		mColorChan[i] = chan;
	}
	virtual void setColorChan(u32 i, const J3DColorChan* chan)
	{
		mColorChan[i] = *chan;
	}
	virtual J3DColorChan* getColorChan(u32 i) { return &mColorChan[i]; }

	virtual void setLight(u32, J3DLightObj*) { }
	virtual J3DLightObj* replaceLight(u32, J3DLightObj*) { return nullptr; }
	virtual J3DLightObj* getLight(u32) { return nullptr; }

	virtual void setCullMode(const u8* mode) { mCullMode = *mode; }
	virtual void setCullMode(u8 mode) { mCullMode = mode; }
	virtual u8 getCullMode() const { return mCullMode; }

	virtual ~J3DColorBlockLightOff() { }
	virtual void load();

private:
	/* 0x04 */ J3DGXColor mMatColor[2];
	/* 0x0C */ u8 mColorChanNum;
	/* 0x0E */ J3DColorChan mColorChan[4];
	/* 0x16 */ u8 mCullMode;
};

class J3DColorBlockLightOn : public J3DColorBlock {
public:
	J3DColorBlockLightOn() { initialize(); }

	void initialize();

	virtual void reset(J3DColorBlock*);
	virtual s32 countDLSize();
	virtual u32 getType() { return 'CLON'; }

	virtual void setMatColor(u32 i, const J3DGXColor* color)
	{
		mMatColor[i] = *color;
	}
	virtual void setMatColor(u32 i, J3DGXColor color) { mMatColor[i] = color; }
	virtual J3DGXColor* getMatColor(u32 i) { return &mMatColor[i]; }

	virtual void setAmbColor(u32 i, const J3DGXColor* color)
	{
		mAmbColor[i] = *color;
	}
	virtual void setAmbColor(u32 i, J3DGXColor color) { mAmbColor[i] = color; }
	virtual J3DGXColor* getAmbColor(u32 i) { return &mAmbColor[i]; }

	virtual void setColorChanNum(u8 num) { mColorChanNum = num; }
	virtual void setColorChanNum(const u8* num) { mColorChanNum = *num; }
	virtual u8 getColorChanNum() const { return mColorChanNum; }

	virtual void setColorChan(u32 i, const J3DColorChan& chan)
	{
		mColorChan[i] = chan;
	}
	virtual void setColorChan(u32 i, const J3DColorChan* chan)
	{
		mColorChan[i] = *chan;
	}
	virtual J3DColorChan* getColorChan(u32 i) { return &mColorChan[i]; }

	virtual void setLight(u32 i, J3DLightObj* light) { mLight[i] = light; }
	virtual J3DLightObj* replaceLight(u32 i, J3DLightObj* light)
	{
		J3DLightObj* ret = mLight[i];
		mLight[i]        = light;
		return ret;
	}
	virtual J3DLightObj* getLight(u32 i) { return mLight[i]; }

	virtual void setCullMode(const u8* mode) { mCullMode = *mode; }
	virtual void setCullMode(u8 mode) { mCullMode = mode; }
	virtual u8 getCullMode() const { return mCullMode; }

	virtual ~J3DColorBlockLightOn() { }
	virtual void load();

public:
	/* 0x04 */ J3DGXColor mMatColor[2];
	/* 0x0C */ J3DGXColor mAmbColor[2];
	/* 0x14 */ u8 mColorChanNum;
	/* 0x16 */ J3DColorChan mColorChan[4];
	/* 0x20 */ J3DLightObj* mLight[8];
	/* 0x40 */ u8 mCullMode;
};

#endif
