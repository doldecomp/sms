#ifndef ENEMY_KOOPA_HPP
#define ENEMY_KOOPA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class TKoopaParams;
class TKoopaFlame;
class TKoopaHand;
class TKoopaHead;
class TKoopaBody;

class TKoopa : public TSpineEnemy {
public:
	TKoopa(const char*);
	virtual ~TKoopa() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void updateAnmSound();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void fall();
	void stagger(bool);
	void getDown();
	bool allowsLaunch() const;
	bool getShowered();
	void effectsTumble() const;
	f32 getTargetDir(const JGeometry::TVec3<f32>&) const;
	f32 getNeckFocus() const;
	bool isFlaming() const;
	f32 getFlameDirRate() const;
	f32 getFlameDirDegree() const;
	void changeAnm(int, int, f32);
	void setUpHitActors();
	void breathFlame();
	bool canTumble() const;
	void checkMarioWhichSide();
	bool endsAnm() const;
	f32 getAnmEnd() const;
	f32 getAnmFrameNext() const;
	f32 getAnmFrame() const;
	int getAnmIndex() const;
	MtxPtr getHeadMtx() const;
	TKoopaParams* getParam() const;
	bool ignoresMario() const;
	bool isBreathing() const;
	bool isProvoking() const;
	bool isTumbling() const;
	void laugh();
	bool passesAnmFrame(f32) const;
	void resetFlame_();
	void setIgnoreMario(s32);
	void stopFlame();

public:
	/* 0x150 */ f32 unk150;
	/* 0x154 */ u8 unk154;
	/* 0x155 */ u8 unk155;
	/* 0x158 */ JGeometry::TVec3<f32> unk158;
	/* 0x164 */ TKoopaFlame* unk164[10];
	/* 0x18C */ TKoopaHand* unk18C[2];
	/* 0x194 */ TKoopaHead* unk194;
	/* 0x198 */ TKoopaBody* unk198;
	/* 0x19C */ s32 unk19C;
	/* 0x1A0 */ s32 unk1A0;
	/* 0x1A4 */ s32 unk1A4;
	/* 0x1A8 */ s32 unk1A8;
	/* 0x1AC */ u8 unk1AC[0xC];
	/* 0x1B8 */ f32 unk1B8;
};

class TKoopaManager : public TEnemyManager {
public:
	TKoopaManager(const char*);

	virtual ~TKoopaManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

#endif
