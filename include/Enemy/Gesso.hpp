#ifndef ENEMY_GESSO_HPP
#define ENEMY_GESSO_HPP

#include <Enemy/EnemyAttachment.hpp>
#include <Enemy/WalkerEnemy.hpp>

class TGessoSaveLoadParams : public TWalkerEnemyParams {
public:
	TGessoSaveLoadParams(const char* path);

	/* 0x32C */ TParamRT<f32> mSLSearchLengthOnObj;
	/* 0x340 */ TParamRT<f32> mSLSearchAngleOnObj;
	/* 0x354 */ TParamRT<f32> mSLSearchAwareOnObj;
	/* 0x368 */ TParamRT<f32> mSLPollutionLength;
	/* 0x37C */ TParamRT<s32> mSLPollutionInterval;
	/* 0x390 */ TParamRT<f32> mSLPolluteObjGravity;
	/* 0x3A4 */ TParamRT<f32> mSLPolluteObjSpeed;
	/* 0x3B8 */ TParamRT<f32> mSLPolluteObjLinerSp;
	/* 0x3CC */ TParamRT<f32> mSLPolluteObjLinerG;
	/* 0x3E0 */ TParamRT<f32> mSLDropGravityY;
	/* 0x3F4 */ TParamRT<f32> mSLBodyAngMax;
	/* 0x408 */ TParamRT<f32> mSLPolluteModelScale;
	/* 0x41C */ TParamRT<f32> mSLTurnLength;
	/* 0x430 */ TParamRT<f32> mSLHitWaterSpXZ;
	/* 0x444 */ TParamRT<f32> mSLHitWaterSpY;
};

class TGessoPolluteModelManager : public TEnemyPolluteModelManager {
public:
	TGessoPolluteModelManager(const char* name = "ゲッソーモデル汚染")
	    : TEnemyPolluteModelManager(name)
	{
	}

	virtual void init(TLiveActor*);
};

class TGessoPolluteModel : public TEnemyPolluteModel {
public:
	TGessoPolluteModel(TLiveActor* actor, SDLModelData* model_data)
	    : TEnemyPolluteModel(actor, 0, model_data, "汚染モデル")
	{
	}

	virtual void setAnm();
};

class TGesso;

class TGessoManager : public TSmallEnemyManager {
public:
	TGessoManager(const char* name = "ゲッソーマネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
	virtual TSmallEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*);
	virtual void initSetEnemies();

	void requestPolluteModel(JGeometry::TVec3<f32>& position,
	                         JGeometry::TVec3<f32>& scale);

	TGesso* getObj(int i) { return (TGesso*)TSmallEnemyManager::getObj(i); }

public:
	/* 0x60 */ TGessoPolluteModelManager* unk60;
};

class TGessoPolluteObj;

class TGesso : public TWalkerEnemy {
public:
	TGesso(const char* name = "ゲッソー");

	virtual void load(JSUMemoryInputStream&);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual void kill();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void genRandomItem();
	virtual void setBehavior();
	virtual void behaveToWater(THitActor*);
	virtual void setWalkAnm();
	virtual void setDeadAnm();
	virtual void setWaitAnm();
	virtual void setRunAnm() { setWalkAnm(); }
	virtual void attackToMario();
	virtual void initAttacker(THitActor*);
	virtual void setAfterDeadEffect();
	virtual bool doKeepDistance();
	virtual void behaveToFindMario();
	virtual void walkBehavior(int, f32);
	virtual bool isResignationAttack();

	void polluteBehavior();
	void setPolluteGoal();
	void pollute();
	void isUseBodyCallBack() const;
	void rollCheck();
	void rollEnd();
	void modifyRotate();
	void fallEnd();
	void turnIn();
	bool turning();
	void turnOut();
	void checkDropInWater();

	static u8 mBodyJntIndex;
	static u8 mMouthJntIndex;
	static f32 mBodyRotSpeed;
	static f32 mPollRange;
	static f32 mThroughHoseiDistY;
	static f32 mAngTestY;

