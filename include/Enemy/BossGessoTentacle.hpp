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
	TBGTentacleMtxCalc();
};

class TBGTakeHit : public TTakeActor {
public:
	TBGTakeHit(TBGTentacle*, const char* name = "イカ足（つかみ）");

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual MtxPtr getTakingMtx();
	virtual void moveRequest(const JGeometry::TVec3<f32>&);

	void enableAttackCheck();
	void disableAttackCheck();

public:
	/* 0x70 */ TBGTentacle* unk70;
	/* 0x74 */ Vec unk74;
	/* 0x80 */ Mtx unk80;
};

class TBGAttackHit : public THitActor {
public:
	TBGAttackHit(TBGTentacle*, f32, const char* name = "イカ足（当たり）");
	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x68 */ TBGTentacle* unk68;
	/* 0x6C */ f32 unk6C;
};

class TBGTentacle : public JDrama::TViewObj {
public:
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

	public:
		/* 0x0 */ Vec unk0;
		/* 0xC */ Vec unkC;
		/* 0x18 */ Vec unk18;
		/* 0x24 */ u32 unk24;
	};

	TBGTentacle(TBossGesso* owner, int, int index);

	virtual void perform(u32, JDrama::TGraphics*);

	void incDamage();
	void throwMario(THitActor*, THitActor*);
	void isAttacking() const;
	void canTake() const;
	void getNodeLen() const;
	void continuousRumble();
	void beatNode(int, const JGeometry::TVec3<f32>&);
	void setAttackTarget();
	void changeStateAndFixNodes(int);
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
		if (unk10 == 6 || unk10 == 3 || unk10 == 4)
			return true;
		return false;
	}

	// fabricated
	BOOL isThing2()
	{
		if (unk10 == 4 || unk10 == 6)
			return true;
		return false;
	}

	// fabricated
	void resetUnk24() { unk24->unk24 = 1; }

public:
	/* 0x10 */ int unk10;
	/* 0x14 */ int unk14;
	/* 0x18 */ int unk18;
	/* 0x1C */ int unk1C;
	/* 0x20 */ int unk20;
	/* 0x24 */ TNode* unk24;
	/* 0x28 */ TSplinePath* unk28;
	/* 0x2C */ MActor* unk2C;
	/* 0x30 */ TBGTentacleMtxCalc* unk30;
	/* 0x34 */ TBossGesso* unk34;
	/* 0x38 */ TTentacleParams* unk38;
	/* 0x3C */ TBGTakeHit* unk3C;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ char unk48[4];
	/* 0x4C */ int unk4C;
	/* 0x50 */ Mtx unk50;
	/* 0x80 */ MActor* unk80;
	/* 0x84 */ Vec unk84;
	/* 0x90 */ TBGAttackHit* unk90[4];
	/* 0xA0 */ char unkA0[4];
};

#endif
