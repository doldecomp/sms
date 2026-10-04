#ifndef ENEMY_LIMITKOOPA_HPP
#define ENEMY_LIMITKOOPA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/KoopaJr.hpp>
#include <Enemy/EnemyManager.hpp>

class TLimitKoopaParams;
class TLimitKoopaFlame;
class TLimitKoopaHand;
class TLimitKoopaHead;
class TLimitKoopaBody;

class TLimitKoopa : public TSpineEnemy {
public:
	TLimitKoopa(const char*);
	virtual ~TLimitKoopa() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual f32 getGravityY() const;
	virtual void reset();

	void setUpHitActors();
	void startHipDrop();
	void breathFlame();
	void calcTargetDirection();
	void changeBck(int, f32);
	void checkMarioWhichSide();
	bool endsAnm() const;
	void fall();
	bool finishedTurn();
	f32 getAnmEnd() const;
	f32 getAnmFrame() const;
	int getAnmIndex() const;
	void getDown();
	f32 getFlameDir() const;
	MtxPtr getHeadMtx() const;
	f32 getNeckFocus() const;
	TLimitKoopaParams* getParam() const;
	bool getShowered();
	bool isBreathing() const;
	bool isFlaming() const;
	bool isTumbling() const;
	void makeDirection(f32);
	void moveHipDrop();
	void moveStop();
	void moveTurn();
	void resetLimitKoopa();
	void setAnimationIndex(int);
	void stagger(bool);
	void stopFlame();
	void updateTimers();

public:
	/* 0x150 */ s32 unk150;
	/* 0x154 */ s32 unk154;
	/* 0x158 */ s32 unk158;
	/* 0x15C */ JGeometry::TVec3<f32> unk15C;
	/* 0x168 */ u8 unk168;
	/* 0x16C */ TDirectionCalc unk16C;
	/* 0x170 */ f32 unk170;
	/* 0x174 */ u8 unk174[4];
	/* 0x178 */ TLimitKoopaFlame* unk178[10];
	/* 0x1A0 */ TLimitKoopaHand* unk1A0[2];
	/* 0x1A8 */ TLimitKoopaHead* unk1A8;
	/* 0x1AC */ TLimitKoopaBody* unk1AC;
	/* 0x1B0 */ s32 unk1B0;
	/* 0x1B4 */ s32 unk1B4;
	/* 0x1B8 */ s32 unk1B8;
	/* 0x1BC */ u8 unk1BC[0xC];
};

class TLimitKoopaManager : public TEnemyManager {
public:
	TLimitKoopaManager(const char*);

	virtual ~TLimitKoopaManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

#endif
