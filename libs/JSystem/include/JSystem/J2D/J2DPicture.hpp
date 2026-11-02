#ifndef J2D_PICTURE_HPP
#define J2D_PICTURE_HPP

#include <JSystem/J2D/J2DPane.hpp>
#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>

class J2DMaterial;
class JUTPalette;
class JUTTexture;
struct ResTIMG;
struct ResTLUT;

enum J2DMirror {
	/* 0 */ J2DMirror_None = 0,
	/* 1 */ J2DMirror_Y    = 1 << 0,
	/* 2 */ J2DMirror_X    = 1 << 1,
	/* 3 */ J2DMirror_XY   = J2DMirror_X | J2DMirror_Y,
};

enum J2DBinding {
	/* 0 */ J2DBind_None   = 0,
	/* 1 */ J2DBind_Bottom = 1 << 0,
	/* 2 */ J2DBind_Top    = 1 << 1,
	/* 4 */ J2DBind_Right  = 1 << 2,
	/* 8 */ J2DBind_Left   = 1 << 3,

	/* 0xF */ J2DBind_All = 0xF,
};

enum J2DWrapmode {
	/* 0 */ J2DWrapmode_NONE   = 0,
	/* 1 */ J2DWrapmode_REPEAT = 1,
};

class J2DPicture : public J2DPane {
public:
	struct TCornerColor {
		u32 mColor[4];
	};

	virtual ~J2DPicture();
	virtual void drawSelf(int, int);
	virtual void drawSelf(int, int, Mtx*);

	J2DPicture();
	J2DPicture(J2DPane*, JSURandomInputStream*, bool);
	J2DPicture(const ResTIMG*);
	J2DPicture(const char*);
	J2DPicture(JUTTexture*);
	J2DPicture(u32, const JUTRect&, const ResTIMG*, const ResTLUT*);
	J2DPicture(u32, const JUTRect&, const char*, const ResTLUT*);

	void initiate(const ResTIMG*, const ResTLUT*);
	void private_initiate(const ResTIMG*, const ResTLUT*);
	void initinfo();
	bool insert(const ResTIMG*, u8, float);
	bool insert(const char*, u8, float);
	bool insert(JUTTexture*, u8, float);
	bool remove(u8);
	bool remove(JUTTexture*);
	const ResTIMG* changeTexture(const ResTIMG*, u8);
	const ResTIMG* changeTexture(const char*, u8);
	void drawFullSet(int, int, int, int, J2DBinding, J2DMirror, bool,
	                 J2DWrapmode, J2DWrapmode, Mtx*);
	void draw(int, int, int, int, bool, bool, bool);
	void drawOut(const JUTRect&, const JUTRect&);
	void drawTexCoord(int, int, int, int, float, float, float, float, float,
	                  float, float, float, Mtx*);
	void setTevMode();
	void setBlendKonstColor();
	void setBlendKonstAlpha();

	void swap(float&, float&);

	JUTTexture* getTexture(u8 idx) const
	{
		return idx < mTextureNum ? mTextures[idx] : nullptr;
	}

	void draw(int x, int y, bool mirrorX, bool mirrorY, bool tumble)
	{
		draw(x, y, mTextures[0]->getWidth(), mTextures[0]->getHeight(), mirrorX,
		     mirrorY, tumble);
	}

	void load(GXTexMapID id, u8 idx)
	{
		if (idx < mTextureNum)
			mTextures[idx]->load(id);
	}

	void getCornerColor(TCornerColor& color) const
	{
		color.mColor[0] = mCornerColor[0];
		color.mColor[1] = mCornerColor[1];
		color.mColor[2] = mCornerColor[2];
		color.mColor[3] = mCornerColor[3];
	}

	void setCornerColor(JUtility::TColor c0, JUtility::TColor c1,
	                    JUtility::TColor c2, JUtility::TColor c3)
	{
		mCornerColor[0] = c0;
		mCornerColor[1] = c1;
		mCornerColor[2] = c2;
		mCornerColor[3] = c3;
	}

	void setCornerColor(JUtility::TColor c0) { setCornerColor(c0, c0, c0, c0); }

	void setCornerColor(J2DPicture::TCornerColor color)
	{
		mCornerColor[0] = color.mColor[0];
		mCornerColor[1] = color.mColor[1];
		mCornerColor[2] = color.mColor[2];
		mCornerColor[3] = color.mColor[3];
	}

	void setBlendColorRatio(f32 r, f32 g, f32 b, f32 a)
	{
		unk104[0] = r;
		unk104[1] = g;
		unk104[2] = b;
		unk104[3] = a;
		setBlendKonstColor();
	}
	void setBlendAlphaRatio(f32 r, f32 g, f32 b, f32 a)
	{
		unk114[0] = r;
		unk114[1] = g;
		unk114[2] = b;
		unk114[3] = a;
		setBlendKonstAlpha();
	}

	bool append(JUTTexture* texture, f32 param_2)
	{
		return insert(texture, mTextureNum, param_2);
	}

	JUtility::TColor getWhite() const { return mWhite; }
	void setWhite(JUtility::TColor white) { mWhite = white; }
	JUtility::TColor getBlack() const { return mBlack; }
	void setBlack(JUtility::TColor black) { mBlack = black; }
	void setBlackWhite(JUtility::TColor black, JUtility::TColor white)
	{
		mBlack = black;
		mWhite = white;
	}

	void setMirror(J2DMirror mirror) { mMirror = mirror; }
	J2DMirror getMirror() const { return mMirror; }

protected:
	/* 0xEC */ JUTTexture* mTextures[4];
	/* 0xFC */ u8 mTextureNum;
	/* 0xFD */ u8 unkFD[4];
	/* 0x104 */ f32 unk104[4];
	/* 0x114 */ f32 unk114[4];
	/* 0x124 */ JUTPalette* mPalette;
	/* 0x128 */ J2DBinding mBinding;
	/* 0x12C */ J2DMirror mMirror;
	/* 0x130 */ bool unk130;
	/* 0x134 */ J2DWrapmode mWrapmodeHor;
	/* 0x138 */ J2DWrapmode mWrapmodeVer;
	/* 0x13C */ JUtility::TColor mWhite;
	/* 0x140 */ JUtility::TColor mBlack;
	/* 0x144 */ JUtility::TColor mCornerColor[4];
	/* 0x154 */ JUtility::TColor mBlendKonstColor;
	/* 0x158 */ JUtility::TColor mBlendKonstAlpha;
};

#endif /* J2DPICTURE_H */
