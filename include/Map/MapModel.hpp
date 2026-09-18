#ifndef MAP_MAP_MODEL_HPP
#define MAP_MAP_MODEL_HPP

#include <Map/JointModel.hpp>
#include <Map/JointModelManager.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>

class MActorAnmData;
class TMapCollisionStatic;
class J3DMaterial;

class TMapModel : public TJointModel {
public:
	TMapModel();

	virtual void initJointModel(TJointModelManager*, const char*,
	                            MActorAnmData*);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual u32 getJ3DModelDataFlag() const
	{
		return J3DMLF_MaterialPEFull | (2 << J3DMLF_TevStageNumShift);
	}

	void initUnderpass();

	// Fabricated name. The accessor level supplies the last 8 bytes of
	// initUnderpass' 0x70 frame; it is +8 at two or three of its call sites
	// (one is +0, four or five +0x10). Only the byte count picks those sites,
	// so an alternative reading is that retail reached getTexCoord()/
	// getZMode() through one more forwarding level than our J3DMaterial does.
	J3DMaterial* getUnderpassMaterial() { return mUnderpassMaterial; }

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
