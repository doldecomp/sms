#ifndef MOVE_BG_MOVE_OBJ_MANAGER_HPP
#define MOVE_BG_MOVE_OBJ_MANAGER_HPP

#include <Strategic/LiveManager.hpp>

class TMapObjBase;

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
};

#endif
