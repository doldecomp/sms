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
};

#endif
