#ifndef ENEMY_BOSS_GESSO_TENTACLE_HPP
#define ENEMY_BOSS_GESSO_TENTACLE_HPP

#include <Strategic/TakeActor.hpp>
#include <System/Params.hpp>
#include <System/ParamInst.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>

class TBossGesso;
class THitActor;
class TSplinePath;
class MActor;
class TBGTentacle;

class TBGTentacleMtxCalc : public J3DMtxCalcSoftimageAnm {
public:
	TBGTentacleMtxCalc(TBGTentacle*);

	virtual void calc(u16);

public:
	/* 0x68 */ TBGTentacle* mOwner;
};

class TBGTakeHit : public TTakeActor {
public:
	TBGTakeHit(TBGTentacle*, const char* name = "イカ足（つかみ）");

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual MtxPtr getTakingMtx();
	virtual bool moveRequest(const JGeometry::TVec3<f32>&);

	void enableAttackCheck();
	void disableAttackCheck();

public:
	/* 0x70 */ TBGTentacle* mOwner;
	/* 0x74 */ JGeometry::TVec3<f32> unk74;
	/* 0x80 */ TMtx34f unk80;
};

class TBGAttackHit : public THitActor {
public:
	TBGAttackHit(TBGTentacle*, f32, const char* name = "イカ足（当たり）");
	virtual void perform(u32, JDrama::TGraphics*);

	f32 getUnk6C() const { return unk6C; }
	void setUnk6C(f32 v) { unk6C = v; }

public:
	/* 0x68 */ TBGTentacle* mOwner;
	/* 0x6C */ f32 unk6C;
};

class TBGTentacle : public JDrama::TViewObj {
public:
	// TODO: are the values correct? Why is mState == 10 used in code then?
	enum {
		TSTATE_WAIT       = 0,
		TSTATE_ATTACK     = 1,
		TSTATE_REST       = 2,
		TSTATE_HELD       = 3,
		TSTATE_AMPUTEE    = 4,
		TSTATE_STUN       = 5,
		TSTATE_HIDE       = 6,
		TSTATE_FOLLOWBODY = 7,
		TSTATE_SYNCBODY   = 8,
		TSTATE_GUARD      = 9,
	};

	class TTentacleParams : public TParams {
	public:
		TTentacleParams(const char*);

		/* 0x8 */ TParamRT<f32> mNodeLength;
		/* 0x1C */ TParamRT<f32> mNodeLenLimit;
		/* 0x30 */ TParamRT<f32> mTotalLenLimit;
		/* 0x44 */ TParamRT<f32> mSpeedProp;
		/* 0x58 */ TParamRT<f32> mInertiaProp;
		/* 0x6C */ TParamRT<f32> mReflectProp;
		/* 0x80 */ TParamRT<f32> mSpeedMax;
		/* 0x94 */ TParamRT<f32> mVibrationForce;
		/* 0xA8 */ TParamRT<f32> mVibrationSpeed;
		/* 0xBC */ TParamRT<f32> mDamagePropF;
		/* 0xD0 */ TParamRT<f32> mDamagePropS;
		/* 0xE4 */ TParamRT<s32> mDamageCountMax;
		/* 0xF8 */ TParamRT<f32> mDamageReflectProp;
		/* 0x10C */ TParamRT<f32> mDamageNodeLength;
		/* 0x120 */ TParamRT<f32> mAttackRadius;
		/* 0x134 */ TParamRT<f32> mAttackHeight;
		/* 0x148 */ TParamRT<f32> mDamageRadius;
		/* 0x15C */ TParamRT<f32> mDamageHeight;
		/* 0x170 */ TParamRT<f32> mAtkSoundTime;
	};

	class TNode {
	public:
		TNode();
		void calcVelocity(TBGTentacle*, TNode*, f32);
		void calcPosition(TBGTentacle*);

		// fabricated but I am 99.99% sure they are real
		const JGeometry::TVec3<f32>& getPosition() const { return mPosition; }
		const JGeometry::TVec3<f32>& getVelocity() const { return mVelocity; }
		void setPosition(const JGeometry::TVec3<f32>& v) { mPosition = v; }
		void setVelocity(const JGeometry::TVec3<f32>& v) { mVelocity = v; }

		// fabricated
		void setUnk18(const JGeometry::TVec3<f32>& v) { unk18 = v; }
		void addVelocity(const JGeometry::TVec3<f32>& v) { mVelocity += v; }

		void onUnk24() { unk24 = true; }
		void offUnk24() { unk24 = false; }
		bool isUnk24() const { return unk24; }

	private:
		/* 0x0 */ JGeometry::TVec3<f32> mPosition;
		/* 0xC */ JGeometry::TVec3<f32> mVelocity;
		/* 0x18 */ JGeometry::TVec3<f32> unk18;
		/* 0x24 */ BOOL unk24;
	};

	TBGTentacle(TBossGesso* owner, int node_num, int index);

	virtual void perform(u32, JDrama::TGraphics*);

	void incDamage();
	void throwMario(THitActor*, THitActor*);
	BOOL isAttacking() const;
	bool canTake() const;
	f32 getNodeLen() const;
	void continuousRumble();
	void beatNode(int, const JGeometry::TVec3<f32>&);
	void setAttackTarget();
	void changeStateAndFixNodes(int new_state);
	void returnToDefaultState();
	void moveNode();
	void moveConstraint();
	void decideOwnState();
	void checkDamage();
	void calcAtkParticleAndSE();
	void decideAtkColExists();
	void calcAttackGuideAnm();
	void resetAllNodes(const JGeometry::TVec3<f32>&);

	// fabricated
	BOOL isThing()
	{
		if (mState == 6 || mState == 3 || mState == 4)
			return true;
		return false;
	}

	// fabricated
	BOOL isThing2()
	{
		if (mState == 4 || mState == 6)
			return true;
		return false;
	}

	// fabricated
	bool isThing3()
	{
		if (mState == 10)
			return false;
		if (mState == 4)
			return false;
		if (mState == 6)
			return false;
		return true;
	}

	// fabricated
	TNode* getFirstNode() { return &mNodes[0]; }
	TNode* getLastNode() { return &mNodes[mNodeNum - 1]; }
	int getState() const { return mState; }
	TTentacleParams* getParams() { return unk38; }
	MActor* getUnk2C() { return unk2C; }
	TBossGesso* getOwner() { return mOwner; }

public:
	/* 0x10 */ int mState;
	/* 0x14 */ int unk14;
	/* 0x18 */ int mIndex;
	/* 0x1C */ int mDamageCount;
	/* 0x20 */ int mNodeNum;
	/* 0x24 */ TNode* mNodes;
	/* 0x28 */ TSplinePath* unk28;
	/* 0x2C */ MActor* unk2C;
	/* 0x30 */ TBGTentacleMtxCalc* unk30;
	/* 0x34 */ TBossGesso* mOwner;
	/* 0x38 */ TTentacleParams* unk38;
	/* 0x3C */ TBGTakeHit* unk3C;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ int unk48;
	/* 0x4C */ int unk4C;
	/* 0x50 */ Mtx unk50;
	/* 0x80 */ MActor* unk80;
	/* 0x84 */ JGeometry::TVec3<f32> unk84;
	/* 0x90 */ TBGAttackHit* unk90[5];
};

#endif
