#ifndef J2DWINDOW_H
#define J2DWINDOW_H

#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JUtility/JUTPalette.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/J2D/J2DPane.hpp>

class JSURandomInputStream;
class JUTTexture;

struct J2DTextureBase {
	// ???
};

struct J2DWindowMirror { };

class J2DWindow : public J2DPane {
public:
	struct TContentsColor {
		TContentsColor() { }
		/* 0x0 */ JUtility::TColor field_0x0;
		/* 0x4 */ JUtility::TColor field_0x4;
		/* 0x8 */ JUtility::TColor field_0x8;
		/* 0xC */ JUtility::TColor field_0xc;
	}; // Size: 0x10

	struct Texture : JUTTexture {
		Texture(const ResTIMG* timg)
		    : JUTTexture(timg)
		{
		}

		virtual ~Texture();
		void draw(int, int, int, int, u16, u16, u16, u16, u8, JUtility::TColor,
		          JUtility::TColor);
		void draw(int, int, bool, bool, u8, JUtility::TColor, JUtility::TColor);
		void drawContentsTexture(int, int, int, int, u8);
		void setTevMode(u8, JUtility::TColor, JUtility::TColor);
	};

	J2DWindow(const ResTIMG*, const ResTIMG*, const ResTIMG*, const ResTIMG*);
	J2DWindow(const char*, const char*, const char*, const char*);
	J2DWindow(const ResTIMG*, J2DTextureBase);
	J2DWindow(const char*, J2DTextureBase);
	J2DWindow(J2DPane*, JSURandomInputStream*, bool);
	J2DWindow(u32, const JUTRect&, const ResTIMG*, const ResTIMG*,
	          const ResTIMG*, const ResTIMG*, const ResTLUT*);
	J2DWindow(u32, const JUTRect&, const char*, const char*, const char*,
	          const char*, const ResTLUT*);
	J2DWindow(u32, const JUTRect&, const ResTIMG*, J2DTextureBase,
	          const ResTLUT*);
	J2DWindow(u32, const JUTRect&, const char*, J2DTextureBase, const ResTLUT*);

	void initiate(const ResTIMG*, const ResTIMG*, const ResTIMG*,
	              const ResTIMG*, const ResTLUT*, J2DWindowMirror,
	              const JUTRect&);
	void initiateColor();
	void draw(const JUTRect&);
	void draw_private(const JUTRect&, const JUTRect&, Mtx*);
	void draw(const JUTRect&, const JUTRect&);
	void setContentsColor(JUtility::TColor, JUtility::TColor, JUtility::TColor,
	                      JUtility::TColor);
	void drawContents(const JUTRect&);

	virtual ~J2DWindow();
	virtual void resize(int, int);
	virtual void drawSelf(int, int);
	virtual void drawSelf(int, int, Mtx*);

protected:
	/* 0xEC */ JUTRect unkEC;
	/* 0xFC */ void* unkFC;
	/* 0x100 */ Texture* unk100;
	/* 0x104 */ Texture* unk104;
	/* 0x108 */ Texture* unk108;
	/* 0x10C */ Texture* unk10C;
	/* 0x110 */ Texture* unk110;
	/* 0x114 */ int unk114;
	/* 0x118 */ JUtility::TColor unk118;
	/* 0x120 */ JUtility::TColor unk120;
	/* 0x11C */ JUtility::TColor unk11C;
	/* 0x124 */ JUtility::TColor unk124;
	/* 0x128 */ JUtility::TColor unk128;
	/* 0x12C */ JUtility::TColor unk12C;
	/* 0x130 */ int unk130;
	/* 0x134 */ int unk134;
	/* 0x138 */ int unk138;
};

#endif
