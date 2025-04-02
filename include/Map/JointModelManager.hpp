#ifndef MAP_JOINT_MODEL_MANAGER_HPP
#define MAP_JOINT_MODEL_MANAGER_HPP

#include <M3DUtil/MActorData.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>

class TJointModel;

class TJointModelManager : JDrama::TViewObj {
public:
	TJointModelManager(const char*);

	virtual ~TJointModelManager() { }
	virtual void perform(unsigned long, JDrama::TGraphics*);
	virtual TJointModel* newJointModel(int) const;

	void initJointModel(const char*, const char**);

	// fabricated
	const char* getFolder() { return unk1C; }

public:
	/* 0x10 */ u32 unk10;
	/* 0x14 */ TJointModel** unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ const char* unk1C;
	/* 0x20 */ MActorAnmData unk20;
};

#endif
