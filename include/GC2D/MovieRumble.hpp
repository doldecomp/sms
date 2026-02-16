#ifndef GC2D_MOVIE_RUMBLE_HPP
#define GC2D_MOVIE_RUMBLE_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TTHPRender;
namespace Koga {
class ToolData;
}

class TMovieRumble : public JDrama::TViewObj {
public:
	TMovieRumble(const TTHPRender*);

	virtual void perform(u32, JDrama::TGraphics*);

	void init(const char*);
	void movement();
	void checkRumbleOn();
	void checkRumbleOff();
	void readCurInfo();
	void makeBcrName(char*, int, const char*);

	bool isValid() const
	{
		bool result = false;
		if (unk14 && unk18 >= 0)
			result = true;
		return result;
	}

	Koga::ToolData* getToolData() const { return unk14; }

public:
	/* 0x10 */ const TTHPRender* unk10;
	/* 0x14 */ Koga::ToolData* unk14;
	/* 0x18 */ int unk18;
	/* 0x1C */ s32 unk1C;
	/* 0x20 */ s32 unk20;
	/* 0x24 */ int unk24;
	/* 0x28 */ bool unk28;
};

#endif
