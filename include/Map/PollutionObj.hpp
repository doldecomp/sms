#ifndef MAP_POLLUTION_OBJ_HPP
#define MAP_POLLUTION_OBJ_HPP

#include <Map/JointObj.hpp>

class TPollutionLayer;

/**
 * @brief A subregion of a layer that has it's own pollution counter.
 * @details Used for checking whether a particular building or other such thing
 * is fully cleaned or not.
 */
class TPollutionObj : public TJointObj {
public:
	TPollutionObj();
	void initAreaInfo(TPollutionLayer*);
	bool isCleaned() const;
	void updateDepthMap();
	u8 getDepthFromMap(int, int);
	virtual TJointObj* newJointObj() const { return new TPollutionObj; }

	TPollutionObj* getChild(int idx)
	{
		return (TPollutionObj*)TJointObj::getChild(idx);
	}

public:
	/* 0x20 */ int mMinS;
	/* 0x24 */ int mMaxS;
	/* 0x28 */ int mMinT;
	/* 0x2C */ int mMaxT;
	/* 0x30 */ u32 mCounter;
	/* 0x34 */ TPollutionLayer* mLayer;
};

#endif