	// fabricated
	TGessoSaveLoadParams* getSaveParams() const { return unk1E8; }

	inline f32 getSightDirection() const
	{
		if (mState == STATE_WANDERING)
			return 0.0f;

		if (unk1A1 != 0) {
			if (unk1C4 != 0) {
				return 0.0f;
			} else {
				return 180.0f;
			}
		} else {
			if (unk1C4 != 0)
				return 90.0f;
			else
				return 270.0f;
		}
	}

	f32 getUnk1B0() const { return mTurnAngle; }

	TGessoManager* getManager() { return (TGessoManager*)mManager; }

	TGessoPolluteObj* getPolluteObj() const { return mPolluteObj; }

	bool isWandering() const
	{
		if (mState == STATE_WANDERING)
			return true;
		return false;
	}
	bool isNotWandering() const
	{
		if (mState == STATE_WANDERING)
			return false;
		return true;
	}

	enum {
		STATE_BEAM_CHILLING = 0,
		STATE_WANDERING     = 1,
		STATE_ROLLING       = 2,
		STATE_FALLING       = 3,
	};

	enum {
		TYPE_REGULAR = 0,
		TYPE_LAND    = 1,
		TYPE_SURF    = 2,
	};

public:
	/* 0x194 */ int mAttackCooldown;
	/* 0x198 */ int mPollutionTimer;
	/* 0x19C */ int mState;
	/* 0x1A0 */ bool mIsRightSideUp;
	/* 0x1A1 */ u8 unk1A1;
	/* 0x1A4 */ f32 unk1A4;
	/* 0x1A8 */ TGessoPolluteObj* mPolluteObj;
	/* 0x1AC */ u8 mGessoType;
	/* 0x1B0 */ f32 mTurnAngle;
	/* 0x1B4 */ int unk1B4;
	/* 0x1B8 */ JGeometry::TVec3<f32> mPolluteVelocity;
	/* 0x1C4 */ u8 unk1C4;
	/* 0x1C8 */ f32 mStayYaw;
	/* 0x1CC */ f32 mBodyTrackingAngle;
	/* 0x1D0 */ f32 unk1D0;
	/* 0x1D4 */ char unk1D4[0x4];
	/* 0x1D8 */ u8 unk1D8;
	/* 0x1D9 */ u8 mNeedsLanding;
	/* 0x1DA */ char unk1DA[0x2];
	/* 0x1DC */ JGeometry::TVec3<f32> unk1DC;
	/* 0x1E8 */ TGessoSaveLoadParams* unk1E8;
};

class TSurfGesso : public TGesso {
public:
	TSurfGesso(const char* name = "サーフゲッソー");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual bool isFindMario(f32) { return false; }
};

class TLandGesso : public TGesso {
public:
	TLandGesso(const char* name = "平地ゲッソー");

	virtual void load(JSUMemoryInputStream&);
};

class TGessoPolluteObj : public TEnemyAttachment {
public:
	TGessoPolluteObj(const char* name = "ゲッソー汚染物");

	virtual void calcRootMatrix();
	virtual void loadInit(TSpineEnemy*, const char*);
	virtual void rebirth();
	virtual void sendMessage();
	virtual void set();
	virtual f32 getNowGravity();
	virtual void pollute();

public:
	/* 0x16C */ TGesso* unk16C;
};

DECLARE_NERVE(TNerveGessoStay, TLiveActor);
DECLARE_NERVE(TNerveGessoFreeze, TLiveActor);
DECLARE_NERVE(TNerveGessoPunch, TLiveActor);
DECLARE_NERVE(TNerveGessoPollute, TLiveActor);
DECLARE_NERVE(TNerveGessoFall, TLiveActor);
DECLARE_NERVE(TNerveGessoRolling, TLiveActor);
DECLARE_NERVE(TNerveGessoFindMario, TLiveActor);
DECLARE_NERVE(TNerveGessoLand, TLiveActor);
DECLARE_NERVE(TNerveGessoTurn, TLiveActor);

#endif
