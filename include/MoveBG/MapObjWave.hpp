#ifndef MOVE_BG_MAP_OBJ_WAVE_HPP
#define MOVE_BG_MAP_OBJ_WAVE_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TMapObjWave;

extern TMapObjWave* gpMapObjWave;

class TMapObjWave : public JDrama::TViewObj {
public:
	TMapObjWave(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	void movement();
	void updateTime();
	void updateHeightAndAlpha();
	void draw();
	void getAlpha(float, float) const;
	void noWave();
	f32 getHeight(float, float, float) const;
	f32 getWaveHeight(float, float) const;
	void getStaticTexPos0(float) const;
	void getStaticTexPos1(float) const;
	void getMoveTexPos0(float) const;
	void getMoveTexPos1(float) const;
	void initDraw();
};

#endif
