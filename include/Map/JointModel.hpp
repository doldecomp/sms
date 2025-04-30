#ifndef MAP_JOINT_MODEL_HPP
#define MAP_JOINT_MODEL_HPP

#include <JSystem/JDrama/JDRGraphics.hpp>
#include <Map/JointObj.hpp>

class TJointModelManager;
class MActorAnmData;
class J3DModelData;
class J3DModel;
class MActor;

class TJointModel : public TJointObj {
public:
	TJointModel();

	virtual void initJointModel(TJointModelManager*, const char*,
	                            MActorAnmData*);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual u32 getJ3DModelDataFlag() const { return 0x10020000; }

	void initActor(const char*, MActorAnmData*);

	// fabriacted
	J3DModel* getModel() { return mModel; }
	J3DModelData* getModelData() { return mModelData; }

public:
	/* 0x20 */ TJointModelManager* mManager;
	/* 0x24 */ J3DModelData* mModelData;
	/* 0x28 */ J3DModel* mModel;
	/* 0x2C */ MActor* mActor;
};

#endif
