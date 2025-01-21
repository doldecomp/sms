#ifndef J2D_SCREEN_HPP
#define J2D_SCREEN_HPP

#include <JSystem/J2D/J2DPane.hpp>
#include <JSystem/JUtility/JUTColor.hpp>

class JKRArchive;

class J2DScreen : public J2DPane {
public:
	J2DScreen()
	    : J2DPane(nullptr, 8, true, 'root', JUTRect(0, 0, 640, 480))
	    , mColor()
	{
		mbClipToParent = false;
	}

	virtual ~J2DScreen();
	virtual void drawSelf(int x, int y, Mtx* mtx);
	virtual J2DPane* search(u32 tag);
	virtual bool makeUserPane(u16, J2DPane*, JSURandomInputStream*);
	virtual bool makeUserPane(u32, J2DPane*, JSURandomInputStream*);

	void set(const char*, JKRArchive*);
	void set(JSURandomInputStream*);
	void makeHiearachyPanes(J2DPane*, JSURandomInputStream*, bool, bool, bool,
	                        long*);
	bool stop();
	void draw(int x, int y, const J2DGrafContext* pCtx);
	void gather(J2DPane**, u32, u32, int);

protected:
	/* 0xEC */ bool mbClipToParent;
	/* 0xED */ char padding[3];
	/* 0xF0 */ JUtility::TColor mColor;
};

class J2DSetScreen : J2DScreen {
public:
	J2DSetScreen(const char*, JKRArchive*);

	virtual ~J2DSetScreen() {};
	virtual bool makeUserPane(u16, J2DPane*, JSURandomInputStream*);
	virtual bool makeUserPane(u32, J2DPane*, JSURandomInputStream*);
};

#endif
