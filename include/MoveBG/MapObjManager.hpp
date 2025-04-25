#ifndef MOVE_BG_MOVE_OBJ_MANAGER_HPP
#define MOVE_BG_MOVE_OBJ_MANAGER_HPP

#include <Strategic/LiveManager.hpp>

class TMapObjBase;
class J3DMaterialTable;
class J3DModel;

class TMapObjBaseManager : public TLiveManager {
public:
	TMapObjBaseManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual void clipActors(JDrama::TGraphics*);

	void getObjNumWithActorType(u32) const;
	void getActorTypeByEventID(u32);
	void newAndRegisterObjByEventID(u32, const char*);
	static TMapObjBase* newAndRegisterObj(const char*,
	                                      const JGeometry::TVec3<f32>&,
	                                      const JGeometry::TVec3<f32>&,
	                                      const JGeometry::TVec3<f32>&);
	void makeObjAppeared(u32);
	void makeObjAppear(u32);
	void makeObjAppear(f32, f32, f32, u32, bool);
	void canAppear(const TMapObjBase*, u32) const;

public:
	/* 0x38 */ f32 unk38;
	/* 0x3C */ f32 unk3C;
};

class TMapObjManager;

extern TMapObjManager* gpMapObjManager;

class TMapObjManager : public TMapObjBaseManager {
public:
	void initKeyCode();

	TMapObjManager(const char*);
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual bool hasMapCollision() const { return true; }

	void loadMatTable(const char*);
	void initDrawBuffer();
	void entryStaticDrawBufferSun(J3DModel*);
	void entryStaticDrawBufferShadow(J3DModel*);

	// fabricated
	MActorAnmData* getUnk40() { return unk40; }
	J3DMaterialTable* getUnk68() { return unk68; }

public:
	/* 0x40 */ MActorAnmData* unk40;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ f32 unk48;
	/* 0x4C */ f32 unk4C;
	/* 0x50 */ f32 unk50;
	/* 0x54 */ f32 unk54;
	/* 0x58 */ f32 unk58;
	/* 0x5C */ f32 unk5C;
	/* 0x60 */ f32 unk60;
	/* 0x64 */ f32 unk64;
	/* 0x68 */ J3DMaterialTable* unk68;
	/* 0x6C */ J3DMaterialTable* unk6C;
	/* 0x70 */ J3DMaterialTable* unk70;
	/* 0x74 */ J3DMaterialTable* unk74;
	/* 0x78 */ J3DMaterialTable* unk78;
	/* 0x7C */ J3DMaterialTable* unk7C;
	/* 0x80 */ J3DMaterialTable* unk80;
	/* 0x84 */ J3DMaterialTable* unk84;
	/* 0x88 */ J3DMaterialTable* unk88;
	/* 0x8C */ J3DMaterialTable* unk8C;
	/* 0x90 */ J3DMaterialTable* unk90;
	/* 0x94 */ J3DMaterialTable* unk94;
	/* 0x98 */ u32 unk98;
	/* 0x9C */ u32 unk9C;
	/* 0xA0 */ u32 unkA0;
	/* 0xA4 */ u32 unkA4;
	/* 0xA8 */ u16 unkA8;
	/* 0xAA */ u16 unkAA;
	/* 0xAC */ u16 unkAC;
	/* 0xAE */ u16 unkAE;
	/* 0xB0 */ u16 unkB0;
	/* 0xB2 */ u16 unkB2;
	/* 0xB4 */ u16 unkB4;
	/* 0xB6 */ u16 unkB6;
	/* 0xB8 */ u16 unkB8;
	/* 0xBA */ u16 unkBA;
	/* 0xBC */ u16 unkBC;
	/* 0xBE */ u16 unkBE;
	/* 0xC0 */ J3DMaterialTable* unkC0;
	/* 0xC4 */ J3DMaterialTable* unkC4;
	/* 0xC8 */ u32 unkC8;
	/* 0xCC */ u32 unkCC;
	/* 0xD0 */ f32 unkD0;
	/* 0xD4 */ f32 unkD4;
	/* 0xD8 */ f32 unkD8;
};

#endif
