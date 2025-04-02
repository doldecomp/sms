#ifndef MAP_MAP_MODEL_HPP
#define MAP_MAP_MODEL_HPP

#include <Map/JointModel.hpp>
#include <Map/JointModelManager.hpp>

class MActorAnmData;

class TMapModel : public TJointModel {
public:
	TMapModel();

	virtual void initJointModel(TJointModelManager*, const char*,
	                            MActorAnmData*);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void getJ3DModelDataFlag() const { }

	void initUnderpass();
};

class TMapModelManager : public TJointModelManager {
public:
	TMapModelManager(const char*);
	virtual ~TMapModelManager();
	virtual void newJointModel(int) const;

	void init();
};

#endif
