#ifndef ENEMY_GENERATOR_HPP
#define ENEMY_GENERATOR_HPP

#include <Strategic/HitActor.hpp>

class TGenerator : public JDrama::TViewObj {
public:
	TGenerator(const char*);
	void load(JSUMemoryInputStream&);
	void perform(u32, JDrama::TGraphics*);
};

class TOneShotGenerator : public THitActor {
public:
	TOneShotGenerator(const char*);
	void load(JSUMemoryInputStream&);
	void loadAfter();
	BOOL receiveMessage(THitActor*, u32);
};

#endif
