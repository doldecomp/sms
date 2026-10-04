#ifndef ENEMY_TINKOOPA_HPP
#define ENEMY_TINKOOPA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class TCoasterKiller;
class TGraphWeb;
class TTinKoopaFlame;
class TTinKoopaPartsBase;

class TTinKoopa : public TSpineEnemy {
public:
	TTinKoopa(const char*);
	virtual ~TTinKoopa() { }
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual BOOL hasMapCollision() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void emitTinKoopaEffects();
	void checkTinKoopaFirstFlameMessage();
	void checkTinKoopaKillerApproachingMessage();
	void launchKiller(int);
	void hitParts();
	void resetTinKoopa();
	f32 calcCoasterDistance(int, int);
	f32 calcCoasterDistanceInOrder(int, int);
	void makeLaunchSchedule();
	void changeBck(int);
	bool checkKillerApproachingFromBack(TCoasterKiller*, JGeometry::TVec3<f32>,
	                                    f32);
	void checkKillerLaunch();
	void checkLap();
	void checkTinKoopaFirstRocketMessage();
	void checkTinKoopaMessage();
	bool checkTruckAnimationPass(int);
	void makeCoasterDistanceTable();
	void makeEyeBeamEffect();
	void makeHitCollision();
	void makeKillerQueue(int, s8);
	void startBreakingParts();
	void startTinKoopaMessage(u32);
	void updateTimers();

public:
	/* 0x150 */ s32 unk150;
	/* 0x154 */ s32 unk154;
	/* 0x158 */ s32 unk158;
	/* 0x15C */ s32 unk15C;
	/* 0x160 */ TTinKoopaFlame* unk160;
	/* 0x164 */ MActor* unk164;
	/* 0x168 */ u8 unk168;
	/* 0x169 */ u8 unk169;
	/* 0x16A */ u8 unk16A;
	/* 0x16B */ u8 unk16B;
	/* 0x16C */ u8 unk16C;
	/* 0x170 */ s32 unk170;
	/* 0x174 */ s32 unk174;
	/* 0x178 */ s32 unk178;
	/* 0x17C */ s32 unk17C;
	/* 0x180 */ s32 unk180;
	/* 0x184 */ JGeometry::TVec3<f32> unk184;
	/* 0x190 */ JGeometry::TVec3<f32> unk190;
	/* 0x19C */ JGeometry::TVec3<f32> unk19C;
	/* 0x1A8 */ JGeometry::TVec3<f32> unk1A8;
	/* 0x1B4 */ f32 unk1B4;
	/* 0x1B8 */ f32 unk1B8;
	/* 0x1BC */ f32 unk1BC;
	/* 0x1C0 */ f32 unk1C0;
	/* 0x1C4 */ u8 unk1C4[4];
	/* 0x1C8 */ s32 unk1C8;
	/* 0x1CC */ TTinKoopaPartsBase* unk1CC[6];
	/* 0x1E4 */ TTinKoopaPartsBase* unk1E4;
	/* 0x1E8 */ f32* unk1E8;
	/* 0x1EC */ TGraphWeb* unk1EC;
	/* 0x1F0 */ TEnemyManager* unk1F0;
	/* 0x1F4 */ void* unk1F4;
	/* 0x1F8 */ s32 unk1F8;
};

class TTinKoopaManager : public TEnemyManager {
public:
	TTinKoopaManager(const char*);

	virtual ~TTinKoopaManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual BOOL hasMapCollision() const;
	virtual TSpineEnemy* createEnemyInstance();
};

#endif
