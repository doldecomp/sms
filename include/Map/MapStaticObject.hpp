#ifndef MAP_MAP_STATIC_OBJECT_HPP
#define MAP_MAP_STATIC_OBJECT_HPP

#include <Strategic/HitActor.hpp>

class JPABaseEmitter;
class J3DModelData;
class J3DModel;
class TGraphWeb;
class MSSceneSE;
class MActor;
class TMapCollisionManager;

class TMapStaticObj : public THitActor {
public:
	TMapStaticObj(const char* name = "簡易マップオブジェ");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();

	void init(const char*);
	void initModel(const char*);
	void initMapCollision(const char*);
	void initUnique();
	void perform(u32 cue, JDrama::TGraphics* graphics);
	void calcUnique(JPABaseEmitter*);
	J3DModel* getModel() const;
	J3DModelData* getModelData() const;

	struct TActorData {
		/* 0x0 */ const char* mActorName;
		/* 0x4 */ u32 mActorType;
		/* 0x8 */ u32 mHitFlags;
		/* 0xC */ f32 mAttackRadius;
		/* 0x10 */ f32 mAttackHeight;
		/* 0x14 */ f32 mDamageRadius;
		/* 0x18 */ f32 mDamageHeight;
		/* 0x1C */ const char* mIdxGroupName;
		/* 0x20 */ const char* mModelFileName;
		/* 0x24 */ u32 unk24;
		/* 0x28 */ const char* mColFileName;
		/* 0x2C */ u32 unk2C;
		/* 0x30 */ u32 mSoundId;
		/* 0x34 */ const char* mParticlePath;
		/* 0x38 */ u32 mParticleId;
		/* 0x3C */ u8 mParticleType;

		enum {
			FLAG_IS_INDIRECT = 0x1,
			FLAG_UNK2        = 0x2,
			FLAG_UNK4        = 0x4,
			FLAG_UNK8        = 0x8,
			FLAG_UNK10       = 0x10,
			FLAG_UNK20       = 0x20,
			FLAG_UNK40       = 0x40,
			FLAG_UNK80       = 0x80,
		};

		/* 0x40 */ u32 mFlags;
	};

	static f32 mEffectCoronaScale;

public:
	/* 0x68 */ const TActorData* mActorData;
	/* 0x6C */ const char* mActorName;
	/* 0x70 */ MActor* mMActor;
	/* 0x74 */ TMapCollisionManager* mCollisionManager;
	/* 0x78 */ u32 mSoundId;
};

class TMapModelActor : public THitActor {
public:
	TMapModelActor(const char* name)
	    : THitActor(name)
	    , unk68(nullptr)
	{
	}

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	// fabricated
	void setActor(MActor* actor) { unk68 = actor; }

public:
	/* 0x68 */ MActor* unk68;
};

class TMapObjSoundGroup : public JDrama::TViewObj {
public:
	TMapObjSoundGroup(const char* name = "環境音テスト");
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

public:
	/* 0x10 */ MSSceneSE* unk10;
	/* 0x14 */ TGraphWeb* unk14;
	/* 0x18 */ u32 unk18;
};

#endif
