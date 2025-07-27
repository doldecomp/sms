#ifndef PLAYER_SPLASH_MANAGER_HPP
#define PLAYER_SPLASH_MANAGER_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TSplashManager;
extern TSplashManager* gpSplashManager;

class TSplashManager : public JDrama::TViewObj {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	void newSplash(JGeometry::TVec3<f32>, f32);
	void move();
	void makeDL(JDrama::TGraphics*) const;
	void draw() const;
};

#endif
