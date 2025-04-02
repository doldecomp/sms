#ifndef MAP_JOINT_MODEL_MANAGER_HPP
#define MAP_JOINT_MODEL_MANAGER_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TJointModelManager : JDrama::TViewObj {
public:
	TJointModelManager(const char*);

	virtual ~TJointModelManager() { }
	virtual void perform(unsigned long, JDrama::TGraphics*);
	virtual void newJointModel(int) const;

	void initJointModel(const char*, const char**);
};

#endif
