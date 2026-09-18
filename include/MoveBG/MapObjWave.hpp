#ifndef MOVE_BG_MAP_OBJ_WAVE_HPP
#define MOVE_BG_MAP_OBJ_WAVE_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/ResTIMG.hpp>
#include <dolphin/gx/GXStruct.h>

class TMapObjWave;

extern TMapObjWave* gpMapObjWave;

/**
 * @brief The large wavy sea surface drawn around Mario.
 *
 * @details Not a map object at all despite the name: it is a plain
 * JDrama::TViewObj that emits a triangle-strip grid centred on Mario every
 * frame and displaces it with two sine waves. Amplitude and alpha fade out
 * over shallow water, so the mesh disappears near the shore.
 */
class TMapObjWave : public JDrama::TViewObj {
public:
	TMapObjWave(const char* name = "波の表現");

	virtual ~TMapObjWave() { }

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void movement();
	void updateTime();
	void updateHeightAndAlpha();
	void draw();
	int getAlpha(f32 x, f32 z) const;
	void noWave();
	f32 getHeight(f32 x, f32 y, f32 z) const;
	f32 getWaveHeight(f32 x, f32 z) const;
	f32 getStaticTexPos0(f32 v) const;
	f32 getStaticTexPos1(f32 v) const;
	f32 getMoveTexPos0(f32 v) const;
	f32 getMoveTexPos1(f32 v) const;
	void initDraw();

	const ResTIMG* getTexture() const { return mTexture; }

public:
	/// Side length of the drawn grid.
	/* 0x10 */ f32 mAreaSize;
	/// Half of TMapObjWave::mAreaSize; the loops run over +/- this.
	/* 0x14 */ f32 mHalfSize;
	/// 1 / TMapObjWave::mHalfSize, for the alpha falloff.
	/* 0x18 */ f32 mInvHalfSize;
	/// Distance between two grid lines.
	/* 0x1C */ f32 mGridSize;
	/// mAreaSize / mGridSize, the vertex count per strip.
	/* 0x20 */ int mDivideNum;
	/* 0x24 */ f32 mWaveFreqX;
	/* 0x28 */ f32 mWaveFreqZ;
	/* 0x2C */ f32 mWaveHeightMaxX;
	/* 0x30 */ f32 mWaveHeightMaxZ;
	/* 0x34 */ f32 mWaveHeightMinX;
	/* 0x38 */ f32 mWaveHeightMinZ;
	/* 0x3C */ f32 mWaveHeightX;
	/* 0x40 */ f32 mWaveHeightZ;
	/// Extra wave height requested by the stream cube Mario stands in.
	/* 0x44 */ f32 mCubeWaveHeight;
	/// Per-frame approach rate of TMapObjWave::mCubeWaveHeight.
	/* 0x48 */ f32 mCubeWaveHeightRate;
	/// Sea depth over which the wave height fades to its minimum.
	/* 0x4C */ f32 mHeightFadeDepth;
	/// Sea depth over which the alpha fades to its minimum.
	/* 0x50 */ f32 mAlphaFadeDepth;
	/* 0x54 */ f32 mAlpha;
	/* 0x58 */ f32 mAlphaMax;
	/* 0x5C */ f32 mAlphaMin;
	/* 0x60 */ f32 mTexScrollSpeed;
	/* 0x64 */ f32 mWavePhaseX;
	/* 0x68 */ f32 mWavePhaseZ;
	/* 0x6C */ f32 mTexPos0;
	/* 0x70 */ f32 mTexPos1;
	/* 0x74 */ f32 mTexScale0;
	/* 0x78 */ f32 mTexScale1;
	/* 0x7C */ GXColorS10 mTevColor0;
	/* 0x84 */ GXColorS10 mTevColor1;
	/* 0x8C */ GXColorS10 mTevColor2;
	/* 0x94 */ const ResTIMG* mTexture;
	/* 0x98 */ u16 unk98;
};

#endif
