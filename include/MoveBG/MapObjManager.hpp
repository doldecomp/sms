#ifndef MOVE_BG_MOVE_OBJ_MANAGER_HPP
#define MOVE_BG_MOVE_OBJ_MANAGER_HPP

#include <Strategic/LiveManager.hpp>

class TMapObjBase;
class J3DMaterialTable;
class TMapObjBaseManager;

extern TMapObjBaseManager* gpMapObjManager;

class TMapObjBaseManager : public TLiveManager {
public:
	~TMapObjBaseManager();
	TMapObjBaseManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual void clipActors(JDrama::TGraphics*);

	void getObjNumWithActorType(u32) const;
	void getActorTypeByEventID(u32);
	void newAndRegisterObjByEventID(u32, const char*);
	void newAndRegisterObj(const char*, const JGeometry::TVec3<f32>&,
	                       const JGeometry::TVec3<f32>&,
	                       const JGeometry::TVec3<f32>&);
	void makeObjAppeared(u32);
	void makeObjAppear(u32);
	void makeObjAppear(f32, f32, f32, u32, bool);
	void canAppear(const TMapObjBase*, u32) const;

	// fabricated
	MActorAnmData* getUnk40() { return unk40; }
	J3DMaterialTable* getUnk68() { return unk68; }

public:
	/* 0x38 */ f32 unk38;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ MActorAnmData* unk40;
	/* 0x44 */ char unk44[0x68 - 0x44];
	/* 0x68 */ J3DMaterialTable* unk68;
};

#endif
