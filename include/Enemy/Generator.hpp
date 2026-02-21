#ifndef ENEMY_GENERATOR_HPP
#define ENEMY_GENERATOR_HPP

#include <Strategic/HitActor.hpp>

class TGenerator : public JDrama::TViewObj {
public:
	TGenerator(const char* name = "<TGenerator>");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32, JDrama::TGraphics*);
};

class TOneShotGenerator : public THitActor {
public:
	TOneShotGenerator(const char* name = "<TOneShotGenerator>");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
};

#endif
