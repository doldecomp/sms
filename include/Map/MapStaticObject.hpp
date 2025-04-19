#ifndef MAP_MAP_STATIC_OBJECT_HPP
#define MAP_MAP_STATIC_OBJECT_HPP

#include <Strategic/HitActor.hpp>

class JPABaseEmitter;
class J3DModelData;
class TGraphWeb;
class MSSceneSE;
class MActor;
class TMapCollisionManager;

class TMapStaticObj : public THitActor {
public:
	TMapStaticObj(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();

	void init(const char*);
	void initModel(const char*);
	void initMapCollision(const char*);
	void initUnique();
	void perform(u32, JDrama::TGraphics*);
	void calcUnique(JPABaseEmitter*);
	void getModel() const;
	J3DModelData* getModelData() const;

	struct ActorDataTableEntry {
		/* 0x0 */ const char* unk0;
		/* 0x4 */ u32 unk4;
		/* 0x8 */ u32 unk8;
		/* 0xC */ f32 unkC;
		/* 0x10 */ f32 unk10;
		/* 0x14 */ f32 unk14;
		/* 0x18 */ f32 unk18;
		/* 0x1C */ const char* unk1C;
		/* 0x20 */ const char* unk20;
		/* 0x24 */ u32 unk24;
		/* 0x28 */ const char* unk28;
		/* 0x2C */ u32 unk2C;
		/* 0x30 */ u32 unk30;
		/* 0x34 */ const char* unk34;
		/* 0x38 */ u32 unk38;
		/* 0x3C */ u32 unk3C;
		/* 0x40 */ u32 unk40;
	};

	static f32 mEffectCoronaScale;

public:
	/* 0x68 */ const ActorDataTableEntry* unk68;
	/* 0x6C */ const char* unk6C;
	/* 0x70 */ MActor* unk70;
	/* 0x74 */ TMapCollisionManager* unk74;
	/* 0x78 */ u32 unk78;
};

class TMapModelActor : public THitActor {
public:
	TMapModelActor();
	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x68 */ MActor* unk68;
};

class TMapObjSoundGroup : public JDrama::TViewObj {
public:
	TMapObjSoundGroup(const char*);
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ MSSceneSE* unk10;
	/* 0x14 */ TGraphWeb* unk14;
	/* 0x18 */ u32 unk18;
};

#endif
