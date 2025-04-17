#ifndef MAP_POLLUTION_OBJ_HPP
#define MAP_POLLUTION_OBJ_HPP

#include <Map/JointObj.hpp>

class TPollutionLayer;

class TPollutionObj : public TJointObj {
public:
	TPollutionObj();
	void initAreaInfo(TPollutionLayer*);
	bool isCleaned() const;
	void updateDepthMap();
	void getDepthFromMap(int, int);
	virtual TJointObj* newJointObj() const { return new TPollutionObj; }

public:
	/* 0x20 */ int unk20;
	/* 0x24 */ int unk24;
	/* 0x28 */ int unk28;
	/* 0x2C */ int unk2C;
	/* 0x30 */ u32 unk30;
	/* 0x34 */ TPollutionLayer* unk34;
};

#endif
