#ifndef ENEMY_ENEMY_ATTACHMENT_HPP
#define ENEMY_ENEMY_ATTACHMENT_HPP

#include <Enemy/Enemy.hpp>

class SDLModelData;
class TSharedParts;

class TEnemyAttachment : public TSpineEnemy {
public:
	TEnemyAttachment(const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32) { return FALSE; }
	virtual void calcRootMatrix();
	virtual void bind();
	virtual void moveObject();
	virtual void kill();
	virtual void loadInit(TSpineEnemy*, const char*);
	virtual void appear()
	{
		unk150     = 1;
		mScaling.x = mScaling.y = mScaling.z = 0.01f;
	}
	virtual void rebirth();
	virtual void sendMessage();
	virtual void set();
	virtual void behaveToHost() { }
	virtual void behaveToHitGround();
	virtual void behaveToHitWall(const TBGCheckData*);
	virtual void forceKill();
	virtual void setBehavior() { }
	virtual void recoverScale();
	virtual f32 getNowGravity() { return mGravity; }

	bool isUnk150Zero() const { return unk150 == 0 ? true : false; }

public:
	/* 0x150 */ int unk150;
	/* 0x154 */ u32 unk154;
	/* 0x158 */ int unk158;
	/* 0x15C */ char unk15C[4];
	/* 0x160 */ TSpineEnemy* unk160;
	/* 0x164 */ f32 unk164;
	/* 0x168 */ u8 unk168;
};

class TEnemyPolluteModel;

class TEnemyPolluteModelManager : public JDrama::TViewObj {
public:
	TEnemyPolluteModelManager(const char* name)
	    : JDrama::TViewObj(name)
	    , unk10(0)
	    , unk14(5)
	    , unk18(nullptr)
	{
	}

	virtual void perform(u32, JDrama::TGraphics*);
	virtual void init(TLiveActor*);

	void generatePolluteModel(JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>&);

public:
	/* 0x10 */ int unk10;
	/* 0x14 */ int unk14;
	/* 0x18 */ TEnemyPolluteModel** unk18;
};

class TEnemyPolluteModel : public JDrama::TViewObj {
public:
	TEnemyPolluteModel(TLiveActor*, int, SDLModelData*, const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual void setAnm() { }

	void generate(JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>&);

public:
	/* 0x10 */ TSharedParts* unk10;
	/* 0x14 */ TPosition3f unk14;
	/* 0x44 */ Vec unk44;
	/* 0x50 */ Vec unk50;
	/* 0x5C */ u8 unk5C;
	/* 0x5D */ u8 unk5D;
};

#endif
