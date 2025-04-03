#ifndef MAP_MAP_MODEL_HPP
#define MAP_MAP_MODEL_HPP

#include <Map/JointModel.hpp>
#include <Map/JointModelManager.hpp>

class MActorAnmData;
class TMapCollisionStatic;
class J3DMaterial;

class TMapModel : public TJointModel {
public:
	TMapModel()
	    : mUnderpass(nullptr)
	    , mUnderpassMaterial(nullptr)
	    , unk38(0.5f)
	    , unk3C(1.0f)
	{
	}

	virtual void initJointModel(TJointModelManager*, const char*,
	                            MActorAnmData*);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual u32 getJ3DModelDataFlag() const { return 0x10020000; }

	void initUnderpass();

public:
	/* 0x30 */ TJointObj* mUnderpass;
	/* 0x34 */ J3DMaterial* mUnderpassMaterial;
	/* 0x38 */ f32 unk38;
	/* 0x3C */ f32 unk3C;
};

class TMapModelManager : public TJointModelManager {
public:
	TMapModelManager(const char*);
	virtual ~TMapModelManager() { }
	virtual TMapModel* newJointModel(int) const { return new TMapModel(); }

	void init();

public:
	/* 0x6C */ TMapCollisionStatic* mCollision;
};

#endif
