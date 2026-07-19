#ifndef ENEMY_DEBU_TELESA_HPP
#define ENEMY_DEBU_TELESA_HPP

#include <Enemy/SmallEnemy.hpp>

class TDebuTelesaParams : public TSmallEnemyParams {
public:
	TDebuTelesaParams(const char* path);
};

class TDebuTelesa : public TSmallEnemy {
public:
	TDebuTelesa(const char* name);

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager* manager);
	virtual void calcRootMatrix();
	virtual void kill();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual bool changeByJuice() { return false; }
	virtual void attackToMario();
	virtual bool isCollidMove(THitActor*);
	virtual bool doKeepDistance();

	void initCollision();
	void setDeadAnm();
	void emitEffects();
	bool isDying() const;

public:
	/* 0x194 */ u32 unk194;
	/* 0x198 */ s32 mNullYodareJointIdx;
	/* 0x19C */ s32 mRightHandJointIdx;
	/* 0x1A0 */ JGeometry::TVec3<f32> mRightHandPos;
};

class TDebuTelesaManager : public TSmallEnemyManager {
public:
	TDebuTelesaManager(const char* name);

	virtual void load(JSUMemoryInputStream& stream);
	virtual void createModelData();
	virtual void clipEnemies(JDrama::TGraphics* graphics);

	// fabricated
	TDebuTelesa* getObj(int idx)
	{
		return (TDebuTelesa*)TObjManager::getObj(idx);
	}
};

DECLARE_NERVE(TNerveDebuTelesaWait, TLiveActor);

#endif
