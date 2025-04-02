#ifndef MAP_JOINT_MODEL_HPP
#define MAP_JOINT_MODEL_HPP

#include <JSystem/JDrama/JDRGraphics.hpp>
#include <Map/JointObj.hpp>

class TJointModelManager;
class MActorAnmData;

class TJointModel : public TJointObj {
public:
	TJointModel();

	virtual void initJointModel(TJointModelManager*, const char*,
	                            MActorAnmData*);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void getJ3DModelDataFlag() const { }

	void initActor(const char*, MActorAnmData*);
};

#endif
