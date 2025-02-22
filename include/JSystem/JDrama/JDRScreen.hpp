#ifndef JDR_SCREEN_HPP
#define JDR_SCREEN_HPP

#include <JSystem/JDrama/JDRViewConnecter.hpp>

class TCamConnecter : public JDrama::TViewConnecter {
public:
	TCamConnecter()
	    : JDrama::TViewConnecter(nullptr, nullptr, 0, "<CamConnecter>")
	{
	}

	virtual ~TCamConnecter() { }
	virtual void perform(u32, JDrama::TGraphics*);
};

namespace JDrama {

class TScreen : public TViewConnecter {
public:
	TScreen(const TRect&, const char*);

	virtual ~TScreen() { }

	void isScissor() const;
	void getCamera() const;
	void assignCamera(TViewObj*);
	void getViewObj() const;
	void assignViewObj(TViewObj*);
};

}; // namespace JDrama

#endif
