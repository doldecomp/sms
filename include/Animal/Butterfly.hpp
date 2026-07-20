#ifndef ANIMAL_BUTTERFLY_HPP
#define ANIMAL_BUTTERFLY_HPP

#include <Animal/fishoid.hpp>

class TButterfloid;

class TButterfly : public TRealoidActor {
public:
	TButterfly(MActor*, TButterfloid*);

	virtual BOOL receiveMessage(THitActor*, u32);

	void init();

public:
	/* 0xA8 */ TButterfloid* unkA8;
};

class TButterfloid : public TRealoid {
public:
	TButterfloid(int, const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual TButterfly* createRealoidActor(MActor*);

	void init(TLiveManager*);
	void initBoids();
	void receiveMessageFromChild(TButterfly*);
	void loadItem(JSUMemoryInputStream&);

public:
	/* 0x158 */ int mType;
	/* 0x15C */ TMapObjBase* mItem;
	/* 0x160 */ int mNumEaten;
};

class TButterfloidManager : public TEnemyManager {
public:
	TButterfloidManager(const char*);

	virtual void createModelData();
};

#endif
