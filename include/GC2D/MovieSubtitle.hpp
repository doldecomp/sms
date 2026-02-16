#ifndef GC2D_MOVIE_SUBTITLE_HPP
#define GC2D_MOVIE_SUBTITLE_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <GC2D/MessageLoader.hpp>

class TTHPRender;
class JKRArchive;
class J2DSetScreen;
class J2DTextBox;
class J2DTextBox;
class TMessageLoader;

class TMovieSubTitle : public JDrama::TViewObj {
public:
	TMovieSubTitle(const TTHPRender*);

	virtual void perform(u32, JDrama::TGraphics*);

	void setupResource(const char*, JKRArchive*);
	void movement();
	void checkSubTitleOff();
	void checkSubTitleOn();
	void show();
	void hide();
	const TMessageLoader::EntryInfo* getCurEntry() const;
	void setCurMessage();
	void makeBmgName(char*, int, const char*);
	void draw(JDrama::TGraphics*);

public:
	/* 0x10 */ const TTHPRender* unk10;
	/* 0x14 */ J2DSetScreen* unk14;
	/* 0x18 */ J2DTextBox* unk18;
	/* 0x1C */ J2DTextBox* unk1C;
	/* 0x20 */ TMessageLoader* unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ bool unk28;
};

#endif
